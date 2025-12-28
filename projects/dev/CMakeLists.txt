cmake_minimum_required(VERSION 4.2)

project(Dev)

file(GLOB HEADER_FILES "src/*.hpp" "src/*/*.hpp")
file(GLOB SOURCE_FILES "src/*.cpp" "src/*/*.cpp")

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/system
)

target_precompile_headers(${PROJECT_NAME} REUSE_FROM Gecko)

# Find Gecko
target_link_libraries(${PROJECT_NAME} Gecko)
