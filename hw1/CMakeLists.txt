cmake_minimum_required(VERSION 3.16)
project("BigInteger")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(biginteger main.cpp biginteger.h biginteger.cpp )
