cmake_minimum_required(VERSION 3.31)
project(ProjectName)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_PREFIX_PATH "W:/Qt/6.11.0/mingw_64")
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_subdirectory(engine)
add_subdirectory(app)
