cmake_minimum_required(VERSION 3.20)

project(QtCalculatorQTestDemo
    VERSION 1.0.0
    DESCRIPTION "Calculatrice graphique Qt C++ avec tests E2E QTest"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

option(BUILD_TESTING "Build tests" ON)

find_package(Qt6 QUIET COMPONENTS Widgets Test)
if (Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets Test)
    set(QT_VERSION_MAJOR 5)
endif()

message(STATUS "Using Qt${QT_VERSION_MAJOR}")

add_library(calculator_core
    src/CalculatorEngine.cpp
    src/CalculatorEngine.h
)

target_include_directories(calculator_core PUBLIC src)

target_link_libraries(calculator_core PUBLIC Qt${QT_VERSION_MAJOR}::Core)

add_library(calculator_ui
    src/CalculatorWindow.cpp
    src/CalculatorWindow.h
)

target_include_directories(calculator_ui PUBLIC src)

target_link_libraries(calculator_ui
    PUBLIC
        calculator_core
        Qt${QT_VERSION_MAJOR}::Widgets
)

add_executable(qt_calculator
    src/main.cpp
)

target_link_libraries(qt_calculator PRIVATE calculator_ui)

include(GNUInstallDirs)
install(TARGETS qt_calculator RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if (BUILD_TESTING)
    enable_testing()

    add_executable(calculator_engine_test
        tests/unit/CalculatorEngineTest.cpp
    )
    target_link_libraries(calculator_engine_test
        PRIVATE
            calculator_core
            Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME unit_calculator_engine COMMAND calculator_engine_test)

    add_executable(calculator_window_e2e_test
        tests/e2e/CalculatorWindowE2ETest.cpp
    )
    target_link_libraries(calculator_window_e2e_test
        PRIVATE
            calculator_ui
            Qt${QT_VERSION_MAJOR}::Test
            Qt${QT_VERSION_MAJOR}::Widgets
    )
    add_test(NAME e2e_calculator_window COMMAND calculator_window_e2e_test)

    set_tests_properties(e2e_calculator_window PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
    )
endif()
