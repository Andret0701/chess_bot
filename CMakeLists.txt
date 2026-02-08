cmake_minimum_required(VERSION 3.23)
project(ChessBot C)

# -------------------------
# Language / defaults
# -------------------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Default to Release if user didn't specify
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# -------------------------
# Collect sources
# -------------------------
file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/engine/*.c"
)
file(GLOB_RECURSE ALGO_SRC CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/algorithm/*.c"
)
file(GLOB_RECURSE UTILS_SRC CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/utils/*.c"
)

# -------------------------
# Targets
# -------------------------
add_library(engine STATIC ${ENGINE_SRC})
add_library(algorithm STATIC ${ALGO_SRC})
add_library(utils STATIC ${UTILS_SRC})

add_executable(andobot src/main.c src/uci.c)

target_link_libraries(andobot PRIVATE engine algorithm utils m)

# -------------------------
# Include dirs
# -------------------------
target_include_directories(engine PUBLIC
  "${CMAKE_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/engine"
  "${CMAKE_SOURCE_DIR}/engine/attack_generation"
  "${CMAKE_SOURCE_DIR}/engine/benchmark"
  "${CMAKE_SOURCE_DIR}/engine/can_move"
  "${CMAKE_SOURCE_DIR}/engine/capture_generation"
  "${CMAKE_SOURCE_DIR}/engine/move_generation"
)

target_include_directories(algorithm PUBLIC
  "${CMAKE_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/algorithm"
  "${CMAKE_SOURCE_DIR}/algorithm/heuristic"
)

target_include_directories(utils PUBLIC
  "${CMAKE_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/utils"
)

# -------------------------
# Warnings (all configs)
# -------------------------
foreach(t engine algorithm utils andobot)
  target_compile_options(${t} PRIVATE -Wall -Wextra -Wpedantic)
endforeach()

# -------------------------
# Performance flags (Release only)
#  - ARM/Pi safe: no -mpopcnt (x86-only)
#  - keep -O3, -march=native, etc.
# -------------------------
set(FAST_C_FLAGS
  -O3
  -march=native
  -mtune=native
  -funroll-loops
  -ffast-math
  -falign-functions=32
  -fprefetch-loop-arrays
  -fomit-frame-pointer
  -fno-plt
)

foreach(t engine algorithm utils andobot)
  target_compile_options(${t} PRIVATE $<$<CONFIG:Release>:${FAST_C_FLAGS}>)
  target_compile_definitions(${t} PRIVATE $<$<CONFIG:Release>:NDEBUG>)
endforeach()

# -------------------------
# Unity build (like your old unity.c trick)
# This often makes a *big* difference for chess engines.
# -------------------------
set_target_properties(engine algorithm utils PROPERTIES UNITY_BUILD ON)

# -------------------------
# LTO / IPO (like -flto) if supported
# -------------------------
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_OK OUTPUT IPO_MSG)
if(IPO_OK)
  set_property(TARGET engine algorithm utils andobot PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
  message(WARNING "IPO/LTO not supported: ${IPO_MSG}")
endif()

# Optional: emit compile_commands.json for easy inspection
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
