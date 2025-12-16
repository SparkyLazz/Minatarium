#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Character.h"
#include "../Utils/Utils.h"

#define LABEL_WIDTH 14
#define MAX_CRIT_CHANCE 50  // Hard cap

//=====================================
//  STAT MANAGEMENT (CORE LOGIC)
//=====================================
void RecalculateStats(Character* character) {
    // HP calculation
    character->attribute.maxHP = character->attribute.base.hp + character->attribute.bonus.hp;

    // Clamp current HP to max
    if (character->attribute.currentHP > character->attribute.maxHP) {
        character->attribute.currentHP = character->attribute.maxHP;
    }

    // Current stats = base + bonus
    character->attribute.current.hp = character->attribute.maxHP;
    character->attribute.current.attack = character->attribute.base.attack + character->attribute.bonus.attack;
    character->attribute.current.defense = character->attribute.base.defense + character->attribute.bonus.defense;

    // Percentage stats with capping
    character->attribute.current.criticalChance = character->attribute.base.criticalChance + character->attribute.bonus.criticalChance;
    if (character->attribute.current.criticalChance > MAX_CRIT_CHANCE) {
        character->attribute.current.criticalChance = MAX_CRIT_CHANCE;
    }
    if (character->attribute.current.criticalChance < 0) {
        character->attribute.current.criticalChance = 0;
    }

    character->attribute.current.criticalDamage = character->attribute.base.criticalDamage + character->attribute.bonus.criticalDamage;
    character->attribute.current.damageBoost = character->attribute.base.damageBoost + character->attribute.bonus.damageBoost;

    character->attribute.current.accuracy = character->attribute.base.accuracy + character->attribute.bonus.accuracy;
    if (character->attribute.current.accuracy > 100) character->attribute.current.accuracy = 100;
    if (character->attribute.current.accuracy < 0) character->attribute.current.accuracy = 0;

    // Resistance stats (capped at 75%)
    character->attribute.current.fireResistance = character->attribute.base.fireResistance + character->attribute.bonus.fireResistance;
    if (character->attribute.current.fireResistance > 75) character->attribute.current.fireResistance = 75;

    character->attribute.current.iceResistance = character->attribute.base.iceResistance + character->attribute.bonus.iceResistance;
    if (character->attribute.current.iceResistance > 75) character->attribute.current.iceResistance = 75;

    character->attribute.current.poisonResistance = character->attribute.base.poisonResistance + character->attribute.bonus.poisonResistance;
    if (character->attribute.current.poisonResistance > 75) character->attribute.current.poisonResistance = 75;

    // Sustain stats
    character->attribute.current.lifeSteal = character->attribute.base.lifeSteal + character->attribute.bonus.lifeSteal;
    character->attribute.current.regen = character->attribute.base.regen + character->attribute.bonus.regen;
}

void InitializeCharacterStats(Character* character, StatBlock baseStats, long long baseHP) {
    // Set base stats (permanent)
    character->attribute.base = baseStats;
    character->attribute.base.hp = baseHP;

    // Zero out bonus stats
    memset(&character->attribute.bonus, 0, sizeof(StatBlock));

    // Initialize HP
    character->attribute.maxHP = baseHP;
    character->attribute.currentHP = baseHP;

    // Recalculate current stats
    RecalculateStats(character);

    // Clear combat state
    ClearCombatState(character);
}

//=====================================
//  COMBAT STATE MANAGEMENT
//=====================================
void SetDefendingStance(Character* character) {
    character->combatState.isDefending = 1;
    character->combatState.damageReduction = 50;  // 50% damage reduction
    character->combatState.reflectChance = 0;      // Optional: could add reflect
}

void ClearCombatState(Character* character) {
    character->combatState.isDefending = 0;
    character->combatState.damageReduction = 0;
    character->combatState.reflectChance = 0;
}

//=====================================
//  CHARACTER PROPERTY
//=====================================
Character playerBluePrint;  // Initialized in GeneratePlayer

void CharacterAddBlessing(Character* target, const Blessing* blessing) {
    // Check if blessing already exists
    int existingIndex = -1;
    for (int i = 0; i < target->blessingCount; i++) {
        if (target->currentBlessing[i].id == blessing->id) {
            existingIndex = i;
            break;
        }
    }

    if (existingIndex >= 0) {
        // Blessing exists, increment stacks and reapply
        target->currentBlessing[existingIndex].stacks += 1;
    } else {
        // New blessing
        if (target->blessingCount < 100) {
            target->currentBlessing[target->blessingCount] = *blessing;
            target->currentBlessing[target->blessingCount].stacks = 1;
            target->blessingCount++;
        }
    }

    // Apply blessing effects to BONUS stats
    for (int i = 0; i < blessing->effectsCount; i++) {
        float effectValue = blessing->effects[i].baseValue;

        switch (blessing->effects[i].type) {
            case DAMAGE_BOOST:
                target->attribute.bonus.damageBoost += (int)(effectValue * 100.0f);
                break;
            case CRITICAL_CHANGE:
                target->attribute.bonus.criticalChance += (int)(effectValue * 100.0f);
                break;
            case CRITICAL_DAMAGE:
                target->attribute.bonus.criticalDamage += (int)(effectValue * 100.0f);
                break;
            case HP_BOOST:
                target->attribute.bonus.hp += (long long)(target->attribute.base.hp * effectValue);
                break;
            case DEFENSE_BOOST:
                target->attribute.bonus.defense += (long long)(target->attribute.base.defense * effectValue);
                break;
            case ACCURACY_BOOST:
                target->attribute.bonus.accuracy += (int)(effectValue * 100.0f);
                break;
            case LIFESTEAL:
                target->attribute.bonus.lifeSteal += (int)(effectValue * 100.0f);
                break;
            case REGEN:
                target->attribute.bonus.regen += (int)(effectValue * 100.0f);
                break;
            default:
                break;
        }
    }

    // ALWAYS recalculate after blessing change
    RecalculateStats(target);
}

void CharacterAddStatus(Character* character, const Status* status) {
    for (int i = 0; i < character->statusCount; i++) {
        if (character->currentStatus[i].type == status->type) {
            character->currentStatus[i].duration += 1;
            return;
        }
    }

    if (character->statusCount < 100) {
        character->currentStatus[character->statusCount] = *status;
        character->statusCount++;
    }
}

//=====================================
//  CHARACTER RENDERER
//=====================================
void CharacterStatsTab(void* data) {
    const Character* character = (Character*)data;
    printColor(COL_BOLD, "Primary Stats\n");
    printf("   HP                : %lld / %lld\n", character->attribute.currentHP, character->attribute.maxHP);
    printf("   Attack            : %lld (base: %lld, bonus: %lld)\n",
           character->attribute.current.attack,
           character->attribute.base.attack,
           character->attribute.bonus.attack);
    printf("   Defense           : %lld (base: %lld, bonus: %lld)\n",
           character->attribute.current.defense,
           character->attribute.base.defense,
           character->attribute.bonus.defense);
    printf("\n");

    printColor(COL_BOLD, "Offensive Stats\n");
    printf("   Crit Chance       : %d%% (capped at %d%%)\n", character->attribute.current.criticalChance, MAX_CRIT_CHANCE);
    printf("   Crit Damage       : %d%%\n", character->attribute.current.criticalDamage);
    printf("   Accuracy          : %d%%\n", character->attribute.current.accuracy);
    printf("   Damage Boost      : %d%%\n", character->attribute.current.damageBoost);
    printf("\n");

    printColor(COL_BOLD, "Elemental Stats\n");
    printf("   Fire Resistance   : %d%%\n", character->attribute.current.fireResistance);
    printf("   Ice Resistance    : %d%%\n", character->attribute.current.iceResistance);
    printf("   Poison Resistance : %d%%\n", character->attribute.current.poisonResistance);
    printf("\n");

    printColor(COL_BOLD, "Unique Stats\n");
    printf("   Life Steal        : %d%%\n", character->attribute.current.lifeSteal);
    printf("   Regen             : %d%%\n", character->attribute.current.regen);
}

void RarityColor(const BlessingRarity rarity) {
    switch (rarity) {
        case RARITY_COMMON: printColor(COL_GREEN, "Common"); break;
        case RARITY_RARE: printColor(COL_CYAN, "Rare"); break;
        case RARITY_EPIC: printColor(COL_MAGENTA, "Epic"); break;
        case RARITY_LEGENDARY: printColor(COL_YELLOW, "Legendary"); break;
    }
}

const char* BlessingEffectString(const BlessingEffectType e) {
    switch (e) {
        case DAMAGE_BOOST: return "Damage Boost";
        case CRITICAL_CHANGE: return "Critical Chance";
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
    if (stacks <= 10) printColor(COL_WHITE, "%lld", stacks);
    else if (stacks <= 30) printColor(COL_GREEN, "%lld", stacks);
    else if (stacks <= 50) printColor(COL_CYAN, "%lld", stacks);
    else if (stacks <= 75) printColor(COL_MAGENTA, "%lld", stacks);
    else if (stacks <= 100) printColor(COL_YELLOW, "%lld", stacks);
    else printColor(COL_BRIGHT_RED, "%lld", stacks);
}

void BlessingTotalValueColor(const float totalPercent) {
    if (totalPercent < 25.0f) printColor(COL_WHITE, "%.2f%%\n", totalPercent);
    else if (totalPercent < 75.0f) printColor(COL_GREEN, "%.2f%%\n", totalPercent);
    else if (totalPercent < 150.0f) printColor(COL_CYAN, "%.2f%%\n", totalPercent);
    else if (totalPercent < 300.0f) printColor(COL_MAGENTA, "%.2f%%\n", totalPercent);
    else printColor(COL_YELLOW, "%.2f%%\n", totalPercent);
}

void CharacterBlessingTab(void* data) {
    const Character* character = (Character*)data;
    for (int i = 0; i < character->blessingCount; i++) {
        printColor(COL_BOLD, "[%d] %s\n", i + 1, character->currentBlessing[i].name);
        printf("    %-*s : ", LABEL_WIDTH, "Rarity");
        RarityColor(character->currentBlessing[i].rarity);
        printf("\n");
        printf("    %-*s : ", LABEL_WIDTH, "Stacks");
        BlessingStackColor(character->currentBlessing[i].stacks);
        printf("\n\n");
        printf("    %-*s : %d\n", LABEL_WIDTH, "Effects", character->currentBlessing[i].effectsCount);

        for (int j = 0; j < character->currentBlessing[i].effectsCount; j++) {
            printf("        %-*s : %s\n", LABEL_WIDTH, "Type",
                   BlessingEffectString(character->currentBlessing[i].effects[j].type));
            printf("        %-*s : %.2f%%\n", LABEL_WIDTH, "Base Value",
                   character->currentBlessing[i].effects[j].baseValue * 100.0f);
            printf("        %-*s : ", LABEL_WIDTH, "Total Value");
            BlessingTotalValueColor(character->currentBlessing[i].effects[j].baseValue *
                                   (float)character->currentBlessing[i].stacks * 100.0f);
            printf("\n");
        }

        if (character->currentBlessing[i].dotsCount > 0) {
            printf("\n    %-*s :\n", LABEL_WIDTH, "On-Hit Statuses");
            for (int j = 0; j < character->currentBlessing[i].dotsCount; j++) {
                printf("        %-*s : %s\n", LABEL_WIDTH, "Type",
                       StatusEffectString(character->currentBlessing[i].dots[j]));
                printf("        %-*s : %d\n", LABEL_WIDTH, "Duration",
                       character->currentBlessing[i].dots[j].DoT.duration);
                printf("        %-*s : %.2f\n", LABEL_WIDTH, "Base Value",
                       character->currentBlessing[i].dots[j].DoT.baseAmount);
                printf("\n");
            }
        }
        printf("\n");
    }
}

void CharacterRenderer(Character* character) {
    Tab tabs[2] = {
        {"Attribute Stat", CharacterStatsTab},
        {"Blessing Stat", CharacterBlessingTab}
    };
    showTabPanel(tabs, 2, character);
}

//=====================================
//  CHARACTER GENERATOR
//=====================================
// ReSharper disable once CppDFAConstantParameter
static long long ScaleHP(const long long base, const int floor, const float multiplier) {
    return (long long)((double)base * pow(1.15, floor * 0.8) * (1.0 + floor * 0.3) * multiplier);
}

static long long ScaleLinearStat(const long long base, const int floor, const float multiplier) {
    return (long long)((double)base * pow(1.12, floor * 0.7) + floor * 2.5 * multiplier);
}

static int ScalePercentage(const int base, const int floor, const float multiplier, const int cap) {
    const int scaled = base + (int)(log2(floor + 1) * 5 * multiplier);
    return (scaled > cap) ? cap : scaled;
}

static int CalculateBlessingCount(const int floor, const CharacterType type) {
    int baseCount = 0;
    switch(type) {
        case NORMAL: baseCount = (floor / 5); break;
        case ELITE: baseCount = 1 + (floor / 4); break;
        case BOSS: baseCount = 2 + (floor / 3); break;
        default: baseCount = 0;
    }
    return (baseCount > 15) ? 15 : baseCount;
}

static long long CalculateBlessingStacks(const int floor, const CharacterType type) {
    long long stacks;
    switch(type) {
        case NORMAL: stacks = 1 + (floor / 4); break;
        case ELITE: stacks = 1 + (floor / 3); break;
        case BOSS: stacks = 1 + (floor / 2); break;
        default: stacks = 1;
    }
    return stacks;
}

CharacterType DetermineEnemyType(const int floor) {
    if (floor % 10 == 0) return BOSS;
    if (floor % 7 == 0 || floor % 8 == 0 || floor % 9 == 0) return ELITE;
    return NORMAL;
}

void InitRandomGenerator() {
    static int initialized = 0;
    if (!initialized) {
        srand((unsigned int)time(NULL));
        initialized = 1;
    }
}

Character GenerateEnemy(const int floor) {
    InitRandomGenerator();

    Character enemy = {0};

    const CharacterType type = DetermineEnemyType(floor);
    enemy.type = type;

    // Base stats
    const long long baseHP = 80;
    const long long baseAttack = 8;
    const long long baseDefense = 3;

    // Type multipliers
    // ReSharper disable once CppDFAUnusedValue
    float hpMult = 1.0f;
    // ReSharper disable once CppDFAUnusedValue
    float atkMult = 1.0f;
    // ReSharper disable once CppDFAUnusedValue
    float defMult = 1.0f;

    switch(type) {
        case NORMAL:
            strcpy(enemy.name, "Normal Enemy");
            hpMult = 1.0f; atkMult = 1.0f; defMult = 1.0f;
            break;
        case ELITE:
            strcpy(enemy.name, "Elite Enemy");
            hpMult = 1.8f; atkMult = 1.4f; defMult = 1.3f;
            break;
        case BOSS:
            strcpy(enemy.name, "Boss Enemy");
            hpMult = 3.0f; atkMult = 1.8f; defMult = 1.6f;
            break;
        // ReSharper disable once CppDFAUnreachableCode
        default:
            strcpy(enemy.name, "Enemy");
    }

    // Setup base stats
    StatBlock baseStats = {0};
    baseStats.attack = ScaleLinearStat(baseAttack, floor, atkMult);
    baseStats.defense = ScaleLinearStat(baseDefense, floor, defMult);
    baseStats.criticalChance = ScalePercentage(5, floor, atkMult, MAX_CRIT_CHANCE);
    baseStats.criticalDamage = ScalePercentage(50, floor, atkMult, 300);
    baseStats.damageBoost = ScalePercentage(0, floor, atkMult, 200);
    baseStats.accuracy = ScalePercentage(70, floor, 1.0f, 100);
    baseStats.fireResistance = ScalePercentage(0, floor, 0.5f, 50);
    baseStats.iceResistance = ScalePercentage(0, floor, 0.5f, 50);
    baseStats.poisonResistance = ScalePercentage(0, floor, 0.5f, 50);
    baseStats.lifeSteal = ScalePercentage(0, floor, 0.3f, 30);
    baseStats.regen = ScalePercentage(0, floor, 0.05f, 20);

    long long scaledHP = ScaleHP(baseHP, floor, hpMult);
    InitializeCharacterStats(&enemy, baseStats, scaledHP);

    // Generate blessings
    int blessingCount = CalculateBlessingCount(floor, type);
    const BlessingDatabase* db = GetBlessingDatabase();

    // Boss gets 1 legendary
    int startIndex = 0;
    if (type == BOSS) {
        for (int i = 0; i < db->count; i++) {
            if (db->blessings[i].rarity == RARITY_LEGENDARY) {
                Blessing legendary = db->blessings[i];
                legendary.stacks = CalculateBlessingStacks(floor, type);
                CharacterAddBlessing(&enemy, &legendary);
                startIndex = 1;
                break;
            }
        }
    }

    // Add remaining blessings
    for (int i = startIndex; i < blessingCount && enemy.blessingCount < 100; i++) {
        int randomIndex;
        do {
            randomIndex = rand() % db->count;
        } while (db->blessings[randomIndex].rarity == RARITY_LEGENDARY);

        Blessing blessing = db->blessings[randomIndex];
        blessing.stacks = CalculateBlessingStacks(floor, type);
        CharacterAddBlessing(&enemy, &blessing);
    }

    enemy.statusCount = 0;
    return enemy;
}

Character GeneratePlayer(const char* playerName) {
    Character player = {0};

    player.type = PLAYER;

    if (playerName != NULL && strlen(playerName) > 0) {
        strncpy(player.name, playerName, sizeof(player.name) - 1);
        player.name[sizeof(player.name) - 1] = '\0';
    } else {
        strcpy(player.name, "Player");
    }

    // Setup base stats
    StatBlock baseStats = {0};
    baseStats.attack = 50;
    baseStats.defense = 50;
    baseStats.criticalChance = 20;
    baseStats.criticalDamage = 80;
    baseStats.damageBoost = 0;
    baseStats.accuracy = 75;
    baseStats.fireResistance = 0;
    baseStats.iceResistance = 0;
    baseStats.poisonResistance = 0;
    baseStats.lifeSteal = 40;
    baseStats.regen = 40;

    InitializeCharacterStats(&player, baseStats, 1000);

    return player;
}