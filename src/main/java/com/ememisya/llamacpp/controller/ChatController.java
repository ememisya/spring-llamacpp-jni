package com.ememisya.llamacpp.controller;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.time.LocalDateTime;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.List;
import java.nio.charset.StandardCharsets;
import com.ememisya.llamacpp.domain.ChatMessage;
import com.ememisya.llamacpp.ChatMessageRole;
import com.ememisya.llamacpp.domain.ChatParams;
import com.ememisya.llamacpp.domain.ChatParamsLog;
import com.ememisya.llamacpp.service.LlamaCppService;
import com.ememisya.llamacpp.repository.ChatParamsLogRepository;
import com.ememisya.llamacpp.repository.ChatParamsRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.web.server.ResponseStatusException;
import com.ememisya.llamacpp.repository.ChatMessageRepository;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import com.ememisya.llamacpp.model.ChatParamsLogIdentifierModel;
import org.springframework.web.bind.annotation.DeleteMapping;

/**
 * REST controller exposing endpoints for chat completions, logging, and
 * context-sensitive llama.cpp interactions.
 *
 * <p>
 * This controller provides:</p>
 * <ul>
 * <li>Retrieval of the most recent {@link ChatParamsLog}</li>
 * <li>Persistence of {@link ChatParams} and associated logs</li>
 * <li>Chat completions with automatic context rebuild when SYSTEM messages
 * change</li>
 * </ul>
 */
@RestController
@RequestMapping("/v1/chat")
public class ChatController {

    /**
     * Repository for persisting and retrieving completions parameter logs.
     */
    @Autowired
    private ChatParamsLogRepository chatParamsLogRepository;

    /**
     * Repository for persisting {@link ChatParams} entities.
     */
    @Autowired
    private ChatParamsRepository chatParamsRepository;

    /**
     * Repository for persisting {@link ChatMessage} entities.
     */
    @Autowired
    private ChatMessageRepository chatMessageRepository;

    /**
     * Service responsible for interacting with the native llama.cpp backend.
     */
    @Autowired(required = false)
    private LlamaCppService llama;

    /**
     * Cached SHA‑256 digest of the last SYSTEM message block. Used to detect
     * when the llama.cpp context must be rebuilt.
     */
    private byte[] lastContextHash;

    /**
     * Default constructor required by Spring.
     */
    public ChatController() {
    }

    /**
     * Retrieves the most recent {@link ChatParamsLog} entry before the given
     * timestamp.
     *
     * @param time optional timestamp; if omitted, the current time is used
     * @return the most recent log entry
     * @throws ResponseStatusException if no entries exist before the timestamp
     */
    @CrossOrigin
    @GetMapping("/last")
    public ChatParamsLog getLastEntry(
            @RequestParam(required = false) final LocalDateTime time) {

        final LocalDateTime effectiveTime
                = (time == null) ? LocalDateTime.now() : time;

        return chatParamsLogRepository
                .findFirstByTimestampLessThanOrderByTimestampDesc(effectiveTime)
                .orElseThrow(()
                        -> new ResponseStatusException(
                        HttpStatus.NOT_FOUND,
                        "No log entries found"
                )
                );
    }

    /**
     * Returns a list of {@link ChatParamsLogIdentifierModel} identifiers.
     *
     * @param year selected year to return entries for
     * @return List of {@link ChatParamsLogIdentifierModel}s for given year.
     */
    @CrossOrigin
    @GetMapping("/chat-params-log-identifiers")
    public List<ChatParamsLogIdentifierModel> getChatParamsLogIdentifierModels(
            @RequestParam final Integer year) {
        return chatParamsLogRepository.findByYear(year);
    }

    /**
     * Deletes a saved completions state.
     *
     * @param id state ID
     */
    @CrossOrigin
    @DeleteMapping("/chat-params-log")
    public void deleteChatParamsLog(@RequestParam final Long id) {
        final ChatParamsLog found = chatParamsLogRepository
                .findById(id)
                .orElseThrow(()
                        -> new ResponseStatusException(
                        HttpStatus.NOT_FOUND,
                        "State not found"
                ));
        chatParamsLogRepository.delete(found);
    }

    /**
     * Loads a saved completions state.
     *
     * @param id state ID
     * @return complete ChatParamsLog
     */
    @CrossOrigin
    @GetMapping("/chat-params-log")
    public ChatParamsLog getChatParamsLog(@RequestParam final Long id) {

        return chatParamsLogRepository
                .findById(id)
                .orElseThrow(()
                        -> new ResponseStatusException(
                        HttpStatus.NOT_FOUND,
                        "State not found"
                ));
    }

    /**
     * Persists a {@link ChatParams} instance and creates a corresponding log
     * entry.
     *
     * @param requestBody the completions parameters to persist
     * @return Persisted {@link ChatParams} entry
     */
    @CrossOrigin
    @PostMapping("/chat-params-log")
    public ChatParamsLog postChatParamsLog(@RequestBody final ChatParams requestBody) {
        final List<ChatMessage> persistentMessages = new ArrayList<>();
        if (requestBody.getMessages() != null
                && !requestBody.getMessages().isEmpty()) {
            int size = requestBody.getMessages().size();
            for (int i = 0; i < size; i++) {
                final ChatMessage transientChatMessage
                        = requestBody.getMessages().remove(0);
                transientChatMessage.setId(null);
                persistentMessages.add(
                        chatMessageRepository.save(transientChatMessage));
            }
        }
        requestBody.setMessages(new ArrayList<>());
        final ChatParams persistedParams
                = chatParamsRepository.save(requestBody);
        persistedParams.getMessages().addAll(persistentMessages);
        final ChatParamsLog log = new ChatParamsLog(
                LocalDateTime.now(), chatParamsRepository.save(
                persistedParams));
        return chatParamsLogRepository.save(log);
    }

    /**
     * Generates a chat completion using llama.cpp. If SYSTEM messages differ
     * from the previous request, the llama.cpp context is rebuilt
     * automatically.
     *
     * @param requestBody the completions parameters including messages and
     * model settings
     * @param forceNewContext if true, then a new context is forced
     * @return the generated assistant response
     * @throws NoSuchAlgorithmException if SHA‑256 is unavailable
     */
    @CrossOrigin
    @PostMapping("/completions")
    public String completions(@RequestBody final ChatParams requestBody,
            @RequestParam(required = false) final boolean forceNewContext)
            throws NoSuchAlgorithmException {

        if (forceNewContext || checkContextChanged(requestBody)) {
            llama.freeContext();
            llama.initializeContext(
                    requestBody.getContextSize(),
                    requestBody.getBatchSize(),
                    requestBody.getuBatchSize()
            );
        }

        try {
            return llama.chat(requestBody);
        } catch (final IllegalStateException e) {
            lastContextHash = null;
            return "Context not initialized, please try again...";
        }
    }

    /**
     * Computes a SHA‑256 digest over all SYSTEM messages in the request. Uses
     * incremental hashing to avoid large intermediate string allocations.
     *
     * @param requestBody the incoming completions parameters
     * @return the computed SHA‑256 digest
     * @throws NoSuchAlgorithmException if SHA‑256 is unavailable
     */
    private byte[] getContextHash(final ChatParams requestBody)
            throws NoSuchAlgorithmException {

        final MessageDigest digest = MessageDigest.getInstance("SHA-256");

        if (requestBody.getMessages() != null) {
            for (final ChatMessage message : requestBody.getMessages()) {
                if (message.getRole() == ChatMessageRole.SYSTEM) {
                    final String text = message.getContent();
                    if (text != null) {
                        digest.update(text.getBytes(StandardCharsets.UTF_8));
                    }
                }
            }
            digest.update(Integer.toString(requestBody.getContextSize()).getBytes());
            digest.update(Integer.toString(requestBody.getBatchSize()).getBytes());
            digest.update(Integer.toString(requestBody.getuBatchSize()).getBytes());
            digest.update(Integer.toString(requestBody.getRngSeed()).getBytes());
            digest.update(Integer.toString(requestBody.getDryAllowedLength()).getBytes());
            digest.update(Integer.toString(requestBody.getDryPenaltyLastN()).getBytes());
            digest.update(Integer.toString(requestBody.getPenaltyLastN()).getBytes());
            digest.update(Float.toString(requestBody.getDryBase()).getBytes());
            digest.update(Float.toString(requestBody.getDryMultiplier()).getBytes());
            digest.update(Float.toString(requestBody.getDynamicTemperature()).getBytes());
            digest.update(Float.toString(requestBody.getFreqPenalty()).getBytes());
            digest.update(Float.toString(requestBody.getMinP()).getBytes());
            digest.update(Float.toString(requestBody.getPresencePenalty()).getBytes());
            digest.update(Float.toString(requestBody.getRepeatPenalty()).getBytes());
            digest.update(Float.toString(requestBody.getTemperature()).getBytes());
        }

        return digest.digest();
    }

    /**
     * Determines whether SYSTEM messages have changed since the last request.
     * If so, the llama.cpp context must be rebuilt.
     *
     * @param requestBody the incoming completions parameters
     * @return true if SYSTEM messages changed; false otherwise
     * @throws NoSuchAlgorithmException if SHA‑256 is unavailable
     */
    private boolean checkContextChanged(final ChatParams requestBody)
            throws NoSuchAlgorithmException {

        final byte[] currentHash = getContextHash(requestBody);
        final boolean rebuildContext = !Arrays.equals(currentHash, lastContextHash);

        lastContextHash = currentHash;
        return rebuildContext;
    }
}
