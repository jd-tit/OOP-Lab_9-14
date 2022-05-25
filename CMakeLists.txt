cmake_minimum_required(VERSION 3.20)
project(Lab9_Qt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Lab9_Qt
        src/main.cpp
        src/domain.cpp
        include/domain.h
        src/controller.cpp
        include/controller.h
        src/UserInterface/user_interface.cpp
        include/user_interface.h
        include/repo.h
        src/validate.cpp
        include/validate.h
        src/test.cpp
        include/test.h
        include/undo_action.hpp
        src/undo_action.cpp
        include/Vector.h include/exception.h src/contract.cpp include/contract.h
        include/undo_action.hpp src/undo_action.cpp src/UserInterface/main_window.cpp include/main_window.hpp src/UserInterface/contract_window.cpp src/UserInterface/contract_window.h)

target_link_libraries(Lab9_Qt
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

target_include_directories(Lab9_Qt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)