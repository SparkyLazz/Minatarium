cmake_minimum_required(VERSION 4.0)
project(Minatarium C)

set(CMAKE_C_STANDARD 11)

add_executable(Minatarium
        main.c
        Blessing/Blessing.h
        Status/Status.h
        Blessing/BlessingData.c
        Entity/Characters.h
        Utils/PanelUtil.c
        Utils/PanelUtils.h
        Entity/Enemy.c
        Entity/EnemyData.h
        Entity/EnemyData.c
        Entity/CharacterUtils.c
        Entity/Player.c
        Game/Combat.c
        Game/Combat.h
        Blessing/Blessing.h
        Status/Status.h
        Characters/Character.h
        Utils/Utils.h
        Utils/PanelUtil.c
        Characters/CharacterUtil.c
        Utils/PrintUtils.c
        Example/Entity/CharacterUtils.c
)
