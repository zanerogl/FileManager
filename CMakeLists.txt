cmake_minimum_required(VERSION 3.25)
project(FileManager)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(QRC_SOURCE_FILE res/res.qrc)
qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILE})
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(FileManager ${QRC_FILES} main.cpp src/filemanager.cpp include/filemanager.h ui/filemanager.ui)
target_link_libraries(FileManager
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

