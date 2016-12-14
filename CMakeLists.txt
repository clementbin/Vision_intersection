cmake_minimum_required (VERSION 2.6)
project (intersection)

find_package (OpenCV REQUIRED)

include_directories(src)

set (SOURCES
  src/intersection.cpp
  src/pattern_matching.cpp
    )
    


set (SRC_EX
    src/threshold_yuv.cpp
    )
    

set (HARRIS_EX
	src/harris.cpp
	src/pattern_matching.cpp
	)

add_executable (intersection ${SOURCES})
add_executable (threshold_yuv ${SRC_EX})
add_executable (harris ${HARRIS_EX})
target_link_libraries (intersection ${OpenCV_LIBS})
target_link_libraries (threshold_yuv ${OpenCV_LIBS})
target_link_libraries (harris ${OpenCV_LIBS})
