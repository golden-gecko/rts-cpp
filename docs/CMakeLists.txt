cmake_minimum_required(VERSION 4.2)

project(rts)

find_package(Doxygen REQUIRED dot)

doxygen_add_docs(docs ${CMAKE_SOURCE_DIR}/projects/Gecko/src WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs)
