package com.ememisya.llamacpp.model;

import java.time.LocalDateTime;

/**
 * Projection representing a persisted chat state.
 */
public interface ChatParamsLogIdentifierModel {

    /**
     * Returns the identifier.
     *
     * @return identifier
     */
    Long getId();

    /**
     * Returns the timestamp associated with the state.
     *
     * @return state timestamp
     */
    LocalDateTime getTimestamp();
}