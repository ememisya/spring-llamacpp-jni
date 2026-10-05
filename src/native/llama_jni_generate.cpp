/**
 * @file llama_jni_generate.cpp
 *
 * Implementation of prompt tokenization and the main generation loop for the
 * JNI llama.cpp integration layer. This file handles:
 *
 *  - Converting a prompt string into llama_token values
 *  - Feeding prompt tokens into the model in safe batches
 *  - Sampling tokens autoregressively
 *  - Converting sampled tokens back into UTF‑8 text
 *
 * All declarations are defined in llama_jni_internal.hpp.
 */

#include "llama_jni_internal.hpp"

/**
 * Tokenize a prompt into llama_token values.
 *
 * llama.cpp requires a two‑step tokenization process:
 *  1. Call with a null buffer to determine required token count
 *  2. Allocate buffer and call again to fill it
 *
 * @param vocab   Vocabulary associated with the model
 * @param ctx     Active llama_context
 * @param prompt  UTF‑8 prompt string to tokenize
 *
 * @return A vector of llama_token values, or an empty vector on failure
 */
std::vector<llama_token> tokenize_prompt(
        const llama_vocab* vocab,
        llama_context* ctx,
        const std::string& prompt) {

    std::vector<llama_token> tokens;

    bool is_first =
        llama_memory_seq_pos_max(llama_get_memory(ctx), 0) == -1;

    // First call: determine required token count
    int32_t count = -llama_tokenize(
        vocab,
        prompt.c_str(),
        static_cast<int32_t>(prompt.size()),
        nullptr,
        0,
        is_first,
        true
    );

    if (count < 0) {
        LOG("tokenize_prompt: failed to compute token count");
        return tokens;
    }

    tokens.resize(count);

    // Second call: fill token buffer
    int32_t ok = llama_tokenize(
        vocab,
        prompt.c_str(),
        static_cast<int32_t>(prompt.size()),
        tokens.data(),
        count,
        is_first,
        true
    );

    if (ok < 0) {
        LOG("tokenize_prompt: tokenization failed");
        tokens.clear();
    }

    LOG("tokenize_prompt: produced %d tokens", (int)tokens.size());
    return tokens;
}

/**
 * Execute the main generation loop:
 *
 *  1. Feed prompt tokens into the model in batches
 *  2. Sample tokens autoregressively
 *  3. Convert tokens to UTF‑8 text
 *
 * @param ctx           Active llama_context
 * @param sampler       Configured llama_sampler chain
 * @param vocab         Vocabulary associated with the model
 * @param prompt_tokens Tokenized prompt to feed into the model
 *
 * @return Generated UTF‑8 text
 */
std::string run_generation_loop(
        llama_context* ctx,
        llama_sampler* sampler,
        const llama_vocab* vocab,
        const std::vector<llama_token>& prompt_tokens) {

    std::string output;

    const int32_t n_batch = llama_n_batch(ctx);

    // ---------------------------------------------------------------------
    // Step 1: Feed prompt tokens in safe chunks
    // ---------------------------------------------------------------------
    int32_t pos = 0;
    while (pos < static_cast<int32_t>(prompt_tokens.size())) {
        int32_t chunk =
            std::min(n_batch,
                     static_cast<int32_t>(prompt_tokens.size()) - pos);

        std::vector<llama_token> tmp(
            prompt_tokens.begin() + pos,
            prompt_tokens.begin() + pos + chunk
        );

        llama_batch batch = llama_batch_get_one(tmp.data(), chunk);

        if (llama_decode(ctx, batch) != 0) {
            LOG("run_generation_loop: llama_decode failed during prompt feed");
            break;
        }

        pos += chunk;
    }

    // ---------------------------------------------------------------------
    // Step 2: Autoregressive generation
    // ---------------------------------------------------------------------
    while (true) {
        llama_token tok = llama_sampler_sample(sampler, ctx, -1);

        if (llama_vocab_is_eog(vocab, tok)) {
            LOG("run_generation_loop: EOG token reached");
            break;
        }

        char buf[256];
        int32_t n = llama_token_to_piece(
            vocab,
            tok,
            buf,
            sizeof(buf),
            0,
            true
        );

        if (n < 0) {
            LOG("run_generation_loop: failed to convert token to text");
            break;
        }

        output.append(buf, n);

        std::printf("%.*s", n, buf);
        std::fflush(stdout);

        llama_batch batch = llama_batch_get_one(&tok, 1);

        if (llama_decode(ctx, batch) != 0) {
            LOG("run_generation_loop: llama_decode failed during generation");
            break;
        }
    }

    LOG("run_generation_loop: generated %zu bytes of text", output.size());
    return output;
}
