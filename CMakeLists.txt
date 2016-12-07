cmake_minimum_required (VERSION 2.6)
project (intersection)

find_package (OpenCV REQUIRED)


set (SOURCES
    src/intersection.cpp
    )

add_executable (intersection ${SOURCES})
target_link_libraries (intersection ${OpenCV_LIBS})
