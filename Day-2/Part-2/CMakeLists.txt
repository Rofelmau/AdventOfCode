cmake_minimum_required(VERSION 3.14)

project(AoC_Day-2_Part-2 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(AoC_Day-2_Part-2
  main.cpp
)
target_link_libraries(AoC_Day-2_Part-2 Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS AoC_Day-2_Part-2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

configure_file("../input.txt" "input.txt" COPYONLY)
