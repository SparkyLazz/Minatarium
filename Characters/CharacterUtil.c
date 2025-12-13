#include <stdio.h>

#include "Character.h"
#include "../Utils/Utils.h"

#define LABEL_WIDTH 14
//=====================================
//  CHARACTER PROPERTY
//=====================================
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
//=====================================
//  CHARACTER RENDERER
//=====================================
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
const char* BlessingEffectString(const BlessingEffectType e) {
    switch (e) {
        case DAMAGE_BOOST: return "Damage Boost";
        case CRITICAL_CHANGE: return "Critical Change";
        case CRITICAL_DAMAGE: return "Critical Damage";
        case ARMOR_PENETRATION: return "Armor Penetration";
        case FIRE_DAMAGE: return "Fire Damage";
        case ICE_DAMAGE: return "Ice Damage";
        case POISON_DAMAGE: return "Poison Damage";
        case HP_BOOST: return "HP Boost";
        case DEFENSE_BOOST: return "Defense Boost";
        case SHIELD_BOOST: return "Shield Boost";
        case REGEN: return "Regeneration";
        case LIFESTEAL: return "Life steal";
        case REGEN_BOOST: return "Regen Boost";
        case THORN: return "Thorn";
        case LUCK: return "Luck";
        case INVULNERABLE: return "Invulnerable";
        default: return "None";
    }
}
const char* StatusEffectString(const BlessingDot status) {
    switch (status.DoT.type) {
        case BURN: return "Burn";
        case POISON: return "Poison";
        case STUN: return "Stun";
        case FREEZE: return "Freeze";
        default: return "None";
    }
}
void BlessingStackColor(const long long int stacks) {
    if (stacks <= 10)
        printColor(COL_WHITE, "%lld", stacks);
    else if (stacks <= 30)
        printColor(COL_GREEN, "%lld", stacks);
    else if (stacks <= 50)
        printColor(COL_CYAN, "%lld", stacks);
    else if (stacks <= 75)
        printColor(COL_MAGENTA, "%lld", stacks);
    else if (stacks <= 100)
        printColor(COL_YELLOW, "%lld", stacks);
    else
        printColor(COL_BRIGHT_RED, "%lld", stacks);
}
void BlessingTotalValueColor(const float totalPercent) {
    if (totalPercent < 25.0f)
        printColor(COL_WHITE, "%.2f%%\n", totalPercent);
    else if (totalPercent < 75.0f)
        printColor(COL_GREEN, "%.2f%%\n", totalPercent);
    else if (totalPercent < 150.0f)
        printColor(COL_CYAN, "%.2f%%\n", totalPercent);
    else if (totalPercent < 300.0f)
        printColor(COL_MAGENTA, "%.2f%%\n", totalPercent);
    else
        printColor(COL_YELLOW, "%.2f%%\n", totalPercent);
}
void CharacterBlessingTab(void* data) {
    const Character* character = (Character*)data;
    for (int i = 0; i < character->blessingCount; i++) {
        printColor(COL_BOLD, "[%d] %s\n", i + 1, character->currentBlessing[i]);
        printf("    %-*s : ", LABEL_WIDTH, "Rarity"); RarityColor(character->currentBlessing[i].rarity);
        printf("\n");
        printf("    %-*s : ", LABEL_WIDTH, "Stacks"); BlessingStackColor(character->currentBlessing[i].stacks);
        printf("\n");
        printf("\n");
        printf("    %-*s : %d\n", LABEL_WIDTH, "Effects",character->currentBlessing[i].effectsCount);
        for (int j = 0; j < character->currentBlessing[i].effectsCount; j++) {
            printf("        %-*s : %s\n",
                   LABEL_WIDTH,
                   "Type",
                   BlessingEffectString(character->currentBlessing[i].effects[j].type));
            printf("        %-*s : %.2f%%\n",
                   LABEL_WIDTH,
                   "Base Value",
                   character->currentBlessing[i].effects[j].baseValue * 100.0f);
            printf("        %-*s : ",
                   LABEL_WIDTH,
                   "Total Value");
            BlessingTotalValueColor(
                character->currentBlessing[i].effects[j].baseValue *
                (float)character->currentBlessing[i].stacks
            );
            printf("\n");
        }
        if (character->currentBlessing[i].dotsCount > 0) {
            printf("\n");
            printf("    %-*s :\n", LABEL_WIDTH, "On-Hit Statuses");
            for (int j = 0; j < character->currentBlessing[i].dotsCount; j++) {
                printf("        %-*s : %s\n",
                       LABEL_WIDTH,
                       "Type",
                       StatusEffectString(character->currentBlessing[i].dots[j]));
                printf("        %-*s : %d\n",
                       LABEL_WIDTH,
                       "Duration",
                       character->currentBlessing[i].dots[j].DoT.duration);
                printf("        %-*s : %.2f\n",
                       LABEL_WIDTH,
                       "Base Value",
                       character->currentBlessing[i].dots[j].DoT.baseAmount);
                printf("        %-*s : ",
                       LABEL_WIDTH,
                       "Total Value");
                BlessingTotalValueColor(
                    character->currentBlessing[i].dots[j].DoT.baseAmount *
                    (float)character->currentBlessing[i].stacks
                );
                printf("\n");
            }
        }
        printf("\n");
    }
}
void CharacterRenderer(Character* character) {
    Tab tabs[3] = {
        {"Attribute Stat", CharacterStatsTab},
        {"Blessing Stat", CharacterBlessingTab}
    };
    showTabPanel(tabs, 2, character);
}
