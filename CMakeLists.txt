cmake_minimum_required(VERSION 3.28)


project(ClientServer)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SERIALIZATION_LIB_DIR "${PROJECT_SOURCE_DIR}/lib")
set(SERIALIZATION_LIB_NAME "serialization")


find_library(SERIALIZATION_LIB_PATH NAMES serialization HINTS ${SERIALIZATION_LIB_DIR})


include_directories(
    ${PROJECT_SOURCE_DIR}/server/include
    ${PROJECT_SOURCE_DIR}/client/include
)


file(GLOB client_SRCS
    "${PROJECT_SOURCE_DIR}/client/include/*.h"
    "${PROJECT_SOURCE_DIR}/client/src/*.cpp"
)


file(GLOB server_SRCS
    "${PROJECT_SOURCE_DIR}/server/include/*.h"
    "${PROJECT_SOURCE_DIR}/server/src/*.cpp"
)


add_executable(client ${client_SRCS})


add_executable(server ${server_SRCS})


link_directories(${SERIALIZATION_LIB_DIR})


if(NOT SERIALIZATION_LIB_PATH)
    message(FATAL_ERROR "Library 'serialization' not found in ${SERIALIZATION_LIB_DIR}")
else()
    target_link_libraries(client ${SERIALIZATION_LIB_PATH})
    target_link_libraries(server ${SERIALIZATION_LIB_PATH})
endif()