cmake_minimum_required( VERSION 3.15 )
project( vagon )

set( CMAKE_CXX_STANDARD 20 )
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set( CMAKE_INCLUDE_CURRENT_DIR ON )

#set( CMAKE_BUILD_TYPE Release )
set( CMAKE_BUILD_TYPE Debug )
add_definitions ( -Wall )

set ( vagon_HDRS
          Alternative.h
          InputData.h
          PathData.h
          VagonCore.h
          )

set ( vagon_SRCS
          main.cpp
          Alternative.cpp
	  VagonCore.cpp
	  )

add_executable ( vagon ${vagon_SRCS} )
add_subdirectory(spdlog)
target_link_libraries(vagon spdlog)
#target_include_directories(vagon PUBLIC ../spdlog/include)
