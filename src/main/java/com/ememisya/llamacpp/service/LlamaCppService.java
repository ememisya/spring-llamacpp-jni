package com.ememisya.llamacpp.service;

import com.ememisya.llamacpp.domain.ChatParams;
import com.ememisya.llamacpp.jni.LlamaCpp;
import jakarta.annotation.PostConstruct;
import jakarta.annotation.PreDestroy;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.autoconfigure.condition.ConditionalOnProperty;
import org.springframework.stereotype.Service;

/**
 * Service responsible for managing the lifecycle of the llama.cpp backend,
 * including backend initialization, model loading, context creation, inference,
 * and cleanup. This class wraps the JNI bridge and exposes a safe,
 * Spring-managed interface for interacting with llama.cpp.
 *
 * <p>
 * Lifecycle:
 * <ul>
 * <li>{@link #initializeBackendAndModel()} is invoked automatically at
 * startup</li>
 * <li>{@link #shutdown()} is invoked automatically during application
 * shutdown</li>
 * </ul>
 */
@Service
@ConditionalOnProperty(name = "llama.enabled", havingValue = "true")
public class LlamaCppService {

    /**
     * JNI bridge instance for interacting with llama.cpp.
     */
    private LlamaCpp llama = new LlamaCpp();

    /**
     * Pointer to the loaded llama.cpp model.
     */
    private long modelPtr;

    /**
     * Pointer to the active llama.cpp context.
     */
    private long contextPtr;

    /**
     * Filesystem path to the GGUF model file.
     */
    @Value("${llama.model-path}")
    private String modelPath;

    /**
     * Number of GPU layers to offload (default: 999 = auto).
     */
    @Value("${llama.gpu-layers:999}")
    private int gpuLayers;

    /**
     * Default constructor.
     */
    public LlamaCppService() {
    }

    /**
     * Initializes the llama.cpp backend and loads the model. This method is
     * invoked automatically by Spring after bean construction.
     */
    @PostConstruct
    public void initializeBackendAndModel() {
        llama.initBackend();
        loadModel();
    }

    /**
     * Frees the currently loaded llama.cpp model, if one exists.
     *
     * <p>
     * Safe to call multiple times. After the model is released the internal
     * model pointer is reset to zero.
     * </p>
     */
    public void freeModel() {
        if (modelPtr != 0) {
            llama.freeModel(modelPtr);
            modelPtr = 0;
        }
    }

    /**
     * Loads the configured GGUF model if it has not already been loaded.
     *
     * <p>
     * The model path is read from the configured Spring property and loaded
     * through the JNI bridge. If a model is already loaded, this method does
     * nothing.
     * </p>
     */
    public void loadModel() {
        if (modelPtr == 0) {
            modelPtr = llama.loadModel(modelPath, gpuLayers);
        }
    }

    /**
     * Creates a new llama.cpp context if one does not already exist.
     *
     * @param contextSize number of tokens the context can hold (n_ctx)
     * @param batchSize batch size for token processing (n_batch)
     * @param uBatchSize micro-batch size (n_ubatch)
     */
    public void initializeContext(
            final int contextSize,
            final int batchSize,
            final int uBatchSize
    ) {
        if (contextPtr == 0) {
            contextPtr = llama.createContext(modelPtr, contextSize,
                    batchSize, uBatchSize);
        }
    }

    /**
     * Generates a chat response using the active llama.cpp context.
     *
     * @param params chat parameters including messages and sampler
     * configuration
     * @return generated text from llama.cpp
     * @throws IllegalStateException if the context has not been initialized
     */
    public String chat(final ChatParams params) {
        if (contextPtr == 0) {
            throw new IllegalStateException(
                    "Context not initialized, please try again...");
        }

        return llama.generateWithContext(modelPtr, contextPtr, params);
    }

    /**
     * Frees the active llama.cpp context if one exists. Safe to call multiple
     * times.
     */
    public void freeContext() {
        if (contextPtr != 0) {
            llama.freeContext(contextPtr);
            contextPtr = 0;
        }
    }

    /**
     * Shuts down the llama.cpp backend, freeing both the context and model.
     * This method is invoked automatically by Spring during application
     * shutdown.
     */
    @PreDestroy
    public void shutdown() {
        freeContext();

        freeModel();

        llama.freeBackend();
    }
}
