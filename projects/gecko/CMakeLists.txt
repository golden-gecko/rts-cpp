cmake_minimum_required(VERSION 4.2)

project(Gecko)

file(GLOB HEADER_FILES "src/*.hpp" "src/*/*.hpp")
file(GLOB SOURCE_FILES "src/*.cpp" "src/*/*.cpp")

list(APPEND COMMON_INCLUDE ${PROJECT_SOURCE_DIR}/src)
list(APPEND COMMON_INCLUDE ${PROJECT_SOURCE_DIR}/src/Components)
list(APPEND COMMON_INCLUDE ${PROJECT_SOURCE_DIR}/src/DataLayers)
list(APPEND COMMON_INCLUDE ${PROJECT_SOURCE_DIR}/src/Layers)
list(APPEND COMMON_INCLUDE ${PROJECT_SOURCE_DIR}/src/Objects)
list(APPEND COMMON_INCLUDE ${PROJECT_SOURCE_DIR}/src/Orders)
list(APPEND COMMON_INCLUDE ${PROJECT_SOURCE_DIR}/src/UI)

include_directories(${COMMON_INCLUDE})

add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE src/Gecko.hpp)

set_property(TARGET ${PROJECT_NAME} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${COMMON_INCLUDE})

target_link_libraries(${PROJECT_NAME} debug boostd optimized boost)
# target_link_libraries(${PROJECT_NAME} debug bulletd optimized bullet)
target_link_libraries(${PROJECT_NAME} debug jsoncppd optimized jsoncpp)
target_link_libraries(${PROJECT_NAME} debug ogred optimized ogre)
target_link_libraries(${PROJECT_NAME} debug oisd optimized ois)
target_link_libraries(${PROJECT_NAME} debug sdld optimized sdl)
target_link_libraries(${PROJECT_NAME} debug ultralightd optimized ultralight)
