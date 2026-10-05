cmake_minimum_required(VERSION 3.10)
project(llama_jni LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(JNI REQUIRED)

# ---------------------------------------------------------------------------
# WIN32 dllpath helper
# ---------------------------------------------------------------------------
if (WIN32)
    add_library(dllpath SHARED dllpath.c)

    target_include_directories(dllpath PRIVATE ${JNI_INCLUDE_DIRS})

    set_target_properties(dllpath PROPERTIES
        OUTPUT_NAME "dllpath"
    )
endif()

# ---------------------------------------------------------------------------
# llama_jni library (isolated includes)
# ---------------------------------------------------------------------------
add_library(llama_jni SHARED
    llama_jni.cpp
    llama_jni_context.cpp
    llama_jni_sampler.cpp
    llama_jni_messages.cpp
    llama_jni_generate.cpp
)

target_include_directories(llama_jni PRIVATE
    ${JNI_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/llama.cpp
    ${CMAKE_SOURCE_DIR}/llama.cpp/include
    ${CMAKE_SOURCE_DIR}/llama.cpp/ggml/include
)

target_link_directories(llama_jni PRIVATE
    ${CMAKE_SOURCE_DIR}/llama.cpp/build/bin
)

target_link_libraries(llama_jni
    ${JNI_LIBRARIES}
    llama
)

# ---------------------------------------------------------------------------
# Maven output path
# ---------------------------------------------------------------------------
set(MAVEN_TARGET_CLASSES_DIR "${CMAKE_SOURCE_DIR}/../../target/classes")
