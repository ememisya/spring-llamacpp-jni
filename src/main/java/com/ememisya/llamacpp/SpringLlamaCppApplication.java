package com.ememisya.llamacpp;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

/**
 * Entry point for the Spring Boot application integrating the llama.cpp native
 * backend with a REST API and supporting infrastructure.
 *
 * <p>
 * This class bootstraps the Spring context, performs component scanning, and
 * starts the embedded server.</p>
 */
@SpringBootApplication
public class SpringLlamaCppApplication {

    /**
     * Launches the Spring Boot application.
     *
     * @param args command-line arguments passed to the application
     */
    public static void main(final String[] args) {
        NativeLibraryLoader.loadAll();
        SpringApplication.run(SpringLlamaCppApplication.class, args);
    }
}
