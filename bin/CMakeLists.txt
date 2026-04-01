cmake_minimum_required(VERSION 4.1.2)

project(Bin)

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

source_group(TREE ${CMAKE_SOURCE_DIR}/bin PREFIX "Binary Files" FILES ${BINARY_FILES})

add_custom_target(${PROJECT_NAME} SOURCES ${BINARY_FILES})
