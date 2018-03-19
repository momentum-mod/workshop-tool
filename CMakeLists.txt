cmake_minimum_required(VERSION 3.0)
project(momentum-workshop-tool)

#include cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

#set C++14
set(CMAKE_CXX_STANDARD 14)

#-- Platform dependant stuff --
if (MSVC)
    set(GUI_TYPE WIN32)
endif(MSVC)

# Set some Apple MacOS Specific settings
if (APPLE)
	set(GUI_TYPE MACOSX_BUNDLE)
endif (APPLE)


#find Qt libraries
find_package(Qt5Widgets)
get_target_property(QtWidgets_location Qt5::Widgets LOCATION)
find_package(Qt5Core)
get_target_property(QtCore_location Qt5::Core LOCATION)
find_package(Qt5Gui)
get_target_property(QtGui_location Qt5::Gui LOCATION)

#find steamworks libraries
find_package(STEAMWORKS REQUIRED)
include_directories(${STEAMWORKS_INCLUDE_DIR})

set (PROJECT_SRCS 
		src/main.cpp
        src/mainwindow.hpp src/mainwindow.cpp
        src/workshopitem.hpp src/workshopitem.cpp
        src/tags.hpp
        src/language.hpp
        src/fileselector.hpp src/fileselector.cpp
        src/imageselector.hpp src/imageselector.cpp)

add_executable(momentum-workshop-tool ${GUI_TYPE} ${PROJECT_SRCS})

target_link_libraries(momentum-workshop-tool 
		Qt5::Widgets
		Qt5::Core 
		Qt5::Gui 
		${STEAMWORKS_LIBRARY})
