/**
 * @file llama_jni_internal.hpp
 *
 * Shared declarations for the JNI llama.cpp integration layer.
 *
 * This header exposes:
 *  - Helper functions for context creation
 *  - Sampler construction utilities
 *  - Message extraction and template application
 *  - Prompt tokenization
 *  - Generation loop
 *
 * All JNI entry points are defined in llama_jni.cpp.
 */

#ifndef LLAMA_JNI_INTERNAL_HPP
#define LLAMA_JNI_INTERNAL_HPP

#include <jni.h>
#include <string>
#include <vector>
#include <string.h>

#include "llama.h"
#include "ggml.h"
#include "ggml-cpu.h"
#include "ggml-backend.h"
#include "ggml-opt.h"

/**
 * Logging macro (enabled only when DEBUG_LLAMA_JNI is defined).
 */
#ifdef DEBUG_LLAMA_JNI
#define LOG(fmt, ...) std::printf("[LLAMA-JNI] " fmt "\n", ##__VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif

/**
 * Extract an int value from a Java object via getter.
 *
 * @param env    JNI environment
 * @param obj    Java object
 * @param getter Name of the getter method
 * @return       Extracted int value
 */
int get_int_field(JNIEnv* env, jobject obj, const char* getter);

/**
 * Extract a float value from a Java object via getter.
 *
 * @param env    JNI environment
 * @param obj    Java object
 * @param getter Name of the getter method
 * @return       Extracted float value
 */
float get_float_field(JNIEnv* env, jobject obj, const char* getter);

/**
 * Create a llama_context from a model and configuration values.
 *
 * @param model       Loaded llama_model
 * @param contextSize Context length (n_ctx)
 * @param batchSize   Batch size (n_batch)
 * @param uBatchSize  Micro-batch size (n_ubatch)
 * @return            Newly created llama_context
 */
llama_context* create_context(
    llama_model* model,
    int contextSize,
    int batchSize,
    int uBatchSize
);

/**
 * Create a sampler chain using all configured sampling strategies.
 */
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
    int   rngSeed
);

/**
 * Extract chat messages from Java ChatParams into llama_chat_message structs.
 */
std::vector<llama_chat_message> extract_messages(
    JNIEnv* env,
    jobject chatParamsObj
);

/**
 * Free memory allocated for extracted llama_chat_message objects.
 */
void free_messages(std::vector<llama_chat_message>& msgs);

/**
 * Apply the model's chat template to a list of messages.
 */
std::string apply_chat_template(
    llama_model* model,
    llama_context* ctx,
    const std::vector<llama_chat_message>& msgs
);

/**
 * Tokenize a prompt into llama_token values.
 */
std::vector<llama_token> tokenize_prompt(
    const llama_vocab* vocab,
    llama_context* ctx,
    const std::string& prompt
);

/**
 * Execute the main generation loop:
 *  - Feed prompt tokens
 *  - Sample tokens
 *  - Convert tokens to text
 */
std::string run_generation_loop(
    llama_context* ctx,
    llama_sampler* smpl,
    const llama_vocab* vocab,
    const std::vector<llama_token>& prompt_tokens
);

#endif // LLAMA_JNI_INTERNAL_HPP
