cmake_minimum_required(VERSION 3.31)
project(Engine)

set(CMAKE_CXX_STANDARD 23)

add_library(Engine STATIC
            src/utilities/logging/Logger.hpp
            src/utilities/logging/Logger.cpp
            src/utilities/logging/LoggingTypes.hpp
            src/utilities/Colour.hpp
            src/utilities/StringUtils.hpp
            src/utilities/Time.hpp)

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(Engine PRIVATE stdc++exp)