package com.ememisya.llamacpp.repository;

import com.ememisya.llamacpp.domain.ChatParams;
import com.ememisya.llamacpp.domain.ChatParamsLog;
import com.ememisya.llamacpp.model.ChatParamsLogIdentifierModel;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.orm.jpa.DataJpaTest;
import org.springframework.boot.test.autoconfigure.orm.jpa.TestEntityManager;

import java.time.LocalDateTime;
import java.util.List;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Data JPA integration tests for {@link ChatParamsLogRepository}.
 */
@DataJpaTest
class ChatParamsLogRepositoryTest {

    /**
     * Entity manager helper for test setups.
     */
    @Autowired
    private TestEntityManager entityManager;

    /**
     * Repository instance under test.
     */
    @Autowired
    private ChatParamsLogRepository repository;

    /**
     * Default constructor required by Checkstyle.
     */
    ChatParamsLogRepositoryTest() {
    }

    /**
     * Tests finding log entries between two timestamps.
     */
    @Test
    @DisplayName("Should find entries between given timestamps")
    void testFindByTimestampBetween() {
        ChatParams params = new ChatParams();
        params.setScratchPad("Log 1");
        entityManager.persist(params);

        LocalDateTime t1 = LocalDateTime.of(2026, 1, 10, 10, 0);
        ChatParamsLog log1 = new ChatParamsLog(t1, params);
        entityManager.persist(log1);
        

        List<ChatParamsLog> result = repository.findByTimestampBetween(
                LocalDateTime.of(2026, 1, 1, 0, 0),
                LocalDateTime.of(2026, 1, 31, 23, 59)
        );

        assertEquals(1, result.size());
        assertEquals("Log 1", result.get(0).getChatParams().getScratchPad());
    }

    /**
     * Tests finding the most recent log entry before a given timestamp.
     */
    @Test
    @DisplayName("Should find first entry less than given timestamp ordered descending")
    void testFindFirstByTimestampLessThanOrderByTimestampDesc() {
        ChatParams params1 = new ChatParams();
        params1.setScratchPad("Older");
        entityManager.persist(params1);

        ChatParams params2 = new ChatParams();
        params2.setScratchPad("Newer");
        entityManager.persist(params2);

        entityManager.persist(new ChatParamsLog(LocalDateTime.of(2026, 1, 1, 10, 0), params1));
        entityManager.persist(new ChatParamsLog(LocalDateTime.of(2026, 1, 2, 10, 0), params2));

        Optional<ChatParamsLog> result = repository
                .findFirstByTimestampLessThanOrderByTimestampDesc(LocalDateTime.of(2026, 1, 3, 0, 0));

        assertTrue(result.isPresent());
        assertEquals("Newer", result.get().getChatParams().getScratchPad());
    }

    /**
     * Tests JPQL query selecting projection by year.
     */
    @Test
    @DisplayName("Should find log identifier models by year")
    void testFindByYear() {
        ChatParams params = new ChatParams();
        params.setScratchPad("Projection Test");
        entityManager.persist(params);

        entityManager.persist(new ChatParamsLog(LocalDateTime.of(2026, 5, 20, 12, 0), params));

        List<ChatParamsLogIdentifierModel> results = repository.findByYear(2026);

        assertEquals(1, results.size());
        assertEquals(2026, results.get(0).getTimestamp().getYear());
    }
}