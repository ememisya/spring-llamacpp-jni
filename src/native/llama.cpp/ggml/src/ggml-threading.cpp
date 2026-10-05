#include "ggml-threading.h"
#include <mutex>

#ifndef _WIN32
namespace {
    static std::mutex * get_critical_section_mutex() {
        static std::mutex * mtx = new std::mutex();
        return mtx;
    }
}  // namespace

void ggml_critical_section_start() {
    get_critical_section_mutex()->lock();
}

void ggml_critical_section_end() {
    get_critical_section_mutex()->unlock();
}
#else
#include <windows.h>

namespace {
    struct CriticalSectionWrapper {
        CRITICAL_SECTION cs;

        CriticalSectionWrapper() { InitializeCriticalSection(&cs); }

        ~CriticalSectionWrapper() { DeleteCriticalSection(&cs); }
    };

    static CriticalSectionWrapper * get_cs_wrapper() {
        static CriticalSectionWrapper * wrapper = new CriticalSectionWrapper();
        return wrapper;
    }
}  // namespace

void ggml_critical_section_start() {
    EnterCriticalSection(&get_cs_wrapper()->cs);
}

void ggml_critical_section_end() {
    LeaveCriticalSection(&get_cs_wrapper()->cs);
}
#endif