package com.ememisya.llamacpp.domain;

import com.ememisya.llamacpp.ChatMessageRole;
import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.EnumType;
import jakarta.persistence.Enumerated;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.Lob;
import jakarta.persistence.Table;

/**
 * Represents a single chat message belonging to a {@link ChatParams} instance.
 *
 * <p>
 * This entity stores the role and content of a message in a fully relational
 * structure, enabling efficient querying and persistence without relying on
 * JSON serialization. It is intentionally decoupled from Spring AI's
 * {@code Message} interface to avoid framework coupling and maintain a clean
 * domain model.</p>
 */
@Entity
@Table(name = "chat_message")
public class ChatMessage {

    /**
     * Primary key for the chat message.
     */
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    /**
     * The role of the message (SYSTEM, USER, ASSISTANT).
     *
     * <p>
     * Stored as a string enum for readability and portability across database
     * engines.</p>
     */
    @Enumerated(EnumType.STRING)
    @Column(nullable = false)
    private ChatMessageRole role;

    /**
     * The raw text content of the message.
     *
     * <p>
     * Limited to 4000 characters to avoid excessive row size while still
     * accommodating typical chat interactions.</p>
     */
    @Lob
    @Column(nullable = false)
    private String content;

    /**
     * Default constructor required by JPA.
     */
    public ChatMessage() {
    }

    /**
     * Constructs a new chat message entity.
     *
     * @param chatParams the owning {@link ChatParams} instance
     * @param role the message role
     * @param text the message content
     */
    public ChatMessage(
            final ChatParams chatParams,
            final ChatMessageRole role,
            final String text
    ) {
        this.role = role;
        this.content = text;
    }

    /**
     * Returns the primary key of this message.
     *
     * @return the message ID
     */
    public Long getId() {
        return id;
    }

    /**
     * Returns the message role as a string.
     *
     * @return the role name
     */
    public ChatMessageRole getRole() {
        return role;
    }

    /**
     * Sets the message role.
     *
     * @param role the role to assign
     */
    public void setRole(final ChatMessageRole role) {
        this.role = role;
    }

    /**
     * Sets the id.
     *
     * @param id the id to assign
     */
    public void setId(final Long id) {
        this.id = id;
    }

    /**
     * Returns the raw content of the message.
     *
     * @return the message content
     */
    public String getContent() {
        return content;
    }

    /**
     * Sets the raw content of the message.
     *
     * @param content the content to assign
     */
    public void setContent(final String content) {
        this.content = content;
    }
}
