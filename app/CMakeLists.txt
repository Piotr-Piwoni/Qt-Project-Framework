cmake_minimum_required(VERSION 3.31)
project(App)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(App
               resources/themes/themes.qrc
               src/main.cpp
               src/theme/ThemeManager.cpp
               src/theme/ThemeManager.hpp
               src/Application.cpp
               src/Application.hpp
               src/utilities/QtLoggerAdapter.hpp)

target_link_libraries(App PRIVATE
                      Engine
                      Qt6::Core
                      Qt6::Gui
                      Qt6::Widgets)


if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(DEBUG_SUFFIX)
	if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
		set(DEBUG_SUFFIX "d")
	endif ()
	set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
	if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
		set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
		if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
			set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
		endif ()
	endif ()
	if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
		add_custom_command(TARGET App POST_BUILD
		                   COMMAND ${CMAKE_COMMAND} -E make_directory
		                   "$<TARGET_FILE_DIR:App>/plugins/platforms/")
		add_custom_command(TARGET App POST_BUILD
		                   COMMAND ${CMAKE_COMMAND} -E copy
		                   "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
		                   "$<TARGET_FILE_DIR:App>/plugins/platforms/")
	endif ()
	foreach (QT_LIB Core Gui Widgets)
		add_custom_command(TARGET App POST_BUILD
		                   COMMAND ${CMAKE_COMMAND} -E copy
		                   "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
		                   "$<TARGET_FILE_DIR:App>")
	endforeach (QT_LIB)
endif ()

