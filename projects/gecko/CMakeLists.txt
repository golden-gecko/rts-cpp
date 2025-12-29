cmake_minimum_required(VERSION 4.2)

project(Gecko LANGUAGES CXX)

set(CMAKE_DEBUG_POSTFIX _d)

# Create target and add source files
file(GLOB_RECURSE BINARY_FILES ${CMAKE_SOURCE_DIR}/bin/*.json ${CMAKE_SOURCE_DIR}/bin/*.material ${CMAKE_SOURCE_DIR}/bin/*.png)
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

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
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_DIR}/include)
target_link_libraries(${PROJECT_NAME} debug ${Boost_DIR}/lib/libboost_atomic-vc143-mt-gd-x64-1_90.lib optimized ${Boost_DIR}/lib/libboost_atomic-vc143-mt-x64-1_90.lib)
target_link_libraries(${PROJECT_NAME} debug ${Boost_DIR}/lib/libboost_log-vc143-mt-gd-x64-1_90.lib optimized ${Boost_DIR}/lib/libboost_log-vc143-mt-x64-1_90.lib)
target_link_libraries(${PROJECT_NAME} debug ${Boost_DIR}/lib/libboost_log_setup-vc143-mt-gd-x64-1_90.lib optimized ${Boost_DIR}/lib/libboost_log_setup-vc143-mt-x64-1_90.lib)
target_link_libraries(${PROJECT_NAME} debug ${Boost_DIR}/lib/libboost_filesystem-vc143-mt-gd-x64-1_90.lib optimized ${Boost_DIR}/lib/libboost_filesystem-vc143-mt-x64-1_90.lib)
target_link_libraries(${PROJECT_NAME} debug ${Boost_DIR}/lib/libboost_program_options-vc143-mt-gd-x64-1_90.lib optimized ${Boost_DIR}/lib/libboost_program_options-vc143-mt-x64-1_90.lib)
target_link_libraries(${PROJECT_NAME} debug ${Boost_DIR}/lib/libboost_thread-vc143-mt-gd-x64-1_90.lib optimized ${Boost_DIR}/lib/libboost_thread-vc143-mt-x64-1_90.lib)

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

# Find Ultralight
target_include_directories(${PROJECT_NAME} PUBLIC ${ULTRALIGHT_DIR}/include)
target_link_libraries(${PROJECT_NAME} ${ULTRALIGHT_DIR}/lib/AppCore.lib)
target_link_libraries(${PROJECT_NAME} ${ULTRALIGHT_DIR}/lib/Ultralight.lib)
target_link_libraries(${PROJECT_NAME} ${ULTRALIGHT_DIR}/lib/UltralightCore.lib)
target_link_libraries(${PROJECT_NAME} ${ULTRALIGHT_DIR}/lib/WebCore.lib)
