cmake_minimum_required(VERSION 4.0)
project(Minatarium C)

set(CMAKE_C_STANDARD 11)

add_executable(Minatarium
        main.c
        Entity/Player.h
        Blessing/Blessing.h
        Status/Status.h
        Entity/Player.c
        Entity/Enemy.h
        Entity/Enemy.c
        Blessing/BlessingData.c
)
