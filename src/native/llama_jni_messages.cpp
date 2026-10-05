/**
 * @file llama_jni_messages.cpp
 *
 * Implementation of message‑related helper functions for the JNI llama.cpp
 * integration layer. This file handles:
 *
 *  - Extracting Java ChatParams messages into llama_chat_message structs
 *  - Freeing allocated message memory
 *  - Applying the model's chat template to produce a final prompt string
 *
 * All declarations are defined in llama_jni_internal.hpp.
 */

#include "llama_jni_internal.hpp"

#ifdef _WIN32
    #define STRDUP _strdup
#else
    #define STRDUP strdup
#endif

/**
 * Extract chat messages from a Java ChatParams object and convert them into
 * llama_chat_message structs suitable for llama.cpp.
 *
 * @param env            JNI environment pointer
 * @param chatParamsObj  Java ChatParams instance containing a List<ChatMessage>
 *
 * @return A vector of llama_chat_message structs. Caller must free memory
 *         using free_messages().
 */
std::vector<llama_chat_message> extract_messages(
        JNIEnv* env,
        jobject chatParamsObj) {

    std::vector<llama_chat_message> out;

    // ChatParams class
    jclass paramsCls = env->GetObjectClass(chatParamsObj);
    if (!paramsCls) {
        LOG("extract_messages: failed to obtain ChatParams class");
        return out;
    }

    // getMessages()
    jmethodID midGetMessages =
        env->GetMethodID(paramsCls, "getMessages", "()Ljava/util/List;");
    if (!midGetMessages) {
        LOG("extract_messages: getMessages() not found");
        return out;
    }

    jobject listObj = env->CallObjectMethod(chatParamsObj, midGetMessages);
    if (!listObj) {
        LOG("extract_messages: message list is null");
        return out;
    }

    // java.util.List reflection
    jclass listCls = env->GetObjectClass(listObj);
    jmethodID midSize = env->GetMethodID(listCls, "size", "()I");
    jmethodID midGet  = env->GetMethodID(listCls, "get", "(I)Ljava/lang/Object;");
    if (!midSize || !midGet) {
        LOG("extract_messages: List methods missing");
        return out;
    }

    jint size = env->CallIntMethod(listObj, midSize);

    // ChatMessageEntity class
    jclass msgCls =
        env->FindClass("com/ememisya/llamacpp/domain/ChatMessage");
    if (!msgCls) {
        LOG("extract_messages: ChatMessage class not found");
        return out;
    }

    // getRole(): ()Lcom/ememisya/llamacpp/ChatMessageRole;
    jmethodID midGetRole =
        env->GetMethodID(
            msgCls,
            "getRole",
            "()Lcom/ememisya/llamacpp/ChatMessageRole;"
        );
    if (!midGetRole) {
        LOG("extract_messages: getRole() not found or wrong signature");
        return out;
    }

    jmethodID midGetContent =
        env->GetMethodID(msgCls, "getContent", "()Ljava/lang/String;");
    if (!midGetContent) {
        LOG("extract_messages: getContent() not found");
        return out;
    }

    // ChatMessageRole enum class
    jclass roleEnumCls =
        env->FindClass("com/ememisya/llamacpp/ChatMessageRole");
    if (!roleEnumCls) {
        LOG("extract_messages: ChatMessageRole enum class not found");
        return out;
    }

    // enum.name(): ()Ljava/lang/String;
    jmethodID midEnumName =
        env->GetMethodID(roleEnumCls, "name", "()Ljava/lang/String;");
    if (!midEnumName) {
        LOG("extract_messages: enum.name() not found");
        return out;
    }

    // Iterate messages
    for (int i = 0; i < size; i++) {

        jobject msgObj = env->CallObjectMethod(listObj, midGet, i);
        if (!msgObj) continue;

        // getRole() → enum object
        jobject roleEnumObj = env->CallObjectMethod(msgObj, midGetRole);
        if (!roleEnumObj) continue;

        // enum.name() → String
        jstring roleStr = (jstring) env->CallObjectMethod(roleEnumObj, midEnumName);
        if (!roleStr) continue;

        const char* roleC = env->GetStringUTFChars(roleStr, nullptr);

        // Convert enum name to lowercase for llama.cpp
        std::string roleLower;
        for (const char* p = roleC; *p; ++p) {
            roleLower.push_back(std::tolower(*p));
        }

        env->ReleaseStringUTFChars(roleStr, roleC);

        // Extract content
        jstring contentStr = (jstring) env->CallObjectMethod(msgObj, midGetContent);
        if (!contentStr) continue;

        const char* contentC = env->GetStringUTFChars(contentStr, nullptr);

        // Build llama_chat_message
        llama_chat_message m;
        m.role    = STRDUP(roleLower.c_str());
        m.content = STRDUP(contentC);
        out.push_back(m);

        env->ReleaseStringUTFChars(contentStr, contentC);
    }

    LOG("extract_messages: extracted %d messages", (int) out.size());
    return out;
}

/**
 * Free memory allocated for llama_chat_message role/content strings.
 *
 * @param msgs Vector of llama_chat_message structs to free
 */
void free_messages(std::vector<llama_chat_message>& msgs) {
    for (auto& m : msgs) {
        if (m.role)    free((void*) m.role);
        if (m.content) free((void*) m.content);
    }
    msgs.clear();
}

/**
 * Apply the model's chat template to a list of messages.
 *
 * @param model  Loaded llama_model instance
 * @param ctx    Active llama_context
 * @param msgs   Vector of llama_chat_message structs
 *
 * @return A fully formatted prompt string, or an empty string on failure
 */
std::string apply_chat_template(
        llama_model* model,
        llama_context* ctx,
        const std::vector<llama_chat_message>& msgs) {

    const char* tmpl = llama_model_chat_template(model, nullptr);
    if (!tmpl) {
        LOG("apply_chat_template: model has no chat template");
        return {};
    }

    std::vector<char> buffer(llama_n_ctx(ctx));

    int32_t len = llama_chat_apply_template(
        tmpl,
        msgs.data(),
        msgs.size(),
        true,               // add assistant prompt
        buffer.data(),
        buffer.size()
    );

    if (len < 0) {
        LOG("apply_chat_template: initial template application failed");
        return {};
    }

    if ((size_t) len > buffer.size()) {
        buffer.resize(len);

        len = llama_chat_apply_template(
            tmpl,
            msgs.data(),
            msgs.size(),
            true,
            buffer.data(),
            buffer.size()
        );

        if (len < 0) {
            LOG("apply_chat_template: second template application failed");
            return {};
        }
    }

    LOG("apply_chat_template: produced %d bytes of prompt", len);
    return std::string(buffer.data(), len);
}
