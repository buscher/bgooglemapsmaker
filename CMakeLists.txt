project(bgooglemapsmaker)
cmake_minimum_required(VERSION 3.2)

option(USE_GRAPHICSMAGICK "Use GraphicsMagick instead of ImageMagick" OFF)

set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
set(magick_libs "")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

if (NOT USE_GRAPHICSMAGICK)
    find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
    include_directories(${ImageMagick_INCLUDE_DIRS})

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
    set (magick_libs ${ImageMagick_LIBRARIES})
else()
    find_package(GraphicsMagick++ REQUIRED)
    include_directories(${GraphicsMagick++_INCLUDE_DIRS})

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp ${GraphicsMagick++_CFLAGS}")
    set (magick_libs ${GraphicsMagick++_LIBS})
endif()

set(bgooglemapsmaker_SRCS bgooglemapsmaker.cpp main.cpp)

add_executable(bgooglemapsmaker ${bgooglemapsmaker_SRCS})
target_link_libraries(bgooglemapsmaker ${magick_libs})
