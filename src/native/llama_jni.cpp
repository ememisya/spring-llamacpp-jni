/**
 * @file llama_jni.cpp
 *
 * JNI entry points for the llama.cpp integration layer.
 *
 * This file exposes the native methods used by the Java class
 * com.ememisya.llamacpp.jni.LlamaCpp. All heavy lifting is delegated to
 * helper modules:
 *
 *  - llama_jni_context.cpp
 *  - llama_jni_sampler.cpp
 *  - llama_jni_messages.cpp
 *  - llama_jni_generate.cpp
 *
 * The goal is to keep this file clean, minimal, and easy to understand.
 */

#include "llama_jni_internal.hpp"


extern "C" {

/**
 * Load a llama model from a file path.
 *
 * @param env        JNI environment
 * @param obj        Java LlamaCpp instance
 * @param modelPath  Path to the GGUF model file
 * @param gpuLayers  Number of layers to offload to GPU
 *
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
 * @param env      JNI environment
 * @param obj      Java LlamaCpp instance
 * @param modelPtr Pointer to llama_model
 */
JNIEXPORT void JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_freeModel(
        JNIEnv* env,
        jobject obj,
        jlong modelPtr) {

    llama_model* model = reinterpret_cast<llama_model*>(modelPtr);
    if (model) {
        llama_model_free(model);
    }
}

/**
 * Initialize the llama backend.
 *
 * Must be called before loading any models.
 */
JNIEXPORT void JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_initBackend(
        JNIEnv* env,
        jobject obj) {

    llama_backend_init();
}

/**
 * Free the llama backend.
 *
 * Should be called when shutting down the application.
 */
JNIEXPORT void JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_freeBackend(
        JNIEnv* env,
        jobject obj) {

    llama_backend_free();
}

/**
 * Create a reusable llama_context.
 *
 * @param env        JNI environment
 * @param obj        Java LlamaCpp instance
 * @param modelPtr   Pointer to llama_model
 * @param ctxSize    Context length (n_ctx)
 * @param batchSize  Batch size (n_batch)
 * @param uBatchSize Micro-batch size (n_ubatch)
 *
 * @return Pointer to llama_context as a jlong
 */
JNIEXPORT jlong JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_createContext(
        JNIEnv* env,
        jobject obj,
        jlong modelPtr,
        jint ctxSize,
        jint batchSize,
        jint uBatchSize) {

    llama_model* model = reinterpret_cast<llama_model*>(modelPtr);
    if (!model) {
        return 0;
    }

    llama_context* ctx =
        create_context(model, ctxSize, batchSize, uBatchSize);

    return reinterpret_cast<jlong>(ctx);
}

/**
 * Free a previously created llama_context.
 *
 * @param env     JNI environment
 * @param obj     Java LlamaCpp instance
 * @param ctxPtr  Pointer to llama_context
 */
JNIEXPORT void JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_freeContext(
        JNIEnv* env,
        jobject obj,
        jlong ctxPtr) {

    llama_context* ctx = reinterpret_cast<llama_context*>(ctxPtr);
    if (ctx) {
        llama_free(ctx);
    }
}

/**
 * Generate text using a fresh llama_context.
 *
 * This is the stateless generation path.
 *
 * @param env            JNI environment
 * @param obj            Java LlamaCpp instance
 * @param modelPtr       Pointer to llama_model
 * @param chatParamsObj  Java ChatParams instance
 *
 * @return Generated text as a Java string
 */
JNIEXPORT jstring JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_generate(
        JNIEnv* env,
        jobject obj,
        jlong modelPtr,
        jobject chatParamsObj) {

    llama_model* model = reinterpret_cast<llama_model*>(modelPtr);
    if (!model) {
        return env->NewStringUTF("Error: null model");
    }

    jclass cls = env->GetObjectClass(chatParamsObj);

    int ctxSize =
        env->CallIntMethod(chatParamsObj,
            env->GetMethodID(cls, "getContextSize", "()I"));

    int batchSize =
        env->CallIntMethod(chatParamsObj,
            env->GetMethodID(cls, "getBatchSize", "()I"));

    int uBatchSize =
        env->CallIntMethod(chatParamsObj,
            env->GetMethodID(cls, "getuBatchSize", "()I"));

    llama_context* ctx =
        create_context(model, ctxSize, batchSize, uBatchSize);

    if (!ctx) {
        return env->NewStringUTF("Error: context creation failed");
    }

    const llama_vocab* vocab = llama_model_get_vocab(model);

    float temperature       = get_float_field(env, chatParamsObj, "getTemperature");
    float dynamicTemperature = get_float_field(env, chatParamsObj, "getDynamicTemperature");
    float minP              = get_float_field(env, chatParamsObj, "getMinP");
    float repeatPenalty     = get_float_field(env, chatParamsObj, "getRepeatPenalty");
    float freqPenalty       = get_float_field(env, chatParamsObj, "getFreqPenalty");
    float presencePenalty   = get_float_field(env, chatParamsObj, "getPresencePenalty");
    int   penaltyLastN      = get_int_field(env, chatParamsObj, "getPenaltyLastN");
    float dryMultiplier     = get_float_field(env, chatParamsObj, "getDryMultiplier");
    float dryBase           = get_float_field(env, chatParamsObj, "getDryBase");
    int   dryAllowedLength  = get_int_field(env, chatParamsObj, "getDryAllowedLength");
    int   dryPenaltyLastN   = get_int_field(env, chatParamsObj, "getDryPenaltyLastN");
    int   rngSeed           = get_int_field(env, chatParamsObj, "getRngSeed");

    llama_sampler* sampler = create_sampler(
        vocab,
        model,
        temperature,
        dynamicTemperature,
        minP,
        penaltyLastN,
        repeatPenalty,
        freqPenalty,
        presencePenalty,
        dryMultiplier,
        dryBase,
        dryAllowedLength,
        dryPenaltyLastN,
        rngSeed
    );

    auto msgs = extract_messages(env, chatParamsObj);
    if (msgs.empty()) {
        llama_sampler_free(sampler);
        llama_free(ctx);
        return env->NewStringUTF("Error: no messages");
    }

    std::string prompt = apply_chat_template(model, ctx, msgs);
    if (prompt.empty()) {
        free_messages(msgs);
        llama_sampler_free(sampler);
        llama_free(ctx);
        return env->NewStringUTF("Error: template failed");
    }

    auto tokens = tokenize_prompt(vocab, ctx, prompt);
    if (tokens.empty()) {
        free_messages(msgs);
        llama_sampler_free(sampler);
        llama_free(ctx);
        return env->NewStringUTF("Error: tokenize failed");
    }

    std::string result =
        run_generation_loop(ctx, sampler, vocab, tokens);

    free_messages(msgs);
    llama_sampler_free(sampler);
    llama_free(ctx);

    return env->NewStringUTF(result.c_str());
}

/**
 * Generate text using an existing llama_context.
 *
 * This is the stateful generation path.
 *
 * @param env            JNI environment
 * @param obj            Java LlamaCpp instance
 * @param modelPtr       Pointer to llama_model
 * @param ctxPtr         Pointer to llama_context
 * @param chatParamsObj  Java ChatParams instance
 *
 * @return Generated text as a Java string
 */
JNIEXPORT jstring JNICALL
Java_com_ememisya_llamacpp_jni_LlamaCpp_generateWithContext(
        JNIEnv* env,
        jobject obj,
        jlong modelPtr,
        jlong ctxPtr,
        jobject chatParamsObj) {

    llama_model* model = reinterpret_cast<llama_model*>(modelPtr);
    llama_context* ctx = reinterpret_cast<llama_context*>(ctxPtr);

    if (!model || !ctx) {
        return env->NewStringUTF("Error: null model or context");
    }

    const llama_vocab* vocab = llama_model_get_vocab(model);

    float temperature       = get_float_field(env, chatParamsObj, "getTemperature");
    float dynamicTemperature = get_float_field(env, chatParamsObj, "getDynamicTemperature");
    float minP              = get_float_field(env, chatParamsObj, "getMinP");
    float repeatPenalty     = get_float_field(env, chatParamsObj, "getRepeatPenalty");
    float freqPenalty       = get_float_field(env, chatParamsObj, "getFreqPenalty");
    float presencePenalty   = get_float_field(env, chatParamsObj, "getPresencePenalty");
    int   penaltyLastN      = get_int_field(env, chatParamsObj, "getPenaltyLastN");
    float dryMultiplier     = get_float_field(env, chatParamsObj, "getDryMultiplier");
    float dryBase           = get_float_field(env, chatParamsObj, "getDryBase");
    int   dryAllowedLength  = get_int_field(env, chatParamsObj, "getDryAllowedLength");
    int   dryPenaltyLastN   = get_int_field(env, chatParamsObj, "getDryPenaltyLastN");
    int   rngSeed           = get_int_field(env, chatParamsObj, "getRngSeed");

    llama_sampler* sampler = create_sampler(
        vocab,
        model,
        temperature,
        dynamicTemperature,
        minP,
        penaltyLastN,
        repeatPenalty,
        freqPenalty,
        presencePenalty,
        dryMultiplier,
        dryBase,
        dryAllowedLength,
        dryPenaltyLastN,
        rngSeed
    );

    auto msgs = extract_messages(env, chatParamsObj);
    if (msgs.empty()) {
        llama_sampler_free(sampler);
        return env->NewStringUTF("Error: no messages");
    }

    std::string prompt = apply_chat_template(model, ctx, msgs);
    if (prompt.empty()) {
        free_messages(msgs);
        llama_sampler_free(sampler);
        return env->NewStringUTF("Error: template failed");
    }

    auto tokens = tokenize_prompt(vocab, ctx, prompt);
    if (tokens.empty()) {
        free_messages(msgs);
        llama_sampler_free(sampler);
        return env->NewStringUTF("Error: tokenize failed");
    }

    std::string result =
        run_generation_loop(ctx, sampler, vocab, tokens);

    free_messages(msgs);
    llama_sampler_free(sampler);

    return env->NewStringUTF(result.c_str());
}

} // extern "C"
