cmake_minimum_required(VERSION 3.20)
project(Minatarium C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Shared game sources (everything except the two entry points).
set(MINATARIUM_SOURCES
        Blessing/Blessing.h
        Blessing/BlessingData.c
        Characters/Character.h
        Characters/CharacterUtil.c
        Game/Game.h
        Game/GameDamage.c
        Game/GameLoop.c
        Game/GameReward.c
        Status/Status.h
        Utils/PanelUtil.c
        Utils/PrintUtils.c
        Utils/Utils.h
)

add_executable(Minatarium main.c ${MINATARIUM_SOURCES})

# Regression tests for the damage engine. Run with: ctest --test-dir <builddir>
enable_testing()
add_executable(MinatariumTests Tests/test_damage.c ${MINATARIUM_SOURCES})
target_compile_definitions(MinatariumTests PRIVATE MINATARIUM_TESTING=1)
add_test(NAME damage_engine COMMAND MinatariumTests)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Minatarium PRIVATE -Wall -Wextra -Wswitch-enum)
    target_compile_options(MinatariumTests PRIVATE -Wall -Wextra -Wswitch-enum)
endif ()
