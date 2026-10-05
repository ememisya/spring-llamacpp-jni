package com.ememisya.llamacpp.config;

import com.fasterxml.jackson.datatype.jsr310.JavaTimeModule;
import org.springframework.boot.autoconfigure.jackson.Jackson2ObjectMapperBuilderCustomizer;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

/**
 * Spring configuration class responsible for customizing all Jackson
 * ObjectMapper instances created by Spring Boot.
 *
 * <p>
 * This ensures that:
 * <ul>
 * <li>Java 8 time types (e.g., LocalDateTime) are fully supported</li>
 * <li>Every ObjectMapper in the application (global or custom) receives
 * module</li>
 * </ul>
 */
@Configuration
public class DBConfig {

    /**
     * Registers the JavaTimeModule and custom Message serializer/deserializer
     * with every ObjectMapper created by Spring Boot.
     *
     * @return a Jackson2ObjectMapperBuilderCustomizer that installs the modules
     */
    @Bean
    public Jackson2ObjectMapperBuilderCustomizer addJavaTimeModuleToAllMappers() {

        return builder -> builder.modulesToInstall(JavaTimeModule.class);
    }
}
