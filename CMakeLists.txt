cmake_minimum_required(VERSION 4.0)
project(Minatarium C)

set(CMAKE_C_STANDARD 11)

add_executable(Minatarium
        Blessing/Blessing.h
        Characters/Character.h
        Characters/CharacterUtil.c
        Status/Status.h
        Utils/PanelUtil.c
        Utils/PrintUtils.c
        main.c
)
