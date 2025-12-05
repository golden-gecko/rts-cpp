cmake_minimum_required(VERSION 4.2)

project(Dev)

file(GLOB HEADER_FILES "src/*.hpp" "src/*/*.hpp")
file(GLOB SOURCE_FILES "src/*.cpp" "src/*/*.cpp")

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE ../Gecko/src/Gecko.hpp)

target_link_libraries(${PROJECT_NAME} Gecko)
