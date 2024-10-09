cmake_minimum_required(VERSION 3.28)
project(softwareProjFinal C)

set(CMAKE_C_STANDARD 90)

add_executable(softwareProjFinal main.c
        symnmf.c
        symnmf.h
        symnmfmodule.c
        matrixFuncs.c
        vectorFuncs.c
)
