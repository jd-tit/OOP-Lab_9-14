cmake_minimum_required(VERSION 3.20)
project(Lab9_Qt)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Lab9_Qt
        main.cpp
        domain.cpp
        domain.h
        controller.cpp
        controller.h
        user_interface.cpp
        user_interface.h
        repo.h
        validate.cpp
        validate.h
        test.cpp
        test.h
        undo_action.hpp
        undo_action.cpp
        Vector.h
        exception.h
        contract.cpp
        contract.h
        )

#set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

set(GCOV_OPTIONS ${GCOV_OPTIONS} )

target_link_libraries(Lab9_Qt
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )
