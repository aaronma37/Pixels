cmake_minimum_required(VERSION 3.19)
project(gdextension-gameplay)

message (${CMAKE_C_COMPILER})
message (${CMAKE_CXX_COMPILER})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} -fno-gnu-unique -fPIC -DHOT_RELOAD_ENABLED -DLINUX_ENABLED -DUNIX_ENABLED -DDEBUG_ENABLED -DDEBUG_METHODS_ENABLED")

include(FetchContent)

FetchContent_Declare(
        GDExtension
        GIT_REPOSITORY https://github.com/godotengine/godot-cpp.git
        GIT_TAG godot-4.2-stable
)

FetchContent_MakeAvailable(GDExtension)

# DES lib

add_library(
  des_interfaces
  STATIC
  des/src/effect_interface.cpp
  des/src/state.cpp
  des/src/event.cpp
  des/src/effects.cpp
  des/src/utils.cpp
  des/src/combat_log.cpp
  des/src/unit.cpp
  des/src/action_handlers/shadowbolt_action_handler.cpp
  des/src/action_handlers/corruption_action_handler.cpp
  des/src/action_handlers/lifetap_action_handler.cpp
  des/src/action_handlers/idle_action_handler.cpp
  des/src/action_handlers/minor_heal_action_handler.cpp
  )
target_include_directories(
  des_interfaces PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/des/src/>)

add_executable(sod_sim des/src/main.cpp)
target_link_libraries(sod_sim PUBLIC 
  "-Wl,--whole-archive libdes_interfaces.a -Wl,--no-whole-archive"
  des_interfaces 
  )

target_compile_options(sod_sim PUBLIC -fsanitize=address -fopenmp -O3 -std=c++20 -g)
target_link_options(sod_sim PUBLIC -fsanitize=address -fopenmp -O3 -std=c++20 -g)

# target_compile_options(sod_sim PUBLIC -fopenmp -O3 -std=c++20 -ffast-math)
# target_link_options(sod_sim PUBLIC  -fopenmp -O3 -std=c++20 -ffast-math)



# Godot

project(sim)

# Automatically pick up files added to src
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Add a dynamic library called gameplay - this builds gameplay.dll
add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME} PUBLIC godot::cpp 
"-Wl,--whole-archive libdes_interfaces.a -Wl,--no-whole-archive"
  des_interfaces)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX src FILES ${SOURCES})
