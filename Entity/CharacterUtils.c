#include "Characters.h"
#include "../Utils/PanelUtils.h"
#include <stdio.h>

// Convert rarity enum to string
const char* getRarityString(BlessingRarity rarity) {
    switch(rarity) {
        case RARITY_COMMON: return "Common";
        case RARITY_RARE: return "Rare";
        case RARITY_EPIC: return "Epic";
        case RARITY_LEGENDARY: return "Legendary";
        default: return "Unknown";
    }
}

// Convert blessing effect type to string
const char* getBlessingEffectString(BlessingEffectType type) {
    switch(type) {
        case BEF_DAMAGE_BOOST: return "Damage Boost";
        case BEF_CRIT_CHANCE: return "Crit Chance";
        case BEF_CRIT_DAMAGE: return "Crit Damage";
        case BEF_ARMOR_PEN: return "Armor Penetration";
        case BEF_ACCURACY: return "Accuracy";
        case BEF_FIRE_DAMAGE: return "Fire Damage";
        case BEF_ICE_DAMAGE: return "Ice Damage";
        case BEF_POISON_DAMAGE: return "Poison Damage";
        case BEF_MAX_HP: return "Max HP";
        case BEF_DEFENSE: return "Defense";
        case BEF_DAMAGE_REDUCTION: return "Damage Reduction";
        case BEF_DODGE: return "Dodge";
        case BEF_SHIELD: return "Shield";
        case BEF_REFLECT: return "Reflect";
        case BEF_FIRE_RES: return "Fire Resistance";
        case BEF_ICE_RES: return "Ice Resistance";
        case BEF_POISON_RES: return "Poison Resistance";
        case BEF_RES_STUN: return "Stun Resistance";
        case BEF_RES_SILENCE: return "Silence Resistance";
        case BEF_REGEN: return "Regeneration";
        case BEF_LIFESTEAL: return "Lifesteal";
        case BEF_HEAL_BOOST: return "Heal Boost";
        case BEF_THORN: return "Thorn";
        case BEF_LUCK: return "Luck";
        case BEF_INVULNERABLE: return "Invulnerable";
        case BEF_GRANT_SKILL: return "Grant Skill";
        default: return "Unknown";
    }
}

// Convert status type to string
const char* getStatusTypeString(StatusType type) {
    switch(type) {
        case STATUS_NONE: return "None";
        case STATUS_BURN: return "Burn";
        case STATUS_POISON: return "Poison";
        case STATUS_STUN: return "Stun";
        case STATUS_FREEZE: return "Freeze";
        case STATUS_SILENCE: return "Silence";
        case STATUS_DEF_DOWN: return "Defense Down";
        case STATUS_ATK_DOWN: return "Attack Down";
        case STATUS_DMG_TAKEN_UP: return "Damage Taken Up";
        case STATUS_CRIT_RES_DOWN: return "Crit Resistance Down";
        case STATUS_WEAKEN: return "Weaken";
        case STATUS_VULNERABLE: return "Vulnerable";
        default: return "Unknown";
    }
}

void renderBaseStats(void* data) {
    const Characters* c = (Characters*)data;
    printf(">> PRIMARY STATS\n");
    printf("   HP               : %lld\n", c->attributes.hp);
    printf("   Attack           : %lld\n", c->attributes.attack);
    printf("   Defense          : %lld\n", c->attributes.defense);
    printf("\n");

    printf(">> OFFENSIVE STATS\n");
    printf("   Crit Chance      : %d\n",  c->attributes.off.critChance);
    printf("   Crit Damage      : %d\n",  c->attributes.off.critDamage);
    printf("   Penetration      : %d\n",  c->attributes.off.penetration);
    printf("   Accuracy         : %d\n",  c->attributes.off.accuracy);
    printf("\n");

    printf(">> DEFENSIVE STATS\n");
    printf("   Dodge            : %d\n", c->attributes.def.dodge);
    printf("   Damage Reduction : %d\n", c->attributes.def.damageReduction);
    printf("\n");

    printf(">> ELEMENTAL STATS\n");
    printf("   Fire Damage      : %d\n", c->attributes.elem.fireDamage);
    printf("   Fire Resistance  : %d\n", c->attributes.elem.fireRes);
    printf("   Ice Damage       : %d\n", c->attributes.elem.iceDamage);
    printf("   Ice Resistance   : %d\n", c->attributes.elem.iceRes);
    printf("   Poison Damage    : %d\n", c->attributes.elem.poisonDamage);
    printf("   Poison Resist    : %d\n", c->attributes.elem.poisonRes);
    printf("\n");

    printf(">> CONTROL & UTILITY STATS\n");
    printf("   Effect Hit Rate  : %d\n", c->attributes.status.effectHitRate);
    printf("   Effect Resist    : %d\n", c->attributes.status.resistanceEffect);
    printf("   Reflect Damage   : %d\n", c->attributes.util.reflectDamage);
}

void renderBlessingData(void* data) {
    const Characters* c = (Characters*)data;
    for (int i = 0; i < c->blessingCount; i++) {
        printf("Name : %s", c->currentBlessing[i].name);
        printf("   Rarity : %s", getRarityString(c->currentBlessing[i].rarity));
        printf("   Blessing Effect [%d] : ", c->currentBlessing[i].effectCount);
        for (int j = 0; j < c->currentBlessing[i].effectCount; j++) {
            printf("      [%d] %s : %f", j, getBlessingEffectString(c->currentBlessing[i].effects[j].type), c->currentBlessing[i].effects[j].baseValue);
            printf("      Stack : %lld", c->currentBlessing[i].effects[j].stacks);

        }
    }
}

void renderStatusData(void* data) {

}

void showPlayerStats(Characters* player) {
    Tab tabs[3] = {
        {"Base Stat", renderBaseStats},
        {"Blessing Stat", renderBlessingData},
        {"Status Stat", renderStatusData}
    };

    showTabPanel(tabs, 3, player);
}