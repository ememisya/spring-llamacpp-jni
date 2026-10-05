package com.ememisya.llamacpp.repository;

import com.ememisya.llamacpp.domain.ChatParamsLog;
import java.time.LocalDateTime;
import java.util.List;
import java.util.Optional;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import com.ememisya.llamacpp.model.ChatParamsLogIdentifierModel;

/**
 * Repository for {@link ChatParamsLog} entities.
 */
public interface ChatParamsLogRepository
        extends JpaRepository<ChatParamsLog, Long> {

    /**
     * Returns all log entries between two timestamps.
     *
     * @param start start timestamp (inclusive)
     * @param end end timestamp (inclusive)
     * @return list of matching log entries
     */
    List<ChatParamsLog> findByTimestampBetween(
            final LocalDateTime start,
            final LocalDateTime end);

    /**
     * Returns the most recent log entry that occurred before the supplied
     * timestamp.
     *
     * @param end upper timestamp bound
     * @return optional containing the matching log entry
     */
    Optional<ChatParamsLog>
            findFirstByTimestampLessThanOrderByTimestampDesc(
                    final LocalDateTime end);

    /**
     * Returns all chat states ordered by timestamp descending.
     *
     * @param year pagination information
     * @return page of chat states
     */
    @Query("SELECT c FROM ChatParamsLog c WHERE EXTRACT(YEAR FROM c.timestamp) = :year")
    List<ChatParamsLogIdentifierModel> findByYear(@Param("year") final int year);

}
