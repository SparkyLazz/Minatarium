cmake_minimum_required(VERSION 4.0)
project(Minatarium C)

set(CMAKE_C_STANDARD 11)

add_executable(Minatarium
        main.c
        Blessing/Blessing.h
        Status/Status.h
        Blessing/BlessingData.c
        Entity/Characters.h
        Utils/PanelUtils.c
        Utils/PanelUtils.h
)
