cmake_minimum_required(VERSION 2.8)

project(recipes)

aux_source_directory(src SRC_DIR)
add_executable(${PROJECT_NAME} ${SRC_DIR})



