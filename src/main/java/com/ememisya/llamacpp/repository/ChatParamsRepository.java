package com.ememisya.llamacpp.repository;

import com.ememisya.llamacpp.domain.ChatParams;
import org.springframework.data.jpa.repository.JpaRepository;

/**
 * Repository for {@link ChatParams} entities.
 */
public interface ChatParamsRepository extends JpaRepository<ChatParams, Long> {

}
