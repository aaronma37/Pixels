project(sim)

# Automatically pick up files added to src
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Add a dynamic library called gameplay - this builds gameplay.dll
add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME} PUBLIC godot::cpp 
"-Wl,--whole-archive libinterfaces.a -Wl,--no-whole-archive"
  des_interfaces)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX src FILES ${SOURCES})
