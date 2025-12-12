#include "Characters.h"
#include "../Utils/PanelUtils.h"
#include <stdio.h>
#include <string.h>

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

// Core computation available to the whole project
void computeEffectiveAttributes(const Characters* c, CombatAttributes* out) {
    if (!c || !out) return;
    CombatAttributes eff = c->attributes; // start from base

    for (int i = 0; i < c->blessingCount; ++i) {
        const ActiveBlessing* ab = &c->currentBlessing[i];
        const Blessing* b = ab->def;
        if (b == NULL || ab->stacks <= 0) continue;
        for (int j = 0; j < b->effectCount; ++j) {
            const BlessingEffect* e = &b->effects[j];
            float add = e->baseValue * (float)ab->stacks;
            switch (e->type) {
                case BEF_DAMAGE_BOOST:      eff.off.damageBoost     += (int)add; break;
                case BEF_CRIT_CHANCE:       eff.off.critChance      += (int)add; break;
                case BEF_CRIT_DAMAGE:       eff.off.critDamage      += (int)add; break;
                case BEF_ARMOR_PEN:         eff.off.penetration     += (int)add; break;
                case BEF_ACCURACY:          eff.off.accuracy        += (int)add; break;

                case BEF_FIRE_DAMAGE:       eff.elem.fireDamage     += (int)add; break;
                case BEF_ICE_DAMAGE:        eff.elem.iceDamage      += (int)add; break;
                case BEF_POISON_DAMAGE:     eff.elem.poisonDamage   += (int)add; break;

                case BEF_MAX_HP:            eff.maxHp               += (long long)add; break;
                case BEF_DEFENSE:           eff.defense             += (long long)add; break;
                case BEF_DAMAGE_REDUCTION:  eff.def.damageReduction += (int)add; break;
                case BEF_DODGE:             eff.def.dodge           += (int)add; break;
                case BEF_SHIELD:            /* no shield stat yet */ break;
                case BEF_REFLECT:           eff.util.reflectDamage  += (int)add; break;

                case BEF_FIRE_RES:          eff.elem.fireRes        += (int)add; break;
                case BEF_ICE_RES:           eff.elem.iceRes         += (int)add; break;
                case BEF_POISON_RES:        eff.elem.poisonRes      += (int)add; break;

                case BEF_RES_STUN:
                case BEF_RES_SILENCE:       eff.status.resistanceEffect += (int)add; break;

                case BEF_REGEN:             eff.sustain.regen       += (int)add; break;
                case BEF_LIFESTEAL:         eff.sustain.lifeSteal   += (int)add; break;
                case BEF_HEAL_BOOST:        /* no heal boost stat yet */ break;

                case BEF_THORN:             eff.util.reflectDamage  += (int)add; break;
                case BEF_LUCK:              /* no luck stat yet */ break;
                case BEF_INVULNERABLE:      /* unique handling elsewhere */ break;
                case BEF_GRANT_SKILL:       /* handled via skills */ break;
                default: break;
            }
        }
    }
    *out = eff;
}

int addBlessingStacks(Characters* c, const Blessing* b, long long stacks) {
    if (!c || !b || stacks <= 0) return 0;
    // if exists, increase stacks
    for (int i = 0; i < c->blessingCount; ++i) {
        if (c->currentBlessing[i].def && c->currentBlessing[i].def->id == b->id) {
            c->currentBlessing[i].stacks += stacks;
            return 1;
        }
    }
    if (c->blessingCount >= MAX_BLESSING) return 0;
    c->currentBlessing[c->blessingCount].def = b;
    c->currentBlessing[c->blessingCount].stacks = stacks;
    c->blessingCount++;
    return 1;
}

int addBlessing(Characters* c, const Blessing* b) {
    return addBlessingStacks(c, b, 1);
}

int removeBlessingById(Characters* c, int blessingId) {
    if (!c) return 0;
    for (int i = 0; i < c->blessingCount; ++i) {
        if (c->currentBlessing[i].def && c->currentBlessing[i].def->id == blessingId) {
            // shift left
            for (int k = i + 1; k < c->blessingCount; ++k) {
                c->currentBlessing[k - 1] = c->currentBlessing[k];
            }
            c->blessingCount--;
            c->currentBlessing[c->blessingCount].def = NULL;
            c->currentBlessing[c->blessingCount].stacks = 0;
            return 1;
        }
    }
    return 0;
}

void clearBlessings(Characters* c) {
    if (!c) return;
    for (int i = 0; i < c->blessingCount; ++i) {
        c->currentBlessing[i].def = NULL;
        c->currentBlessing[i].stacks = 0;
    }
    c->blessingCount = 0;
}

void renderBaseStats(void* data) {
    const Characters* c = (Characters*)data;

    CombatAttributes eff;
    computeEffectiveAttributes(c, &eff);

    printf(">> PRIMARY STATS\n");
    printf("   HP               : %lld\n", eff.hp);
    printf("   Attack           : %lld\n", eff.attack);
    printf("   Defense          : %lld\n", eff.defense);
    printf("\n");

    printf(">> OFFENSIVE STATS\n");
    printf("   Crit Chance      : %d\n",  eff.off.critChance);
    printf("   Crit Damage      : %d\n",  eff.off.critDamage);
    printf("   Penetration      : %d\n",  eff.off.penetration);
    printf("   Accuracy         : %d\n",  eff.off.accuracy);
    printf("   Damage Boost     : %d\n",  eff.off.damageBoost);
    printf("\n");

    printf(">> DEFENSIVE STATS\n");
    printf("   Dodge            : %d\n", eff.def.dodge);
    printf("   Damage Reduction : %d\n", eff.def.damageReduction);
    printf("\n");

    printf(">> ELEMENTAL STATS\n");
    printf("   Fire Damage      : %d\n", eff.elem.fireDamage);
    printf("   Fire Resistance  : %d\n", eff.elem.fireRes);
    printf("   Ice Damage       : %d\n", eff.elem.iceDamage);
    printf("   Ice Resistance   : %d\n", eff.elem.iceRes);
    printf("   Poison Damage    : %d\n", eff.elem.poisonDamage);
    printf("   Poison Resist    : %d\n", eff.elem.poisonRes);
    printf("\n");

    printf(">> CONTROL & UTILITY STATS\n");
    printf("   Effect Hit Rate  : %d\n", eff.status.effectHitRate);
    printf("   Effect Resist    : %d\n", eff.status.resistanceEffect);
    printf("   Reflect Damage   : %d\n", eff.util.reflectDamage);
}

void renderBlessingData(void* data) {
    const Characters* c = (Characters*)data;
    for (int i = 0; i < c->blessingCount; i++) {
        const ActiveBlessing* ab = &c->currentBlessing[i];
        const Blessing* b = ab->def;
        if (b == NULL) continue;
        printf("- %s (Stacks: %lld)\n", b->name, ab->stacks);
        printf("   Rarity : %s\n", getRarityString(b->rarity));
        printf("   Effects (%d) :\n", b->effectCount);
        for (int j = 0; j < b->effectCount; j++) {
            const BlessingEffect* e = &b->effects[j];
            printf("      [%d] %s : base %.2f  => total %.2f\n", j, getBlessingEffectString(e->type), e->baseValue, e->baseValue * (float)ab->stacks);
        }
        if (b->statusCount > 0) {
            printf("   On-Hit Statuses (%d) :\n", b->statusCount);
            for (int k = 0; k < b->statusCount; ++k) {
                const BlessingStatus* s = &b->statuses[k];
                printf("      [%d] %s  Mag: %.2f  Chance: %d%%\n", k, getStatusTypeString(s->type), s->magnitude, s->chance);
            }
        }
    }
}

void renderStatusData(void* data) {
    const Characters* c = (Characters*)data;
    printf(">> ACTIVE STATUSES\n");
    if (c->statusCount == 0) {
        printf("   (none)\n");
        return;
    }
    for (int i = 0; i < c->statusCount; ++i) {
        const ActiveStatus* s = &c->currentStatus[i];
        printf("   - %s | Amount: %.2f | Remaining: %d\n", getStatusTypeString(s->type), s->amount, s->remaining);
    }
}

void showPlayerStats(Characters* player) {
    Tab tabs[3] = {
        {"Base Stat", renderBaseStats},
        {"Blessing Stat", renderBlessingData},
        {"Status Stat", renderStatusData}
    };

    showTabPanel(tabs, 3, player);
}