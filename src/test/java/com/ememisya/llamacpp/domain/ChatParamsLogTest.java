package com.ememisya.llamacpp.domain;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.time.LocalDateTime;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

/**
 * Unit tests for {@link ChatParamsLog}.
 */
class ChatParamsLogTest {

    /**
     * Default constructor required by Checkstyle.
     */
    ChatParamsLogTest() {
    }

    /**
     * Tests parameterized constructor initialization.
     */
    @Test
    @DisplayName("Should initialize ChatParamsLog using parameterized constructor")
    void testParameterizedConstructor() {
        LocalDateTime now = LocalDateTime.now();
        ChatParams params = new ChatParams();
        ChatParamsLog log = new ChatParamsLog(now, params);

        assertNull(log.getId());
        assertEquals(now, log.getTimestamp());
        assertEquals(params, log.getChatParams());
    }

    /**
     * Tests field mutations via setters.
     */
    @Test
    @DisplayName("Should mutate ChatParamsLog via setters")
    void testSettersAndGetters() {
        ChatParamsLog log = new ChatParamsLog();
        LocalDateTime time = LocalDateTime.of(2026, 1, 1, 12, 0);
        ChatParams params = new ChatParams();

        log.setTimestamp(time);
        log.setChatParams(params);

        assertEquals(time, log.getTimestamp());
        assertEquals(params, log.getChatParams());
    }
}