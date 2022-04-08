cmake_minimum_required(VERSION 3.20)
project(Lab5_Contract_de_studii)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(Lab5_Contract_de_studii
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
        Vector.h exception.h)
