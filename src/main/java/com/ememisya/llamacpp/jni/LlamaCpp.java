package com.ememisya.llamacpp.jni;

import com.ememisya.llamacpp.domain.ChatParams;

/**
 * JNI bridge for interacting with the native llama.cpp library.
 *
 * <p>
 * This class exposes low-level native methods for:
 * <ul>
 * <li>Backend initialization and shutdown</li>
 * <li>Model loading and unloading</li>
 * <li>Context creation and destruction</li>
 * <li>Stateless and stateful text generation</li>
 * </ul>
 *
 * <p>
 * All heavy lifting is performed in native code. This class contains no
 * business logic and is intentionally lightweight and stateless.</p>
 */
public class LlamaCpp {

    /**
     * Sets the process DLL search directory on Microsoft Windows.
     *
     * <p>
     * This method is implemented in native code and is used to ensure that
     * dependent native libraries can be resolved from the extracted library
     * directory. On non-Windows platforms this method is not required.
     * </p>
     *
     * @param path absolute path containing native library dependencies
     */
    public static native void setDllDirectory(final String path);

    /**
     * Initializes the llama.cpp backend (e.g., GPU initialization, threading).
     * Must be called before loading a model.
     */
    public native void initBackend();

    /**
     * Frees backend resources allocated by llama.cpp. Should be called once
     * during application shutdown.
     */
    public native void freeBackend();

    /**
     * Loads a GGUF model file into memory.
     *
     * @param modelPath filesystem path to the GGUF model
     * @param gpuLayers number of layers to offload to GPU (0 = CPU only)
     * @return pointer to the loaded model
     */
    public native long loadModel(final String modelPath, final int gpuLayers);

    /**
     * Frees a previously loaded model.
     *
     * @param modelPtr pointer returned by {@link #loadModel(String, int)}
     */
    public native void freeModel(final long modelPtr);

    /**
     * Performs stateless text generation. A temporary context is created
     * internally for this call.
     *
     * @param modelPtr pointer to the loaded model
     * @param params chat parameters including messages and sampler settings
     * @return generated text
     */
    public native String generate(final long modelPtr, final ChatParams params);

    /**
     * Performs stateful text generation using an existing llama_context.
     *
     * @param modelPtr pointer to the loaded model
     * @param ctxPtr pointer to an existing llama_context
     * @param params chat parameters including messages and sampler settings
     * @return generated text
     */
    public native String generateWithContext(
            final long modelPtr,
            final long ctxPtr,
            final ChatParams params
    );

    /**
     * Creates a new llama.cpp context for stateful generation.
     *
     * @param modelPtr pointer to the loaded model
     * @param contextSize context size (n_ctx)
     * @param batchSize batch size (n_batch)
     * @param uBatchSize micro-batch size (n_ubatch)
     * @return pointer to the created context
     */
    public native long createContext(
            final long modelPtr,
            final int contextSize,
            final int batchSize,
            final int uBatchSize
    );

    /**
     * Frees a previously created llama.cpp context.
     *
     * @param ctxPtr pointer returned by
     * {@link #createContext(long, int, int, int)}
     */
    public native void freeContext(final long ctxPtr);
}
