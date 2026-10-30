/**
 * @file llama_jni_generate.cpp
 *
 * Implementation of prompt tokenization and the main generation loop for the
 * JNI llama.cpp integration layer.
 */

#include "llama_jni_internal.hpp"
#include "bert-emotion-classifier.h"

namespace {

/**
 * Initialize a llama_batch with a specified number of tokens.
 *
 * @param n_tokens Number of tokens the batch will hold
 * @return Initialized llama_batch structure
 */
inline llama_batch create_initialized_batch(int32_t n_tokens) {
    llama_batch batch = llama_batch_init(n_tokens, 0, 1);
    batch.n_tokens = n_tokens;
    return batch;
}

/**
 * Configure properties for a single element in a llama_batch.
 *
 * @param batch Reference to the llama_batch structure
 * @param i Index within the batch
 * @param token Token value to assign
 * @param pos Position index
 * @param seq_id Sequence identifier for the token
 * @param logits Logits flag (1 or 0)
 */
inline void set_batch_element(llama_batch& batch, int32_t i, llama_token token, llama_pos pos, llama_seq_id seq_id, int8_t logits) {
    batch.token[i] = token;
    batch.pos[i] = pos;
    batch.n_seq_id[i] = 1;
    batch.seq_id[i][0] = seq_id;
    batch.logits[i] = logits;
}

/**
 * Compute pooler layer embeddings using tanh activation.
 *
 * @param head Classifier head configuration and weights
 * @param all_embds Raw embeddings tensor buffer from context
 * @return Vector of computed pooler float values
 */
std::vector<float> compute_pooler(const ClassifierHead& head, const float* all_embds) {
    std::vector<float> pooled(head.pooler_dim, 0.0f);
    for (int p = 0; p < head.pooler_dim; ++p) {
        float sum = head.pooler_biases[p];
        const float* weight_row = &head.pooler_weights[p * head.input_dim];
        for (int i = 0; i < head.input_dim; ++i) {
            sum += weight_row[i] * all_embds[i];
        }
        pooled[p] = std::tanh(sum);
    }
    return pooled;
}

/**
 * Calculate classification logits and format results into a string.
 *
 * @param model Active llama_model instance
 * @param head Classifier head configuration and weights
 * @param pooled Computed pooler embedding vector
 * @param num_classes Number of classification output categories
 * @return Formatted string containing percentage probabilities for each class
 */
std::string format_classifier_logits(const struct llama_model* model, const ClassifierHead& head, const std::vector<float>& pooled, int num_classes) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << "{";
    for (int c = 0; c < num_classes; ++c) {
        float score = head.biases[c];
        const float* weight_row = &head.weights[c * head.pooler_dim];
        for (int p = 0; p < head.pooler_dim; ++p) {
            score += weight_row[p] * pooled[p];
        }
        float prob = 1.0f / (1.0f + std::exp(-score));
        ss << "\"" << llama_model_cls_label(model, c) << "\":" << (prob * 100.0f);
        if (c < num_classes - 1) ss << ",";
    }
    return ss.str() + "}";
}

/**
 * Feed prompt tokens into the model in safe batches.
 *
 * @param ctx Active llama_context
 * @param tokens Vector of tokenized prompt values
 * @return True on success, false on decoding failure
 */
bool feed_prompt(llama_context* ctx, const std::vector<llama_token>& tokens) {
    int32_t n_batch = llama_n_batch(ctx), total = tokens.size(), pos = 0;
    while (pos < total) {
        int32_t chunk = std::min(n_batch, total - pos);
        llama_batch batch = create_initialized_batch(chunk);
        for (int32_t i = 0; i < chunk; ++i) {
            set_batch_element(batch, i, tokens[pos + i], pos + i, 0, ((pos + i) == total - 1) ? 1 : 0);
        }
        if (llama_decode(ctx, batch) != 0) {
            llama_batch_free(batch);
            return false;
        }
        llama_batch_free(batch);
        pos += chunk;
    }
    return true;
}

/**
 * Coax EOG tokens when approaching the generation limit using a soft limit penalty.
 *
 * @param ctx Active llama_context
 * @param vocab Vocabulary associated with the model
 * @param gen_count Number of tokens generated so far
 * @param soft_limit Threshold at which to start coaxing EOG
 * @param soft_limit_penalty Magnitude of the logit inflation penalty
 */
void handle_soft_limit(llama_context* ctx, const llama_vocab* vocab, int gen_count, int soft_limit, float soft_limit_penalty) {
    if (gen_count >= soft_limit) {
        if (float* logits = llama_get_logits_ith(ctx, -1)) {
            logits[llama_vocab_eos(vocab)] += soft_limit_penalty;
        }
    }
}

/**
 * Decode a single generated token into the context.
 *
 * @param ctx Active llama_context
 * @param tok Single token to decode
 * @param pos Current position index in the sequence
 * @return True on success, false on failure
 */
bool decode_single_token(llama_context* ctx, llama_token tok, int32_t pos) {
    llama_batch batch = create_initialized_batch(1);
    set_batch_element(batch, 0, tok, pos, 0, 1);
    bool ok = (llama_decode(ctx, batch) == 0);
    llama_batch_free(batch);
    return ok;
}

} // namespace

/**
 * Tokenize a prompt into llama_token values.
 *
 * @param vocab Vocabulary associated with the model
 * @param ctx Active llama_context
 * @param prompt UTF-8 prompt string to tokenize
 * @return A vector of llama_token values, or an empty vector on failure
 */
std::vector<llama_token> tokenize_prompt(
        const llama_vocab* vocab,
        llama_context* ctx,
        const std::string& prompt) {
    bool is_first = llama_memory_seq_pos_max(llama_get_memory(ctx), 0) == -1;
    int32_t count = -llama_tokenize(vocab, prompt.c_str(), prompt.size(), nullptr, 0, is_first, true);
    if (count < 0) return {};
    std::vector<llama_token> tokens(count);
    if (llama_tokenize(vocab, prompt.c_str(), prompt.size(), tokens.data(), count, is_first, true) < 0) {
        return {};
    }
    return tokens;
}

/**
 * Executes BERT encoder inference using llama.cpp embeddings.
 *
 * @param ctx Active llama_context
 * @param vocab Vocabulary associated with the model
 * @param prompt_tokens Tokenized input prompt
 * @return Formatted string containing percentage probabilities for each class
 */
std::string run_encoder_inference(
        llama_context* ctx,
        const llama_vocab* vocab,
        const std::vector<llama_token>& prompt_tokens) {
    const struct llama_model* model = llama_get_model(ctx);
    int num_classes = model ? llama_model_n_cls_out(model) : 0;
    if (num_classes <= 0) return "Error: Model or classes invalid";

    llama_batch batch = create_initialized_batch(prompt_tokens.size());
    for (size_t i = 0; i < prompt_tokens.size(); ++i) {
        set_batch_element(batch, i, prompt_tokens[i], i, 0, 1);
    }
    if (llama_encode(ctx, batch) != 0) {
        llama_batch_free(batch);
        return "";
    }
    llama_batch_free(batch);

    const float* embds = llama_get_embeddings(ctx);
    if (!embds) return "";
    const ClassifierHead& head = get_classifier(bert_emotion_classifier);
    auto pooled = compute_pooler(head, embds);
    return format_classifier_logits(model, head, pooled, num_classes);
}

/**
 * Execute the main autoregressive generation loop.
 *
 * @param ctx Active llama_context
 * @param sampler Configured llama_sampler chain
 * @param vocab Vocabulary associated with the model
 * @param prompt_tokens Tokenized prompt to feed into the model
 * @param max_tokens Hard maximum token generation limit
 * @param soft_limit Soft limit threshold for EOG coaxing
 * @param soft_limit_penalty Magnitude of the logit inflation penalty
 * @return Generated UTF-8 text
 */
std::string run_generation_loop(
        llama_context* ctx,
        llama_sampler* sampler,
        const llama_vocab* vocab,
        const std::vector<llama_token>& prompt_tokens,
        int32_t max_tokens,
        int32_t soft_limit,
        float soft_limit_penalty) {
    if (!feed_prompt(ctx, prompt_tokens)) return "";

    std::string output;
    int32_t pos = prompt_tokens.size();
    int32_t gen_count = 0;

    while (gen_count < max_tokens) {
        handle_soft_limit(ctx, vocab, gen_count, soft_limit, soft_limit_penalty);
        llama_token tok = llama_sampler_sample(sampler, ctx, -1);
        if (llama_vocab_is_eog(vocab, tok)) break;

        char buf[256];
        int32_t n = llama_token_to_piece(vocab, tok, buf, sizeof(buf), 0, true);
        if (n < 0) break;

        output.append(buf, n);
        std::printf("%.*s", n, buf);
        std::fflush(stdout);

        gen_count++;
        if (gen_count >= max_tokens || !decode_single_token(ctx, tok, pos++)) break;
    }
    return output;
}