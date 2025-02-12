cmake_minimum_required(VERSION 3.28)
project(Server)
#--------help-----------
#mkdir build && cd build
#cmake .. build 
#cmake --build . --config debug/release
#-----------------------


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 


set(SERIALIZATION_LIB_DIR "${PROJECT_SOURCE_DIR}/lib")
set(SERIALIZATION_LIB_NAME "serialization.lib")


# Найдите библиотеку core
#HINTS доп подсказки где искать библиотеку 
find_library(SERIALIZATION_LIB_PATH NAMES serialization HINTS ${SERIALIZATION_LIB_DIR})

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/include
)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
)


add_executable(server ${all_SRCS})

# Укажите путь к библиотеке, если она находится в другом каталоге
link_directories(${SERIALIZATION_LIB_DIR})


# Свяжите исполняемый файл с библиотекой
#target_link_libraries(AppSerial_ES ${SERIALIZATION_LIB_PATH})

if(NOT SERIALIZATION_LIB_PATH)
        message(FATAL_ERROR "Library 'serialization' not found in ${SERIALIZATION_LIB_DIR}")
    else()
        target_link_libraries(server ${SERIALIZATION_LIB_PATH})
    endif()
