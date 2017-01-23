cmake_minimum_required(VERSION 3.2)
project(cfeap)

set(CMAKE_C_STANDARD 11)

set(CFEAP_VERSION_MAJOR 0)
set(CFEAP_VERSION_MINOR 999999)
set(CFEAP_VERSION_PATCH 0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_PATH "${PROJECT_SOURCE_DIR}/src")
set(INC_PATH "${PROJECT_SOURCE_DIR}/include")
set(TEST_PATH "${PROJECT_SOURCE_DIR}/test")

# Build flags
set(COMMON_FLAGS "-Wall -Wl,--no-as-needed -lblas")
set(CMAKE_C_FLAGS_RELEASE "-O3 -march=native ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb ${COMMON_FLAGS}")

configure_file(
    "${SRC_PATH}/cfeap-cfg.h.in"
    "${INC_PATH}/cfeap-cfg.h"
)

# Default is debug build
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Common
list(APPEND INC_PATH "/usr/include")
include_directories(${INC_PATH})

file(GLOB_RECURSE CFEAP_COMMON_SRC "${SRC_PATH}/utils/*.c")
list(APPEND CFEAP_SRC ${CFEAP_COMMON_SRC})
file(GLOB_RECURSE CFEAP_COMMON_SRC "${SRC_PATH}/linalg/*.c")
list(APPEND CFEAP_SRC ${CFEAP_COMMON_SRC})
file(GLOB_RECURSE CFEAP_COMMON_SRC "${SRC_PATH}/yaml/*.c")
list(APPEND CFEAP_SRC ${CFEAP_COMMON_SRC})

# Solid mechanics module
option(USE_SM "Enable solid mechanics module" ON)
if(USE_SM)
    file(GLOB_RECURSE CFEAP_SM_SRC "${SRC_PATH}/sm/*.c")
    list(APPEND CFEAP_SRC ${CFEAP_SM_SRC})
endif()

# Build library
option(BUILD_LIB "Build library" ON)
if(BUILD_LIB)
    set(CFEAP_LIB "lib${PROJECT_NAME}")
    set(CFEAP_LIB_SRC "${SRC_PATH}/cfeap.c")
    add_library(${CFEAP_LIB} STATIC ${CFEAP_LIB_SRC} ${CFEAP_SRC})
endif()

# Build executable binary
option(BUILD_EXE "Build executable binary" ON)
if(BUILD_EXE)
    set(CFEAP_BIN "${PROJECT_NAME}")
    set(CFEAP_BIN_SRC "${SRC_PATH}/main.c")
    add_executable(${CFEAP_BIN} ${CFEAP_BIN_SRC})
    target_link_libraries(${CFEAP_BIN} ${CFEAP_LIB})
endif()

# Build test
option(BUILD_TEST "Build test" ON)
if(BUILD_TEST)
    set(CFEAP_TEST "test-${PROJECT_NAME}")
    include_directories(${TEST_PATH})
    set(CFEAP_TEST_SRC "${TEST_PATH}/test-${PROJECT_NAME}.c")
    add_executable(${CFEAP_TEST} ${CFEAP_TEST_SRC})
    target_link_libraries(${CFEAP_TEST} ${CFEAP_LIB})
endif()

if(NOT MSVC)
    set_target_properties(
        ${CFEAP_LIB}
        PROPERTIES
        PREFIX                           ""
        SUFFIX                           ""
        OUTPUT_NAME                      "lib${PROJECT_NAME}.a"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${PROJECT_SOURCE_DIR}/bin/debug"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/release"
    )
else()
    set_target_properties(
        ${CFEAP_LIB}
        PROPERTIES
        PREFIX                           ""
        SUFFIX                           ""
        OUTPUT_NAME                      "${PROJECT_NAME}.lib"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${PROJECT_SOURCE_DIR}/bin/debug"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/release"
    )
endif()

set_target_properties(
    ${CFEAP_BIN}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${PROJECT_SOURCE_DIR}/bin/debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/release"
)
set_target_properties(
    ${CFEAP_TEST}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${PROJECT_SOURCE_DIR}/bin/debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/release"
)
