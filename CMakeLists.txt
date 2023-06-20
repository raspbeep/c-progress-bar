cmake_minimum_required(VERSION 3.25)
project(progress_bar C)

set(CMAKE_C_STANDARD 23)
SET(CURL_COMP_FLAG "-lcurl")

SET(CMAKE_CXX_FLAGS  "${CURL_COMP_FLAG}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CURL_COMP_FLAG}")

add_executable(progress_bar main.c)
