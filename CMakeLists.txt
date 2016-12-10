cmake_minimum_required (VERSION 2.6)
project (intersection)

find_package (OpenCV REQUIRED)

include_directories(src)

set (SOURCES
  src/intersection.cpp
  src/pattern_matching.cpp
    )
    


set (SRC_EX
    src/example_opencv.cpp
    )
    
add_executable (intersection ${SOURCES})
add_executable (example_opencv ${SRC_EX})
target_link_libraries (intersection ${OpenCV_LIBS})
target_link_libraries (example_opencv ${OpenCV_LIBS})
