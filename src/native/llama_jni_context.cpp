/**
 * @file llama_jni_context.cpp
 *
 * Implementation of context‑related helper functions for the JNI llama.cpp
 * integration layer. These functions handle:
 *
 *  - Extracting primitive values from Java objects
 *  - Creating llama_context instances with user‑provided parameters
 *
 * All declarations are defined in llama_jni_internal.hpp.
 */

#include "llama_jni_internal.hpp"

/**
 * Retrieve an integer value from a Java object by invoking a getter method.
 *
 * @param env    JNI environment pointer
 * @param obj    Java object containing the getter
 * @param getter Name of the getter method (e.g., "getBatchSize")
 * @return       The integer returned by the getter, or 0 on failure
 */
int get_int_field(JNIEnv* env, jobject obj, const char* getter) {
    jclass cls = env->GetObjectClass(obj);
    if (!cls) {
        LOG("get_int_field: failed to obtain object class");
        return 0;
    }

    jmethodID mid = env->GetMethodID(cls, getter, "()I");
    if (!mid) {
        LOG("get_int_field: failed to find getter %s", getter);
        return 0;
    }

    return env->CallIntMethod(obj, mid);
}

/**
 * Retrieve a float value from a Java object by invoking a getter method.
 *
 * @param env    JNI environment pointer
 * @param obj    Java object containing the getter
 * @param getter Name of the getter method (e.g., "getTemperature")
 * @return       The float returned by the getter, or 0.0f on failure
 */
float get_float_field(JNIEnv* env, jobject obj, const char* getter) {
    jclass cls = env->GetObjectClass(obj);
    if (!cls) {
        LOG("get_float_field: failed to obtain object class");
        return 0.0f;
    }

    jmethodID mid = env->GetMethodID(cls, getter, "()F");
    if (!mid) {
        LOG("get_float_field: failed to find getter %s", getter);
        return 0.0f;
    }

    return env->CallFloatMethod(obj, mid);
}

/**
 * Create a llama_context using the provided model and configuration values.
 *
 * @param model       Pointer to an already-loaded llama_model
 * @param contextSize Maximum context length (n_ctx)
 * @param batchSize   Batch size for decoding (n_batch)
 * @param uBatchSize  Micro-batch size (n_ubatch)
 * @return            Newly created llama_context, or nullptr on failure
 */
llama_context* create_context(
        llama_model* model,
        int contextSize,
        int batchSize,
        int uBatchSize) {

    llama_context_params params = llama_context_default_params();
    params.n_ctx   = static_cast<uint32_t>(contextSize);
    params.n_batch = static_cast<uint32_t>(batchSize);
    params.n_ubatch = static_cast<uint32_t>(uBatchSize);

    LOG("create_context: n_ctx=%d, n_batch=%d, n_ubatch=%d",
        contextSize, batchSize, uBatchSize);

    llama_context* ctx = llama_init_from_model(model, params);

    if (!ctx) {
        LOG("create_context: FAILED to create llama_context");
    }

    return ctx;
}
