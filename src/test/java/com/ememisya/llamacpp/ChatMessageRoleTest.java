package com.ememisya.llamacpp;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertThrows;

/**
 * Unit tests for {@link ChatMessageRole}.
 */
class ChatMessageRoleTest {

    /**
     * Default constructor required by Checkstyle.
     */
    ChatMessageRoleTest() {
    }

    /**
     * Tests serialization to lowercase JSON string.
     */
    @Test
    @DisplayName("Should serialize to lowercase JSON string")
    void testToJson() {
        assertEquals("system", ChatMessageRole.SYSTEM.toJson());
        assertEquals("user", ChatMessageRole.USER.toJson());
        assertEquals("assistant", ChatMessageRole.ASSISTANT.toJson());
    }

    /**
     * Tests deserialization with valid inputs.
     *
     * @param input valid JSON role representation
     */
    @ParameterizedTest
    @ValueSource(strings = {"user", "USER", " User ", "uSeR"})
    @DisplayName("Should deserialize valid values case-insensitively")
    void testFromJsonValid(final String input) {
        assertEquals(ChatMessageRole.USER, ChatMessageRole.fromJson(input));
    }

    /**
     * Tests deserialization returns null when value is null.
     */
    @Test
    @DisplayName("Should return null when deserializing null")
    void testFromJsonNull() {
        assertNull(ChatMessageRole.fromJson(null));
    }

    /**
     * Tests deserialization throws exception on invalid value.
     */
    @Test
    @DisplayName("Should throw IllegalArgumentException on unknown role")
    void testFromJsonInvalid() {
        assertThrows(IllegalArgumentException.class, () -> ChatMessageRole.fromJson("invalid_role"));
    }
}
