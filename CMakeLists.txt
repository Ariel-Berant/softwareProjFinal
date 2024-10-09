cmake_minimum_required(VERSION 3.28)
project(softwareProjFinal C)

set(CMAKE_C_STANDARD 90)

add_executable(softwareProjFinal
        symnmf.c
        symnmf.h
        matrixFuncs.c
        vectorFuncs.c
        symnmfmodule.c
)
