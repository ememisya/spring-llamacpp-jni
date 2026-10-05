package com.ememisya.llamacpp;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonValue;

/**
 * Represents the role of a chat message within a conversation.
 *
 * <p>
 * This enum preserves Java‑idiomatic uppercase constant names while providing
 * stable, lowercase JSON values that align with llama.cpp's expected role
 * identifiers ("system", "user", "assistant").</p>
 *
 * <p>
 * Serialization uses {@link #toJson()} to emit lowercase values, and
 * deserialization uses {@link #fromJson(String)} to accept case‑insensitive
 * input.</p>
 */
public enum ChatMessageRole {

    /**
     * System‑level instruction or context message.
     */
    SYSTEM("system"),
    /**
     * User‑authored message.
     */
    USER("user"),
    /**
     * Assistant‑generated message.
     */
    ASSISTANT("assistant");

    /**
     * Canonical JSON value for this role, always lowercase.
     */
    private final String jsonValue;

    /**
     * Constructs a role with its associated JSON representation.
     *
     * @param jsonValue the lowercase JSON value for this role
     */
    ChatMessageRole(final String jsonValue) {
        this.jsonValue = jsonValue;
    }

    /**
     * Serializes this enum to its lowercase JSON representation.
     *
     * @return the canonical lowercase JSON value
     */
    @JsonValue
    public String toJson() {
        return jsonValue;
    }

    /**
     * Deserializes a JSON value into a {@link ChatMessageRole}, accepting
     * case‑insensitive input and trimming whitespace.
     *
     * @param value the JSON string to parse
     * @return the matching {@link ChatMessageRole}
     * @throws IllegalArgumentException if the value does not match any role
     */
    @JsonCreator
    public static ChatMessageRole fromJson(final String value) {
        if (value == null) {
            return null;
        }
        final String normalized = value.trim().toLowerCase();
        for (ChatMessageRole role : values()) {
            if (role.jsonValue.equals(normalized)) {
                return role;
            }
        }
        throw new IllegalArgumentException("Unknown ChatMessageRole: " + value);
    }

}
