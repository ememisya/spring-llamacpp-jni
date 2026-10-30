#include "llama_jni_internal.hpp"
#include <cstring>
#include <cstdio>

ClassifierHead& get_classifier(const unsigned char* initial_ptr) {
    static ClassifierHead head = [initial_ptr]() {
        ClassifierHead h;
        const unsigned char* ptr = initial_ptr;

        auto read_bytes = [&](void* dest, size_t size) {
            std::memcpy(dest, ptr, size);
            ptr += size;
        };

        read_bytes(&h.num_classes, sizeof(int));
        read_bytes(&h.input_dim, sizeof(int));
        read_bytes(&h.pooler_dim, sizeof(int));
        
        h.pooler_weights.resize(h.pooler_dim * h.input_dim);
        read_bytes(h.pooler_weights.data(), h.pooler_weights.size() * sizeof(float));
        
        h.pooler_biases.resize(h.pooler_dim);
        read_bytes(h.pooler_biases.data(), h.pooler_biases.size() * sizeof(float));

        h.weights.resize(h.num_classes * h.input_dim);
        read_bytes(h.weights.data(), h.weights.size() * sizeof(float));
        
        h.biases.resize(h.num_classes);
        read_bytes(h.biases.data(), h.biases.size() * sizeof(float));

        std::printf("[TRACE] get_classifier: Loaded from embedded header! classes=%d, input_dim=%d\n", h.num_classes, h.input_dim);
        std::fflush(stdout);

        return h;
    }();
    return head;
}