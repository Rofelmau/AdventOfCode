cmake_minimum_required(VERSION 3.14)

project(AoC_Day-4_Part-2 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

qt_add_executable(AoC_Day-4_Part-2
  main.cpp
)
target_link_libraries(AoC_Day-4_Part-2 PRIVATE Qt6::Core)

install(TARGETS AoC_Day-4_Part-2
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

configure_file("../input.txt" "input.txt" COPYONLY)

if (APPLE)
    set_target_properties(AoC_Day-4_Part-2 PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif (APPLE)

qt6_generate_deploy_app_script(
    TARGET AoC_Day-4_Part-2
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
    NO_TRANSLATIONS
)
install(SCRIPT ${deploy_script})
