package com.ememisya.llamacpp.domain;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.FetchType;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.Lob;
import jakarta.persistence.OneToMany;
import jakarta.persistence.Table;
import java.util.ArrayList;
import java.util.List;

/**
 * Entity representing all chat parameters passed into the llama.cpp JNI bridge.
 *
 * <p>
 * This includes:
 * <ul>
 * <li>Ordered chat messages (system, user, assistant)</li>
 * <li>Context and batch configuration</li>
 * <li>Sampler configuration (temperature, penalties, DRY, RNG seed, etc.)</li>
 * </ul>
 *
 * <p>
 * This entity is stored relationally instead of as a JSON CLOB, allowing full
 * queryability and normalization of message and sampler data.
 */
@Entity
@Table(name = "chat_params")
public class ChatParams {

    /**
     * Primary key for this entity.
     */
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    /**
     * Ordered list of chat messages associated with this ChatParams instance.
     */
    @OneToMany(fetch = FetchType.EAGER)
    private List<ChatMessage> messages = new ArrayList<>();

    /**
     * The raw text content of the scratch pad.
     */
    @Lob
    @Column(nullable = false)
    private String scratchPad;

    /**
     * Llama.cpp context size (n_ctx).
     */
    @Column(nullable = false)
    private int contextSize = 18432;

    /**
     * Llama.cpp batch size (n_batch).
     */
    @Column(nullable = false)
    private int batchSize = 4096;

    /**
     * Llama.cpp micro-batch size (n_ubatch).
     */
    @Column(nullable = false)
    private int uBatchSize = 512;

    /**
     * Temperature for sampling (higher = more random).
     */
    private float temperature = 0.8023f;

    /**
     * Minimum probability threshold for min-p sampling.
     */
    private float minP = 0.05f;

    /**
     * Repetition penalty strength.
     */
    private float repeatPenalty = 1.0543f;

    /**
     * Frequency penalty strength.
     */
    private float freqPenalty = 0.05f;

    /**
     * Dynamic temperature scaling factor.
     */
    private float dynamicTemperature = 0.1465f;

    /**
     * Presence penalty strength.
     */
    private float presencePenalty = 0.023f;

    /**
     * Number of recent tokens considered for penalties.
     */
    private int penaltyLastN = 64;

    /**
     * DRY sampler multiplier.
     */
    private float dryMultiplier = 0.523f;

    /**
     * DRY sampler base value.
     */
    private float dryBase = 1.75f;

    /**
     * DRY sampler allowed length.
     */
    private int dryAllowedLength = 2;

    /**
     * DRY sampler penalty window.
     */
    private int dryPenaltyLastN = 512;

    /**
     * RNG seed for deterministic sampling.
     */
    private int rngSeed = 42;

    /**
     * Default constructor.
     */
    public ChatParams() {
    }

    /**
     * Returns the database identifier.
     *
     * @return entity identifier
     */
    public Long getId() {
        return id;
    }

    /**
     * Returns the ordered list of chat messages.
     *
     * @return chat messages
     */
    public List<ChatMessage> getMessages() {
        return messages;
    }

    /**
     * Sets the list of chat messages associated with this ChatParams instance.
     *
     * @param messages list of ChatMessage objects
     */
    public void setMessages(final List<ChatMessage> messages) {
        this.messages = messages;
    }

    /**
     * Returns the llama.cpp context size (n_ctx).
     *
     * @return context size
     */
    public int getContextSize() {
        return contextSize;
    }

    /**
     * Returns the scratch pad contents.
     *
     * @return scratch pad contents
     */
    public String getScratchPad() {
        return scratchPad;
    }

    /**
     * Sets the scratchPad contents.
     *
     * @param scratchPad contents of the scratch pad
     */
    public void setScratchPad(final String scratchPad) {
        this.scratchPad = scratchPad;
    }

    /**
     * Sets the llama.cpp context size (n_ctx).
     *
     * @param contextSize number of tokens the model can hold
     */
    public void setContextSize(final int contextSize) {
        this.contextSize = contextSize;
    }

    /**
     * Returns the llama.cpp batch size (n_batch).
     *
     * @return batch size
     */
    public int getBatchSize() {
        return batchSize;
    }

    /**
     * Sets the llama.cpp batch size (n_batch).
     *
     * @param batchSize number of tokens processed per batch
     */
    public void setBatchSize(final int batchSize) {
        this.batchSize = batchSize;
    }

    /**
     * Returns the llama.cpp micro-batch size (n_ubatch).
     *
     * @return micro-batch size
     */
    public int getuBatchSize() {
        return uBatchSize;
    }

    /**
     * Sets the llama.cpp micro-batch size (n_ubatch).
     *
     * @param uBatchSize micro-batch size
     */
    public void setuBatchSize(final int uBatchSize) {
        this.uBatchSize = uBatchSize;
    }

    /**
     * Returns the sampling temperature.
     *
     * @return temperature value
     */
    public float getTemperature() {
        return temperature;
    }

    /**
     * Sets the sampling temperature.
     *
     * @param temperature randomness factor
     */
    public void setTemperature(final float temperature) {
        this.temperature = temperature;
    }

    /**
     * Returns the min-p threshold.
     *
     * @return min-p value
     */
    public float getMinP() {
        return minP;
    }

    /**
     * Sets the min-p threshold for sampling.
     *
     * @param minP minimum probability threshold
     */
    public void setMinP(final float minP) {
        this.minP = minP;
    }

    /**
     * Returns the repetition penalty strength.
     *
     * @return repetition penalty
     */
    public float getRepeatPenalty() {
        return repeatPenalty;
    }

    /**
     * Sets the repetition penalty strength.
     *
     * @param repeatPenalty penalty value
     */
    public void setRepeatPenalty(final float repeatPenalty) {
        this.repeatPenalty = repeatPenalty;
    }

    /**
     * Returns the frequency penalty strength.
     *
     * @return freqPenalty
     */
    public float getFreqPenalty() {
        return freqPenalty;
    }

    /**
     * Sets the frequency penalty strength.
     *
     * @param freqPenalty penalty value
     */
    public void setFreqPenalty(final float freqPenalty) {
        this.freqPenalty = freqPenalty;
    }

    /**
     * Returns the dynamic temperature modifier.
     *
     * @return dynamicTemperature
     */
    public float getDynamicTemperature() {
        return dynamicTemperature;
    }

    /**
     * Sets the dynamic temperature scaling factor.
     *
     * @param dynamicTemperature dynamic temperature value
     */
    public void setDynamicTemperature(final float dynamicTemperature) {
        this.dynamicTemperature = dynamicTemperature;
    }

    /**
     * Returns the presence penalty.
     *
     * @return presencePenalty
     */
    public float getPresencePenalty() {
        return presencePenalty;
    }

    /**
     * Sets the presence penalty strength.
     *
     * @param presencePenalty penalty value
     */
    public void setPresencePenalty(final float presencePenalty) {
        this.presencePenalty = presencePenalty;
    }

    /**
     * Returns the penalty last counter.
     *
     * @return penaltyLastN
     */
    public int getPenaltyLastN() {
        return penaltyLastN;
    }

    /**
     * Sets how many recent tokens are considered for penalties.
     *
     * @param penaltyLastN number of tokens
     */
    public void setPenaltyLastN(final int penaltyLastN) {
        this.penaltyLastN = penaltyLastN;
    }

    /**
     * Returns the dryMultiplier.
     *
     * @return dryMultiplier
     */
    public float getDryMultiplier() {
        return dryMultiplier;
    }

    /**
     * Sets the DRY sampler multiplier.
     *
     * @param dryMultiplier multiplier value
     */
    public void setDryMultiplier(final float dryMultiplier) {
        this.dryMultiplier = dryMultiplier;
    }

    /**
     * Returns the dryBase.
     *
     * @return dryBase
     */
    public float getDryBase() {
        return dryBase;
    }

    /**
     * Sets the DRY sampler base value.
     *
     * @param dryBase base value
     */
    public void setDryBase(final float dryBase) {
        this.dryBase = dryBase;
    }

    /**
     * Returns the dryAllowedLength.
     *
     * @return dryAllowedLength
     */
    public int getDryAllowedLength() {
        return dryAllowedLength;
    }

    /**
     * Sets the DRY sampler allowed length.
     *
     * @param dryAllowedLength allowed length
     */
    public void setDryAllowedLength(final int dryAllowedLength) {
        this.dryAllowedLength = dryAllowedLength;
    }

    /**
     * Returns the dryPenaltyLastN.
     *
     * @return dryPenaltyLastN
     */
    public int getDryPenaltyLastN() {
        return dryPenaltyLastN;
    }

    /**
     * Sets the DRY sampler penalty window.
     *
     * @param dryPenaltyLastN penalty window
     */
    public void setDryPenaltyLastN(final int dryPenaltyLastN) {
        this.dryPenaltyLastN = dryPenaltyLastN;
    }

    /**
     * Returns the rngSeed.
     *
     * @return rngSeed
     */
    public int getRngSeed() {
        return rngSeed;
    }

    /**
     * Sets the RNG seed for deterministic sampling.
     *
     * @param rngSeed seed value
     */
    public void setRngSeed(final int rngSeed) {
        this.rngSeed = rngSeed;
    }
}
