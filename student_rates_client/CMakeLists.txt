cmake_minimum_required(VERSION 3.5)

project(student_rates_client LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS  Network  REQUIRED)
find_package(Qt5 COMPONENTS  Sql REQUIRED)
find_package(Qt5 COMPONENTS  Core REQUIRED)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

if(ANDROID)
  add_library(student_rates_client SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(student_rates_client
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
endif()

add_library(input
    input.h
    input.cpp
    input.ui
)

add_library(rateswindow
    rateswindow.h
    rateswindow.cpp
    rateswindow.ui
)
get_property(QT_CORE_INCLUDE_DIRS TARGET Qt5::Core PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
list(GET QT_CORE_INCLUDE_DIRS 0 QT_CORE_MAIN_INCLUDE_DIR)
get_filename_component(QT_MAIN_DIR ${QT_CORE_MAIN_INCLUDE_DIR}/.. ABSOLUTE)


install(FILES $<TARGET_FILE:Qt5::Widgets> DESTINATION ${PROJECT_BINARY_DIR}/Release CONFIGURATIONS Release)
install(FILES $<TARGET_FILE:Qt5::Widgets> DESTINATION ${PROJECT_BINARY_DIR}/Debug CONFIGURATIONS Debug)

install(FILES $<TARGET_FILE:Qt5::Network> DESTINATION ${PROJECT_BINARY_DIR}/Release CONFIGURATIONS Release)
install(FILES $<TARGET_FILE:Qt5::Network> DESTINATION ${PROJECT_BINARY_DIR}/Debug CONFIGURATIONS Debug)

install(FILES $<TARGET_FILE:Qt5::Sql> DESTINATION ${PROJECT_BINARY_DIR}/Release CONFIGURATIONS Release)
install(FILES $<TARGET_FILE:Qt5::Sql> DESTINATION ${PROJECT_BINARY_DIR}/Debug CONFIGURATIONS Debug)


install(FILES $<TARGET_FILE:Qt5::Core> DESTINATION ${PROJECT_BINARY_DIR}/Release CONFIGURATIONS Release)
install(FILES $<TARGET_FILE:Qt5::Core> DESTINATION ${PROJECT_BINARY_DIR}/Debug CONFIGURATIONS Debug)

install(FILES $<TARGET_FILE:Qt5::Gui> DESTINATION ${PROJECT_BINARY_DIR}/Release CONFIGURATIONS Release)
install(FILES $<TARGET_FILE:Qt5::Gui> DESTINATION ${PROJECT_BINARY_DIR}/Debug CONFIGURATIONS Debug)


install(DIRECTORY ${QT_MAIN_DIR}/plugins/sqldrivers/
        DESTINATION ${PROJECT_BINARY_DIR}/Release/Plugins/sqldrivers USE_SOURCE_PERMISSIONS
        CONFIGURATIONS Release)

install(DIRECTORY ${QT_MAIN_DIR}/plugins/sqldrivers/
        DESTINATION ${PROJECT_BINARY_DIR}/Debug/Plugins/sqldrivers USE_SOURCE_PERMISSIONS
        CONFIGURATIONS Debug)

install(DIRECTORY ${QT_MAIN_DIR}/plugins/platforms/
            DESTINATION ${PROJECT_BINARY_DIR}/Release/Plugins/platforms USE_SOURCE_PERMISSIONS
            CONFIGURATIONS Release)
install(DIRECTORY ${QT_MAIN_DIR}/plugins/platforms/
                DESTINATION ${PROJECT_BINARY_DIR}/Debug/Plugins/platforms USE_SOURCE_PERMISSIONS
                CONFIGURATIONS Debug)


target_link_libraries(student_rates_client PRIVATE Qt5::Widgets rateswindow )
target_link_libraries(input Qt5::Widgets  Qt5::Network)
target_link_libraries(rateswindow Qt5::Widgets  Qt5::Network input Qt5::Sql)
