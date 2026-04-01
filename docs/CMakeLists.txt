cmake_minimum_required(VERSION 4.1.2)

project(Docs)

find_package(Doxygen REQUIRED dot)

doxygen_add_docs(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/projects/Gecko/src WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs)
