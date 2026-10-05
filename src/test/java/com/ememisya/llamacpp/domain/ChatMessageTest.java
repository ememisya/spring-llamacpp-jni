package com.ememisya.llamacpp.domain;

import com.ememisya.llamacpp.ChatMessageRole;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

/**
 * Unit tests for {@link ChatMessage}.
 */
class ChatMessageTest {

    /**
     * Default constructor required by Checkstyle.
     */
    ChatMessageTest() {
    }

    /**
     * Tests no-arg constructor and field accessors.
     */
    @Test
    @DisplayName("Should initialize ChatMessage using no-arg constructor and setters")
    void testNoArgConstructorAndSetters() {
        ChatMessage message = new ChatMessage();
        message.setId(1L);
        message.setRole(ChatMessageRole.USER);
        message.setContent("Hello World");

        assertEquals(1L, message.getId());
        assertEquals(ChatMessageRole.USER, message.getRole());
        assertEquals("Hello World", message.getContent());
    }

    /**
     * Tests parameterized constructor initialization.
     */
    @Test
    @DisplayName("Should initialize ChatMessage using parameterized constructor")
    void testParameterizedConstructor() {
        ChatParams params = new ChatParams();
        ChatMessage message = new ChatMessage(params, ChatMessageRole.SYSTEM, "You are a helpful assistant.");

        assertNull(message.getId());
        assertEquals(ChatMessageRole.SYSTEM, message.getRole());
        assertEquals("You are a helpful assistant.", message.getContent());
    }
}