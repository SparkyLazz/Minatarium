#include <stdio.h>

#include "Character.h"
#include "../Utils/Utils.h"
Character playerBluePrint = {
    .name = "",
    .type = PLAYER,
    .attribute = {
        .maxHP = 100,
        .hp = 100,
        .attack = 10,
        .defense = 5,
        .criticalChange = 20,
        .criticalDamage = 80,
        .damageBoost = 0,
        .accuracy = 75,
        .fireDamage = 0,
        .iceDamage = 0,
        .poisonDamage = 0,
        .fireResistance = 0,
        .iceResistance = 0,
        .poisonResistance = 0,
        .lifeSteal = 0,
        .regen = 0,
    },
    .blessingCount = 0,
    .statusCount = 0,
};

void CharacterStatsTab(void* data) {
    const Character* character = (Character*)data;
    printColor(COL_BOLD, "Primary Stats\n");
    printf("   HP                : %lld\n", character->attribute.maxHP);
    printf("   Attack            : %lld\n", character->attribute.attack);
    printf("   Defense           : %lld\n", character->attribute.defense);
    printf("\n");

    printColor(COL_BOLD, "Offensive Stats\n");
    printf("   Crit Chance       : %d\n",  character->attribute.criticalChange);
    printf("   Crit Damage       : %d\n",  character->attribute.criticalDamage);
    printf("   Accuracy          : %d\n",  character->attribute.accuracy);
    printf("   Damage Boost      : %d\n",  character->attribute.damageBoost);
    printf("\n");

    printColor(COL_BOLD, "Elemental Stats\n");
    printf("   Fire Damage       : %d\n",  character->attribute.fireDamage);
    printf("   Ice Damage        : %d\n",  character->attribute.iceDamage);
    printf("   Poison Damage     : %d\n",  character->attribute.poisonDamage);
    printf("\n");

    printf("   Fire Resistance   : %d\n",  character->attribute.fireResistance);
    printf("   Ice Resistance    : %d\n",  character->attribute.iceResistance);
    printf("   Poison Resistance : %d\n",  character->attribute.poisonResistance);
    printf("\n");

    printColor(COL_BOLD, "Unique Stats\n");
    printf("   Life Steal        : %d\n",  character->attribute.lifeSteal);
    printf("   Regen             : %d\n",  character->attribute.regen);
}

void RarityColor(const BlessingRarity rarity) {
    switch (rarity) {
        case RARITY_COMMON:
            printColor(COL_GREEN, "Common");
            break;
        case RARITY_RARE:
            printColor(COL_CYAN, "Rare");
            break;
        case RARITY_EPIC:
            printColor(COL_MAGENTA, "Epic");
            break;
        case RARITY_LEGENDARY:
            printColor(COL_YELLOW, "Legendary");
    }
}

void CharacterBlessingTab(void* data) {
    const Character* character = (Character*)data;
    for (int i = 0; i < character->blessingCount; i++) {
        printColor(COL_BOLD, "[%d] %s", i + 1, character->currentBlessing[i]);
        printf("    Rarity : "); RarityColor(character->currentBlessing[i].rarity);
        printf("\n");


    }
}