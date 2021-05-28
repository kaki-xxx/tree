cmake_minimum_required(VERSION 3.16)
project(tree VERSION 0.1)
add_executable(tree main.c tree.c)
configure_file(config.h.in config.h)
target_include_directories(tree PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
