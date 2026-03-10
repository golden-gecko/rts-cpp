cmake_minimum_required(VERSION 4.1.2)

project(Editor)

# Create target and add source files
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

source_group(TREE ${PROJECT_SOURCE_DIR}/src PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE ${PROJECT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SOURCE_FILES})

add_executable(${PROJECT_NAME} WIN32 ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/system
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

# Find Gecko
target_link_libraries(${PROJECT_NAME} Gecko)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS base core net)
target_link_libraries(${PROJECT_NAME} wxWidgets::wxWidgets)
