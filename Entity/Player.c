#include <stddef.h>

#include "Characters.h"
#include "../Blessing/Blessing.h"
const Characters playerBluePrint = {
    .name = "",
    .type = CHARACTER_PLAYER,
    .attributes = {
        .maxHp = 100,
        .hp = 100,
        .attack = 10,
        .defense = 10,
        .off = {
            .critChance = 20,
            .critDamage = 80,
            .penetration = 0,
            .damageBoost = 0,
            .accuracy = 75,
        },
        .def = {
            .blockRate = 0,
            .damageReduction = 0,
            .dodge = 0,
        },
        .elem = {
            .fireDamage = 0,
            .fireRes = 0,
            .iceDamage = 0,
            .iceRes = 0,
            .poisonDamage = 0,
            .poisonRes = 0,
        },
        .sustain = {
            .lifeSteal = 0,
            .regen = 0
        },
        .status = {
            .effectHitRate = 20,
            .resistanceEffect = 20
        },
        .util = {
            .reflectDamage = 0
        }
    },
};