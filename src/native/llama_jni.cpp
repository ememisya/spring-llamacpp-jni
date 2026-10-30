/**
 * @file llama_jni.cpp
 *
 * JNI entry points for the llama.cpp integration layer.
 */

#include "llama_jni_internal.hpp"

namespace {

/**
 * Extract sampling configuration parameters from a Java ChatParams object.
 *
 * @param env JNI environment
 * @param chatParamsObj Java ChatParams instance
 * @return Configured sampler instance
 */
llama_sampler* extract_and_create_sampler(JNIEnv* env, jobject chatParamsObj, const llama_vocab* vocab, const struct llama_model* model) {
    float temperature = get_float_field(env, chatParamsObj, "getTemperature");
    float dynamicTemperature = get_float_field(env, chatParamsObj, "getDynamicTemperature");
    float minP = get_float_field(env, chatParamsObj, "getMinP");
    float repeatPenalty = get_float_field(env, chatParamsObj, "getRepeatPenalty");
    float freqPenalty = get_float_field(env, chatParamsObj, "getFreqPenalty");
    float presencePenalty = get_float_field(env, chatParamsObj, "getPresencePenalty");
    int penaltyLastN = get_int_field(env, chatParamsObj, "getPenaltyLastN");
    float dryMultiplier = get_float_field(env, chatParamsObj, "getDryMultiplier");
    float dryBase = get_float_field(env, chatParamsObj, "getDryBase");
    int dryAllowedLength = get_int_field(env, chatParamsObj, "getDryAllowedLength");
    int dryPenaltyLastN = get_int_field(env, chatParamsObj, "getDryPenaltyLastN");
    int rngSeed = get_int_field(env, chatParamsObj, "getRngSeed");

    return create_sampler(
        vocab, model, temperature, dynamicTemperature, minP,
        penaltyLastN, repeatPenalty, freqPenalty, presencePenalty,
        dryMultiplier, dryBase, dryAllowedLength, dryPenaltyLastN, rngSeed
    );
}

/**
 * Execute generation or encoder inference based on model and parameters.
 *
 * @param env JNI environment
 * @param chatParamsObj Java ChatParams instance
 * @param ctx Active llama_context
 * @param vocab Vocabulary associated with the model
 * @param model Active llama_model instance
 * @param tokens Tokenized prompt values
 * @param isEncoder Whether to run encoder inference only
 * @return Resulting generated string or classification output
 */
std::string execute_generation(
        JNIEnv* env,
        jobject chatParamsObj,
        llama_context* ctx,
        const llama_vocab* vocab,
        const struct llama_model* model,
        const std::vector<llama_token>& tokens,
        jboolean isEncoder) {
    if (isEncoder || is_bert_or_t5(model)) {
        return run_encoder_inference(ctx, vocab, tokens);
    }

    int32_t max_tokens = get_int_field(env, chatParamsObj, "getMaxTokens");
    int32_t soft_limit = get_int_field(env, chatParamsObj, "getSoftLimit");
    float soft_limit_penalty = get_float_field(env, chatParamsObj, "getSoftLimitPenalty");

    llama_sampler* sampler = extract_and_create_sampler(env, chatParamsObj, vocab, model);
    std::string result = run_generation_loop(ctx, sampler, vocab, tokens, max_tokens, soft_limit, soft_limit_penalty);
    llama_sampler_free(sampler);
    return result;
}

/**
 * Common pipeline to extract messages, apply chat template, and tokenize.
 *
 * @param env JNI environment
 * @param chatParamsObj Java ChatParams instance
 * @param model Active llama_model instance
 * @param ctx Active llama_context
 * @param vocab Vocabulary associated with the model
 * @param out_tokens Output vector to store tokenized prompt
 * @return True on success, false on failure
 */
bool prepare_prompt_tokens(
        JNIEnv* env,
        jobject chatParamsObj,
        const struct llama_model* model,
        llama_context* ctx,
        const llama_vocab* vocab,
        std::vector<llama_token>& out_tokens) {
    auto msgs = extract_messages(env, chatParamsObj);
    if (msgs.empty()) return false;

    std::string prompt = apply_chat_template(model, ctx, msgs);
    free_messages(msgs);
    if (prompt.empty()) return false;

    out_tokens = tokenize_prompt(vocab, ctx, prompt);
    return !out_tokens.empty();
}

static void error_only_log_callback(enum ggml_log_level level, const char * text, void * user_data) {
    if (level == GGML_LOG_LEVEL_WARN) {
        std::fprintf(stderr, "%s", text);
    }
}

} // namespace

extern "C" {

/**
 * Load a llama model from a file path.
 *
 * @param env JNI environment
 * @param obj Java LlamaCpp instance
 * @param modelPath Path to the GGUF model file
 * @param gpuLayers Number of layers to offload to GPU
 * @return Pointer to the loaded llama_model as a jlong
 */
JNIEXPORT jlong JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_loadModel(
        JNIEnv* env,
        jobject obj,
        jstring modelPath,
        jint gpuLayers) {
    const char* path = env->GetStringUTFChars(modelPath, nullptr);
    llama_model_params params = llama_model_default_params();
    params.n_gpu_layers = gpuLayers;
    params.no_host = true;
    llama_model* model = llama_model_load_from_file(path, params);
    env->ReleaseStringUTFChars(modelPath, path);
    return reinterpret_cast<jlong>(model);
}

/**
 * Free a previously loaded llama_model.
 *
 * @param env JNI environment
 * @param obj Java LlamaCpp instance
 * @param modelPtr Pointer to llama_model
 */
JNIEXPORT void JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_freeModel(
        JNIEnv* env,
        jobject obj,
        jlong modelPtr) {
    if (auto* model = reinterpret_cast<llama_model*>(modelPtr)) {
        llama_model_free(model);
    }
}

/**
 * Initialize the llama backend.
 */
JNIEXPORT void JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_initBackend(JNIEnv* env, jobject obj) {
    llama_backend_init();
}

/**
 * Initialize the llama logging level.
 */
JNIEXPORT void JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_setLogLevel(JNIEnv* env, jobject obj) {
    llama_log_set(error_only_log_callback, nullptr);
}

/**
 * Free the llama backend.
 */
JNIEXPORT void JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_freeBackend(JNIEnv* env, jobject obj) {
    llama_backend_free();
}

/**
 * Create a reusable llama_context.
 *
 * @param env JNI environment
 * @param obj Java LlamaCpp instance
 * @param modelPtr Pointer to llama_model
 * @param ctxSize Context length (n_ctx)
 * @param batchSize Batch size (n_batch)
 * @param uBatchSize Micro-batch size (n_ubatch)
 * @return Pointer to llama_context as a jlong
 */
JNIEXPORT jlong JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_createContext(
        JNIEnv* env, jobject obj, jlong modelPtr, jint ctxSize, jint batchSize, jint uBatchSize) {
    auto* model = reinterpret_cast<llama_model*>(modelPtr);
    if (!model) return 0;
    return reinterpret_cast<jlong>(create_context(model, ctxSize, batchSize, uBatchSize));
}

/**
 * Free a previously created llama_context.
 *
 * @param env JNI environment
 * @param obj Java LlamaCpp instance
 * @param ctxPtr Pointer to llama_context
 */
JNIEXPORT void JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_freeContext(JNIEnv* env, jobject obj, jlong ctxPtr) {
    if (auto* ctx = reinterpret_cast<llama_context*>(ctxPtr)) {
        llama_free(ctx);
    }
}

/**
 * Tokenize messages and return the token count.
 *
 * @param env JNI environment
 * @param obj Java LlamaCpp instance
 * @param modelPtr Pointer to llama_model
 * @param ctxPtr Pointer to llama_context
 * @param chatParamsObj Java ChatParams instance
 * @return Token count
 */
JNIEXPORT jint JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_getTokenCount(
        JNIEnv* env, jobject obj, jlong modelPtr, jlong ctxPtr, jobject chatParamsObj) {
    auto* model = reinterpret_cast<llama_model*>(modelPtr);
    auto* ctx = reinterpret_cast<llama_context*>(ctxPtr);
    if (!model || !ctx) return -1;

    std::vector<llama_token> tokens;
    if (!prepare_prompt_tokens(env, chatParamsObj, model, ctx, llama_model_get_vocab(model), tokens)) {
        return -2;
    }
    return static_cast<jint>(tokens.size());
}

/**
 * Generate text using a stateless context path.
 *
 * @param env JNI environment
 * @param obj Java LlamaCpp instance
 * @param modelPtr Pointer to llama_model
 * @param chatParamsObj Java ChatParams instance
 * @param isEncoder Whether to run encoder inference only
 * @return Generated text as a Java string
 */
JNIEXPORT jstring JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_generate(
        JNIEnv* env, jobject obj, jlong modelPtr, jobject chatParamsObj, jboolean isEncoder) {
    auto* model = reinterpret_cast<llama_model*>(modelPtr);
    if (!model) return env->NewStringUTF("Error: null model");

    jclass cls = env->GetObjectClass(chatParamsObj);
    int ctxSize = env->CallIntMethod(chatParamsObj, env->GetMethodID(cls, "getContextSize", "()I"));
    int batchSize = env->CallIntMethod(chatParamsObj, env->GetMethodID(cls, "getBatchSize", "()I"));
    int uBatchSize = env->CallIntMethod(chatParamsObj, env->GetMethodID(cls, "getuBatchSize", "()I"));

    llama_context* ctx = create_context(model, ctxSize, batchSize, uBatchSize);
    if (!ctx) return env->NewStringUTF("Error: context creation failed");

    std::vector<llama_token> tokens;
    const auto* vocab = llama_model_get_vocab(model);
    if (!prepare_prompt_tokens(env, chatParamsObj, model, ctx, vocab, tokens)) {
        llama_free(ctx);
        return env->NewStringUTF("Error: template or tokenization failed");
    }

    std::string result = execute_generation(env, chatParamsObj, ctx, vocab, model, tokens, isEncoder);
    llama_free(ctx);
    return env->NewStringUTF(result.c_str());
}

/**
 * Generate text using a stateful context path.
 *
 * @param env JNI environment
 * @param obj Java LlamaCpp instance
 * @param modelPtr Pointer to llama_model
 * @param ctxPtr Pointer to llama_context
 * @param chatParamsObj Java ChatParams instance
 * @param isEncoder Whether to run encoder inference only
 * @return Generated text as a Java string
 */
JNIEXPORT jstring JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_generateWithContext(
        JNIEnv* env, jobject obj, jlong modelPtr, jlong ctxPtr, jobject chatParamsObj, jboolean isEncoder) {
    auto* model = reinterpret_cast<llama_model*>(modelPtr);
    auto* ctx = reinterpret_cast<llama_context*>(ctxPtr);
    if (!model || !ctx) return env->NewStringUTF("Error: null model or context");

    std::vector<llama_token> tokens;
    const auto* vocab = llama_model_get_vocab(model);
    if (!prepare_prompt_tokens(env, chatParamsObj, model, ctx, vocab, tokens)) {
        return env->NewStringUTF("Error: template or tokenization failed");
    }

    std::string result = execute_generation(env, chatParamsObj, ctx, vocab, model, tokens, isEncoder);
    return env->NewStringUTF(result.c_str());
}

} // extern "C"