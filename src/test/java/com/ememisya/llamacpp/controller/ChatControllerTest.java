package com.ememisya.llamacpp.controller;

import com.ememisya.llamacpp.ChatMessageRole;
import com.ememisya.llamacpp.domain.ChatMessage;
import com.ememisya.llamacpp.domain.ChatParams;
import com.ememisya.llamacpp.domain.ChatParamsLog;
import com.ememisya.llamacpp.repository.ChatMessageRepository;
import com.ememisya.llamacpp.repository.ChatParamsLogRepository;
import com.ememisya.llamacpp.repository.ChatParamsRepository;
import com.ememisya.llamacpp.service.LlamaCppService;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;

import java.time.LocalDateTime;
import java.util.Optional;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import org.springframework.test.context.bean.override.mockito.MockitoBean;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.delete;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.content;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

/**
 * MockMvc integration tests for {@link ChatController}.
 */
@WebMvcTest(ChatController.class)
class ChatControllerTest {

    /**
     * MockMvc instance for performing HTTP requests.
     */
    @Autowired
    private MockMvc mockMvc;

    /**
     * Jackson Object Mapper for serializing and deserializing payloads.
     */
    @Autowired
    private ObjectMapper objectMapper;

    /**
     * Mocked repository for {@link ChatParamsLog} entities.
     */
    @MockitoBean
    private ChatParamsLogRepository chatParamsLogRepository;

    /**
     * Mocked repository for {@link ChatParams} entities.
     */
    @MockitoBean
    private ChatParamsRepository chatParamsRepository;

    /**
     * Mocked repository for {@link ChatMessage} entities.
     */
    @MockitoBean
    private ChatMessageRepository chatMessageRepository;

    /**
     * Mocked service for interacting with native llama.cpp bindings.
     */
    @MockitoBean
    private LlamaCppService llamaCppService;

    /**
     * Default constructor required by Checkstyle.
     */
    ChatControllerTest() {
    }

    /**
     * Tests successful retrieval of the last log entry.
     *
     * @throws Exception if perform operation fails
     */
    @Test
    @DisplayName("GET /v1/chat/last - Success")
    void testGetLastEntrySuccess() throws Exception {
        ChatParams params = new ChatParams();
        params.setScratchPad("Log test pad");

        ChatParamsLog log = new ChatParamsLog(LocalDateTime.now(), params);

        when(chatParamsLogRepository.findFirstByTimestampLessThanOrderByTimestampDesc(any(LocalDateTime.class)))
                .thenReturn(Optional.of(log));

        mockMvc.perform(get("/v1/chat/last"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.chatParams.scratchPad").value("Log test pad"));
    }

    /**
     * Tests 404 response when no previous log entry exists.
     *
     * @throws Exception if perform operation fails
     */
    @Test
    @DisplayName("GET /v1/chat/last - Not Found")
    void testGetLastEntryNotFound() throws Exception {
        when(chatParamsLogRepository.findFirstByTimestampLessThanOrderByTimestampDesc(any(LocalDateTime.class)))
                .thenReturn(Optional.empty());

        mockMvc.perform(get("/v1/chat/last"))
                .andExpect(status().isNotFound());
    }

    /**
     * Tests successful retrieval of a chat parameter log by ID.
     *
     * @throws Exception if perform operation fails
     */
    @Test
    @DisplayName("GET /v1/chat/chat-params-log - Success")
    void testGetChatParamsLogSuccess() throws Exception {
        ChatParams params = new ChatParams();
        params.setScratchPad("Sample ScratchPad");
        ChatParamsLog log = new ChatParamsLog(LocalDateTime.now(), params);

        when(chatParamsLogRepository.findById(1L)).thenReturn(Optional.of(log));

        mockMvc.perform(get("/v1/chat/chat-params-log").param("id", "1"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.chatParams.scratchPad").value("Sample ScratchPad"));
    }

    /**
     * Tests deletion of a saved chat parameter log entry.
     *
     * @throws Exception if perform operation fails
     */
    @Test
    @DisplayName("DELETE /v1/chat/chat-params-log - Success")
    void testDeleteChatParamsLogSuccess() throws Exception {
        ChatParamsLog log = new ChatParamsLog();
        when(chatParamsLogRepository.findById(1L)).thenReturn(Optional.of(log));
        doNothing().when(chatParamsLogRepository).delete(log);

        mockMvc.perform(delete("/v1/chat/chat-params-log").param("id", "1"))
                .andExpect(status().isOk());

        verify(chatParamsLogRepository).delete(log);
    }

    /**
     * Tests persisting new chat parameters and generating a log entry.
     *
     * @throws Exception if perform operation fails
     */
    @Test
    @DisplayName("POST /v1/chat/chat-params-log - Saves parameters and messages")
    void testPostChatParamsLog() throws Exception {
        ChatParams requestParams = new ChatParams();
        requestParams.setScratchPad("New Entry ScratchPad");

        ChatMessage message = new ChatMessage(null, ChatMessageRole.USER, "Hello model");
        requestParams.getMessages().add(message);

        ChatMessage savedMessage = new ChatMessage(null, ChatMessageRole.USER, "Hello model");
        savedMessage.setId(10L);

        ChatParams savedParams = new ChatParams();
        savedParams.setScratchPad("New Entry ScratchPad");

        ChatParamsLog savedLog = new ChatParamsLog(LocalDateTime.now(), savedParams);

        when(chatMessageRepository.save(any(ChatMessage.class))).thenReturn(savedMessage);
        when(chatParamsRepository.save(any(ChatParams.class))).thenReturn(savedParams);
        when(chatParamsLogRepository.save(any(ChatParamsLog.class))).thenReturn(savedLog);

        mockMvc.perform(post("/v1/chat/chat-params-log")
                .contentType(MediaType.APPLICATION_JSON)
                .content(objectMapper.writeValueAsString(requestParams)))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.chatParams.scratchPad").value("New Entry ScratchPad"));
    }

    /**
     * Tests generating completions using llama.cpp service.
     *
     * @throws Exception if perform operation fails
     */
    @Test
    @DisplayName("POST /v1/chat/completions - Triggers chat inference")
    void testCompletions() throws Exception {
        ChatParams requestParams = new ChatParams();
        requestParams.setScratchPad("Prompt");
        requestParams.getMessages().add(new ChatMessage(requestParams, ChatMessageRole.USER, "How are you?"));

        when(llamaCppService.chat(any(ChatParams.class))).thenReturn("I am doing well!");

        mockMvc.perform(post("/v1/chat/completions")
                .contentType(MediaType.APPLICATION_JSON)
                .content(objectMapper.writeValueAsString(requestParams)))
                .andExpect(status().isOk())
                .andExpect(content().string("I am doing well!"));

        verify(llamaCppService).freeContext();
        verify(llamaCppService).initializeContext(
                requestParams.getContextSize(),
                requestParams.getBatchSize(),
                requestParams.getuBatchSize()
        );
        verify(llamaCppService).chat(any(ChatParams.class));
    }
}
