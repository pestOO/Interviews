cmake_minimum_required(VERSION 2.8)

project(StoryBoard)
set(CMAKE_CXX_STANDARD 14)

include_directories(
    "${CMAKE_SOURCE_DIR}/headers")

# TODO(EZ): create library
set(HEADER_FILES
    headers/Note.hpp
    headers/Storyboard.hpp)

add_executable(NoteTest ${SOURCES} ${HEADER_FILES} tests/notetest.cpp)
add_executable(StoryBoardTest ${SOURCES} ${HEADER_FILES} tests/storyboardtest.cpp)
