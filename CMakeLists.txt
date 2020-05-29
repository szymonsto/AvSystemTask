cmake_minimum_required(VERSION 3.1)
project(anjay-task C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)

find_package(anjay REQUIRED)

add_executable(${PROJECT_NAME}
               src/main.c
               src/avst_utils.h
               src/avst_utils.c
               src/device_object.h
               src/device_object.c
               src/connectivity_object.h
               src/connectivity_object.c
               src/temperature_object.h
               src/temperature_object.c)
target_link_libraries(${PROJECT_NAME} PRIVATE anjay)
