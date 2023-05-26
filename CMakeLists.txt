cmake_minimum_required(VERSION 3.10)

project(big-blind-cxxlib)

add_executable(big-blind-cxxlib)

target_sources(big-blind-cxxlib  PRIVATE
    main.cpp
)

include_directories(big_blind)

