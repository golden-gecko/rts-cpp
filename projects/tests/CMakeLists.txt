cmake_minimum_required(VERSION 4.2)

project(Tests)

# Create target and add source files
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

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

# Find GoogleTest
find_package(GTest REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES})
