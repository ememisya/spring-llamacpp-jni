package com.ememisya.llamacpp.domain;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.OneToOne;
import jakarta.persistence.Table;
import java.time.LocalDateTime;

/**
 * Represents a persisted log entry capturing the parameters used for a single
 * chat invocation, including sampler settings, context configuration, and
 * associated chat messages.
 *
 * <p>
 * This entity replaces the previous JSON-based storage mechanism with a fully
 * relational structure via {@link ChatParams}, enabling structured queries and
 * improved maintainability.</p>
 */
@Entity
@Table(name = "chat_params_log")
public class ChatParamsLog {

    /**
     * Primary key for the log entry.
     */
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    /**
     * Timestamp indicating when this log entry was created.
     */
    private LocalDateTime timestamp;

    /**
     * The full set of chat parameters used for this invocation, including
     * sampler configuration and the list of chat messages.
     */
    @OneToOne
    private ChatParams chatParams;

    /**
     * Default constructor required by JPA.
     */
    public ChatParamsLog() {
    }

    /**
     * Constructs a new log entry with the given timestamp and chat parameters.
     *
     * @param timestamp the creation timestamp of this log entry
     * @param chatParams the chat parameters associated with this invocation
     */
    public ChatParamsLog(final LocalDateTime timestamp, final ChatParams chatParams) {
        this.timestamp = timestamp;
        this.chatParams = chatParams;
    }

    /**
     * Returns the primary key of this log entry.
     *
     * @return the log entry ID
     */
    public Long getId() {
        return id;
    }

    /**
     * Returns the timestamp when this log entry was created.
     *
     * @return the timestamp
     */
    public LocalDateTime getTimestamp() {
        return timestamp;
    }

    /**
     * Sets the timestamp for this log entry.
     *
     * @param timestamp the timestamp to assign
     */
    public void setTimestamp(final LocalDateTime timestamp) {
        this.timestamp = timestamp;
    }

    /**
     * Returns the chat parameters associated with this log entry.
     *
     * @return the chat parameters
     */
    public ChatParams getChatParams() {
        return chatParams;
    }

    /**
     * Sets the chat parameters for this log entry.
     *
     * @param chatParams the chat parameters to assign
     */
    public void setChatParams(final ChatParams chatParams) {
        this.chatParams = chatParams;
    }
}
