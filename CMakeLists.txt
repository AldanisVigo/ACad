cmake_minimum_required(VERSION 3.25)
project(ACad)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use libc++ and target macOS 13+
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -mmacosx-version-min=13.0")

# Find GTKmm via pkg-config
find_package(PkgConfig REQUIRED)
find_package(nlohmann_json REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

# Your sources
set(SOURCES
    src/main.cpp
    src/core/CircuitComponent.cpp
    src/ui/CircuitCanvas.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} 
    PRIVATE ${GTKMM_LIBRARIES} 
    PRIVATE nlohmann_json::nlohmann_json
)


# Put the binary in a bin/ directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)

# Add a custom 'run' target
add_custom_target(run
    COMMAND "${CMAKE_BINARY_DIR}/../bin/${PROJECT_NAME}"
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running ${PROJECT_NAME}..."
)
