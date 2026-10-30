/**
 * @file llama_jni_messages.cpp
 *
 * Implementation of message-related helper functions for the JNI llama.cpp
 * integration layer.
 */

#include "llama_jni_internal.hpp"

#ifdef _WIN32
    #define STRDUP _strdup
#else
    #define STRDUP strdup
#endif

namespace {

/**
 * Extract an individual Java ChatMessage entity into a llama_chat_message struct.
 *
 * @param env JNI environment pointer
 * @param msgObj Java ChatMessage instance
 * @param midGetRole Method ID for getRole()
 * @param midGetContent Method ID for getContent()
 * @param midEnumName Method ID for enum.name()
 * @return Populated llama_chat_message struct, or empty strings on failure
 */
llama_chat_message extract_single_message(
        JNIEnv* env,
        jobject msgObj,
        jmethodID midGetRole,
        jmethodID midGetContent,
        jmethodID midEnumName) {
    llama_chat_message m = {nullptr, nullptr};
    jobject roleEnumObj = env->CallObjectMethod(msgObj, midGetRole);
    if (!roleEnumObj) return m;

    jstring roleStr = (jstring) env->CallObjectMethod(roleEnumObj, midEnumName);
    if (!roleStr) return m;

    const char* roleC = env->GetStringUTFChars(roleStr, nullptr);
    std::string roleLower;
    for (const char* p = roleC; *p; ++p) {
        roleLower.push_back(std::tolower(*p));
    }
    env->ReleaseStringUTFChars(roleStr, roleC);

    jstring contentStr = (jstring) env->CallObjectMethod(msgObj, midGetContent);
    if (!contentStr) return m;

    const char* contentC = env->GetStringUTFChars(contentStr, nullptr);
    m.role = STRDUP(roleLower.c_str());
    m.content = STRDUP(contentC);
    env->ReleaseStringUTFChars(contentStr, contentC);
    return m;
}

/**
 * Concatenate message contents directly as a fallback when no chat template is found.
 *
 * @param msgs Vector of llama_chat_message structs
 * @return Concatenated fallback prompt string
 */
std::string concatenate_fallback_messages(const std::vector<llama_chat_message>& msgs) {
    LOG("apply_chat_template: model has no chat template; concatenating message content directly");
    std::string concatenated;
    for (const auto& m : msgs) {
        if (m.content) {
            concatenated.append(m.content);
            concatenated.append("\n");
        }
    }
    if (!concatenated.empty()) {
        concatenated.pop_back();
    }
    return concatenated;
}

/**
 * Resize buffer and re-apply chat template if the initial buffer was too small.
 *
 * @param tmpl Chat template string
 * @param msgs Vector of llama_chat_message structs
 * @param buffer Vector buffer for template output
 * @return Length of formatted template string, or negative on failure
 */
int32_t resize_and_apply_template(
        const char* tmpl,
        const std::vector<llama_chat_message>& msgs,
        std::vector<char>& buffer) {
    int32_t len = llama_chat_apply_template(tmpl, msgs.data(), msgs.size(), true, buffer.data(), buffer.size());
    if (len > static_cast<int32_t>(buffer.size())) {
        buffer.resize(len);
        len = llama_chat_apply_template(tmpl, msgs.data(), msgs.size(), true, buffer.data(), buffer.size());
    }
    return len;
}

} // namespace

/**
 * Extract chat messages from a Java ChatParams object and convert them into
 * llama_chat_message structs suitable for llama.cpp.
 *
 * @param env JNI environment pointer
 * @param chatParamsObj Java ChatParams instance containing a List<ChatMessage>
 * @return A vector of llama_chat_message structs. Caller must free memory using free_messages().
 */
std::vector<llama_chat_message> extract_messages(
        JNIEnv* env,
        jobject chatParamsObj) {
    std::vector<llama_chat_message> out;
    jclass paramsCls = env->GetObjectClass(chatParamsObj);
    if (!paramsCls) return out;

    jmethodID midGetMessages = env->GetMethodID(paramsCls, "getMessages", "()Ljava/util/List;");
    if (!midGetMessages) return out;

    jobject listObj = env->CallObjectMethod(chatParamsObj, midGetMessages);
    if (!listObj) return out;

    jclass listCls = env->GetObjectClass(listObj);
    jmethodID midSize = env->GetMethodID(listCls, "size", "()I");
    jmethodID midGet = env->GetMethodID(listCls, "get", "(I)Ljava/lang/Object;");
    if (!midSize || !midGet) return out;

    jint size = env->CallIntMethod(listObj, midSize);
    jclass msgCls = env->FindClass("com/ememisya/llamacpp/domain/ChatMessage");
    jclass roleEnumCls = env->FindClass("com/ememisya/llamacpp/ChatMessageRole");
    if (!msgCls || !roleEnumCls) return out;

    jmethodID midGetRole = env->GetMethodID(msgCls, "getRole", "()Lcom/ememisya/llamacpp/ChatMessageRole;");
    jmethodID midGetContent = env->GetMethodID(msgCls, "getContent", "()Ljava/lang/String;");
    jmethodID midEnumName = env->GetMethodID(roleEnumCls, "name", "()Ljava/lang/String;");
    if (!midGetRole || !midGetContent || !midEnumName) return out;

    for (int i = 0; i < size; i++) {
        jobject msgObj = env->CallObjectMethod(listObj, midGet, i);
        if (!msgObj) continue;
        llama_chat_message m = extract_single_message(env, msgObj, midGetRole, midGetContent, midEnumName);
        if (m.role && m.content) out.push_back(m);
    }
    return out;
}

/**
 * Free memory allocated for llama_chat_message role/content strings.
 *
 * @param msgs Vector of llama_chat_message structs to free
 */
void free_messages(std::vector<llama_chat_message>& msgs) {
    for (auto& m : msgs) {
        if (m.role) free((void*) m.role);
        if (m.content) free((void*) m.content);
    }
    msgs.clear();
}

/**
 * Apply the model's chat template to a list of messages.
 *
 * @param model Loaded llama_model instance
 * @param ctx Active llama_context
 * @param msgs Vector of llama_chat_message structs
 * @return A fully formatted prompt string, or an empty string on failure
 */
std::string apply_chat_template(
        const struct llama_model* model,
        llama_context* ctx,
        const std::vector<llama_chat_message>& msgs) {
    const char* tmpl = llama_model_chat_template(model, nullptr);
    if (!tmpl) {
        return concatenate_fallback_messages(msgs);
    }

    std::vector<char> buffer(llama_n_ctx(ctx));
    int32_t len = resize_and_apply_template(tmpl, msgs, buffer);
    if (len < 0) {
        LOG("apply_chat_template: template application failed");
        return {};
    }

    return std::string(buffer.data(), len);
}