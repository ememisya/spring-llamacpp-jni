package com.ememisya.llamacpp.domain;

import com.ememisya.llamacpp.ChatMessageRole;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;

/**
 * Unit tests for {@link ChatParams}.
 */
class ChatParamsTest {

    /**
     * Default constructor required by Checkstyle.
     */
    ChatParamsTest() {
    }

    /**
     * Tests default field initialization values.
     */
    @Test
    @DisplayName("Should initialize ChatParams with default values")
    void testDefaultValues() {
        ChatParams params = new ChatParams();

        assertNotNull(params.getMessages());
        assertEquals(18432, params.getContextSize());
        assertEquals(4096, params.getBatchSize());
        assertEquals(512, params.getuBatchSize());
        assertEquals(0.8023f, params.getTemperature(), 0.0001f);
        assertEquals(0.05f, params.getMinP(), 0.0001f);
        assertEquals(1.0543f, params.getRepeatPenalty(), 0.0001f);
        assertEquals(0.05f, params.getFreqPenalty(), 0.0001f);
        assertEquals(0.1465f, params.getDynamicTemperature(), 0.0001f);
        assertEquals(0.023f, params.getPresencePenalty(), 0.0001f);
        assertEquals(64, params.getPenaltyLastN());
        assertEquals(0.523f, params.getDryMultiplier(), 0.0001f);
        assertEquals(1.75f, params.getDryBase(), 0.0001f);
        assertEquals(2, params.getDryAllowedLength());
        assertEquals(512, params.getDryPenaltyLastN());
        assertEquals(42, params.getRngSeed());
    }

    /**
     * Tests field mutations via setters.
     */
    @Test
    @DisplayName("Should modify ChatParams fields via setters")
    void testSettersAndGetters() {
        ChatParams params = new ChatParams();
        params.setScratchPad("Draft content");
        params.setContextSize(2048);
        params.setBatchSize(256);
        params.setuBatchSize(64);
        params.setTemperature(0.7f);
        params.setMinP(0.1f);
        params.setRepeatPenalty(1.1f);
        params.setFreqPenalty(0.01f);
        params.setDynamicTemperature(0.2f);
        params.setPresencePenalty(0.05f);
        params.setPenaltyLastN(32);
        params.setDryMultiplier(0.4f);
        params.setDryBase(1.5f);
        params.setDryAllowedLength(4);
        params.setDryPenaltyLastN(256);
        params.setRngSeed(12345);

        ChatMessage msg = new ChatMessage(params, ChatMessageRole.USER, "Test");
        params.setMessages(List.of(msg));

        assertEquals("Draft content", params.getScratchPad());
        assertEquals(2048, params.getContextSize());
        assertEquals(256, params.getBatchSize());
        assertEquals(64, params.getuBatchSize());
        assertEquals(0.7f, params.getTemperature(), 0.0001f);
        assertEquals(0.1f, params.getMinP(), 0.0001f);
        assertEquals(1.1f, params.getRepeatPenalty(), 0.0001f);
        assertEquals(0.01f, params.getFreqPenalty(), 0.0001f);
        assertEquals(0.2f, params.getDynamicTemperature(), 0.0001f);
        assertEquals(0.05f, params.getPresencePenalty(), 0.0001f);
        assertEquals(32, params.getPenaltyLastN());
        assertEquals(0.4f, params.getDryMultiplier(), 0.0001f);
        assertEquals(1.5f, params.getDryBase(), 0.0001f);
        assertEquals(4, params.getDryAllowedLength());
        assertEquals(256, params.getDryPenaltyLastN());
        assertEquals(12345, params.getRngSeed());
        assertEquals(1, params.getMessages().size());
    }
}