cmake_minimum_required(VERSION 4.2)

project(Gecko LANGUAGES CXX)

set(CMAKE_DEBUG_POSTFIX _d)

# Create target and add source files
file(GLOB_RECURSE BINARY_FILES
    ${CMAKE_SOURCE_DIR}/bin/*.cfg
    ${CMAKE_SOURCE_DIR}/bin/*.jpg
    ${CMAKE_SOURCE_DIR}/bin/*.json
    ${CMAKE_SOURCE_DIR}/bin/*.log
    ${CMAKE_SOURCE_DIR}/bin/*.material
    ${CMAKE_SOURCE_DIR}/bin/*.mesh
    ${CMAKE_SOURCE_DIR}/bin/*.particle
    ${CMAKE_SOURCE_DIR}/bin/*.png
    ${CMAKE_SOURCE_DIR}/bin/*.rcss
    ${CMAKE_SOURCE_DIR}/bin/*.rml
    ${CMAKE_SOURCE_DIR}/bin/*.tga
    ${CMAKE_SOURCE_DIR}/bin/*.ttf
    ${CMAKE_SOURCE_DIR}/bin/*.zip
)
file(GLOB_RECURSE HEADER_FILES
    ${PROJECT_SOURCE_DIR}/src/*.hpp
)
file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

source_group(TREE ${CMAKE_SOURCE_DIR}/bin PREFIX "Binary Files" FILES ${BINARY_FILES})
source_group(TREE ${PROJECT_SOURCE_DIR}/src PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE ${PROJECT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SOURCE_FILES})

add_library(${PROJECT_NAME} STATIC ${BINARY_FILES} ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/Gecko/Gecko.hpp)

# Find Boost
find_package(Boost REQUIRED COMPONENTS filesystem log_setup program_options)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

# Find JsonCpp
find_package(jsoncpp REQUIRED)
get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(${PROJECT_NAME} PUBLIC ${JSON_INC_PATH})
target_link_libraries(${PROJECT_NAME} jsoncpp_lib)

# Find Ogre
find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)
target_link_libraries(${PROJECT_NAME} OgreBites)

# Find OIS
target_include_directories(${PROJECT_NAME} PUBLIC ${OIS_DIR}/include/ois)
target_link_libraries(${PROJECT_NAME} debug ${OIS_DIR}/lib/OIS_d.lib optimized ${OIS_DIR}/lib/OIS.lib)

# Find PathFinder
find_package(PathFinder REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${PathFinder_DIR}/../../include/PathFinder)
target_link_libraries(${PROJECT_NAME} PathFinder::PathFinder)

# Find RmlUi
find_package(RmlUi REQUIRED)
target_link_libraries(${PROJECT_NAME} RmlUi::RmlUi)
