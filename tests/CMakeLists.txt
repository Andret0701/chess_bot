cmake_minimum_required(VERSION 3.23)

# We are inside /tests, but we use the parent project targets (engine/algorithm/utils).
# So this file assumes the root already defines:
#   - target: engine
#   - target: algorithm
#   - target: utils

# ---- Unity framework ----
add_library(unity STATIC
  "${CMAKE_CURRENT_SOURCE_DIR}/third_party/unity/src/unity.c"
)
target_include_directories(unity PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/third_party/unity/src"
)

# Enable Unity double-precision assertions (TEST_ASSERT_*_DOUBLE*)
target_compile_definitions(unity PUBLIC
  UNITY_INCLUDE_DOUBLE
  UNITY_DOUBLE_PRECISION=1e-12
)

# Optional: nicer warnings for unity too
if(MSVC)
  target_compile_options(unity PRIVATE /W4)
else()
  target_compile_options(unity PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ---- Helper: create a Unity-based test executable and register it with CTest ----
function(add_unity_test TARGET_NAME)
  cmake_parse_arguments(UT "" "" "SOURCES;LIBS" ${ARGN})

  # If you haven't written tests yet, don't hard-fail — just skip cleanly.
  # (This also prevents the "No SOURCES given" error.)
  if(NOT UT_SOURCES)
    message(STATUS "Skipping ${TARGET_NAME}: no test sources found")
    return()
  endif()

  add_executable(${TARGET_NAME} ${UT_SOURCES})
  target_link_libraries(${TARGET_NAME} PRIVATE unity ${UT_LIBS})

  if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W4)
  else()
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic)
  endif()

  add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
endfunction()

# ---- Collect tests (USE FORWARD SLASHES ALWAYS) ----
file(GLOB_RECURSE ENGINE_TEST_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/engine/*.c"
)
file(GLOB_RECURSE ALGO_TEST_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/algorithm/*.c"
)
file(GLOB_RECURSE UTILS_TEST_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.c"
)

# ---- Test executables (link against your libraries) ----
# Easiest: link all libs everywhere; keeps linking simple with static libs.
add_unity_test(engine_tests
  SOURCES "${ENGINE_TEST_SOURCES}"
  LIBS engine algorithm utils
)

add_unity_test(algorithm_tests
  SOURCES "${ALGO_TEST_SOURCES}"
  LIBS engine algorithm utils
)

add_unity_test(utils_tests
  SOURCES "${UTILS_TEST_SOURCES}"
  LIBS engine algorithm utils
)
