/**
 * @file llama_jni_sampler.cpp
 *
 * Implementation of sampler‑related helper functions for the JNI llama.cpp
 * integration layer. This file constructs a sampler chain using all configured
 * sampling strategies, including:
 *
 *  - Hard‑coded banned token sampler (custom)
 *  - Repetition / frequency / presence penalties
 *  - DRY sampling
 *  - Min‑p sampling
 *  - Temperature scaling
 *  - RNG distribution
 *
 * All declarations are defined in llama_jni_internal.hpp.
 */

#include "llama_jni_internal.hpp"
#include <vector>
#include <string>
#include <cmath>

// ============================================================================
// Custom sampler: Hard-ban specific tokens + non-ASCII tokens
// ============================================================================

bool is_printable(const std::string & s) {
    for (unsigned char c : s) {
            if (!(
                (c >= 0 && c <= 126) ||  // printable ASCII
                c == 9  ||               // tab
                c == 10 ||               // newline
                c == 13 ||                  // carriage return
                c == ' ' ||
                c == 160 ||
                c == 173 ||
                c == '\t' || 
                c == '\n' ||
                c == '\r' ||
                (c >= 0xC2)
            )) {
               return false;
            }
        
    }
    return true;
}


static llama_sampler * create_ban_sampler(
        const llama_vocab * vocab,
        const std::vector<std::string> & phrases) {

    std::vector<llama_logit_bias> biases;
    int vocab_size = llama_vocab_n_tokens(vocab);

    // ---------------------------------------------------------------------
    // 1. Ban tokens belonging to banned phrases
    // ---------------------------------------------------------------------
    for (const auto & phrase : phrases) {

        std::vector<llama_token> toks(128);

        int n = llama_tokenize(
            vocab,
            phrase.c_str(),
            (int) phrase.size(),
            toks.data(),
            (int) toks.size(),
            true,   // add_special
            true    // parse_special
        );

        if (n > 0) {
            for (int i = 0; i < n; i++) {
                llama_logit_bias b;
                b.token = toks[i];
                b.bias  = -15.0f;   // hard ban
                biases.push_back(b);
            }
        }
    }

    // ---------------------------------------------------------------------
    // 2. Ban all tokens that decode to non-printable ASCII
    // ---------------------------------------------------------------------

    for (int id = 0; id < vocab_size; id++) {
        const char * text = llama_token_get_text(vocab, id);
        if (!is_printable(std::string(text))) {
            biases.push_back({ id, -15.0f });
        }
    }

    // ---------------------------------------------------------------------
    // 3. Create logit-bias sampler
    // ---------------------------------------------------------------------
    return llama_sampler_init_logit_bias(
        vocab_size,
        biases.size(),
        biases.data()
    );
}

// ============================================================================
// Main sampler chain builder
// ============================================================================

llama_sampler* create_sampler(
        const llama_vocab* vocab,
        llama_model* model,
        float temperature,
        float dynamicTemperature,
        float minP,
        int   penaltyLastN,
        float repeatPenalty,
        float freqPenalty,
        float presencePenalty,
        float dryMultiplier,
        float dryBase,
        int   dryAllowedLength,
        int   dryPenaltyLastN,
        int   rngSeed) {

    llama_sampler_chain_params chain_params = llama_sampler_chain_default_params();
    llama_sampler* sampler = llama_sampler_chain_init(chain_params);

    if (!sampler) {
        LOG("create_sampler: FAILED to initialize sampler chain");
        return nullptr;
    }

    // ---------------------------------------------------------------------
    // Hard‑coded banned phrases → tokenize → ban tokens if you wanted...
    // ---------------------------------------------------------------------
    std::vector<std::string> banned_phrases = {
    };

    llama_sampler* ban = create_ban_sampler(vocab, banned_phrases);
    llama_sampler_chain_add(sampler, ban);

    // ---------------------------------------------------------------------
    // Penalty sampler (repetition, frequency, presence)
    // ---------------------------------------------------------------------
    llama_sampler* penalties = llama_sampler_init_penalties(
        penaltyLastN,
        repeatPenalty,
        freqPenalty,
        presencePenalty
    );

    llama_sampler_chain_add(sampler, penalties);

    // ---------------------------------------------------------------------
    // DRY sampler (requires training context length)
    // ---------------------------------------------------------------------
    int train_ctx = 2048;
    char buf[64] = {0};

    if (llama_model_meta_val_str(model, "llama.context_length", buf, sizeof(buf))) {
        train_ctx = atoi(buf);
        LOG("create_sampler: training context length = %d", train_ctx);
    } else {
        LOG("create_sampler: training context length not found, using default 2048");
    }

    llama_sampler* dry = llama_sampler_init_dry(
        vocab,
        train_ctx,
        dryMultiplier,
        dryBase,
        dryAllowedLength,
        dryPenaltyLastN,
        nullptr,
        0
    );

    llama_sampler_chain_add(sampler, dry);

    // ---------------------------------------------------------------------
    // |                     Min‑p sampler                                 |
    // ---------------------------------------------------------------------
    llama_sampler* minp = llama_sampler_init_min_p(minP, 1);
    llama_sampler_chain_add(sampler, minp);

    // ---------------------------------------------------------------------
    // |             Temperature sampler (extended)                        |
    // ---------------------------------------------------------------------
    llama_sampler* temp = llama_sampler_init_temp_ext(
        temperature,
        dynamicTemperature,
        1.0f
    );

    llama_sampler_chain_add(sampler, temp);

    // ---------------------------------------------------------------------
    // RNG sampler (distribution)
    // ---------------------------------------------------------------------
    llama_sampler* dist = llama_sampler_init_dist(rngSeed);
    llama_sampler_chain_add(sampler, dist);

    LOG("create_sampler: sampler chain successfully constructed");

    return sampler;
}
