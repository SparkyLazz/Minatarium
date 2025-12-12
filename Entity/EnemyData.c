#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "EnemyData.h"
#include "../Blessing/Blessing.h"

// Simple unique boss skills
static void skill_inferno(CharacterSkill* self, CharacterSkill* target) {
    if (!self || !target) return;
    long long dmg = self->attributes.attack + (self->attributes.attack / 2);
    if (target->attributes.hp > dmg) target->attributes.hp -= dmg; else target->attributes.hp = 0;
}

static void skill_ice_prison(CharacterSkill* self, CharacterSkill* target) {
    if (!self || !target) return;
    // Apply freeze for 2 turns
    if (target->statusCount < MAX_STATUS) {
        target->currentStatus[target->statusCount].type = STATUS_FREEZE;
        target->currentStatus[target->statusCount].amount = 0.f;
        target->currentStatus[target->statusCount].remaining = 2;
        target->statusCount++;
    }
}

static void skill_plague_cloud(CharacterSkill* self, CharacterSkill* target) {
    if (!self || !target) return;
    // Poison for 4 turns, amount based on attack
    if (target->statusCount < MAX_STATUS) {
        target->currentStatus[target->statusCount].type = STATUS_POISON;
        target->currentStatus[target->statusCount].amount = (float)(self->attributes.attack / 4);
        target->currentStatus[target->statusCount].remaining = 4;
        target->statusCount++;
    }
}

static BlessingSkill BOSS_SKILL_INFERNO = { "Inferno", skill_inferno, 3, 0 };
static BlessingSkill BOSS_SKILL_ICE_PRISON = { "Ice Prison", skill_ice_prison, 4, 0 };
static BlessingSkill BOSS_SKILL_PLAGUE = { "Plague Cloud", skill_plague_cloud, 3, 0 };

// Enemy blueprints storage
static Characters normals[20];
static Characters elites[15];
static Characters bosses[15];
static int initialized = 0;

static void base_from_player(Characters* dst, const char* name, CharacterType t) {
    extern const Characters playerBluePrint;
    *dst = playerBluePrint;
    strncpy(dst->name, name, sizeof(dst->name)-1);
    dst->type = t;
    dst->blessingCount = 0;
    dst->statusCount = 0;
    dst->skillCount = 0;
}

static void add_random_blessing(Characters* c, BlessingRarity r, int stacks) {
    const Blessing* pool[128];
    int n = blessing_collect_by_rarity(r, pool, 128);
    if (n <= 0) return;
    const Blessing* b = pool[rand() % n];
    if (c->blessingCount < MAX_BLESSING) {
        c->currentBlessing[c->blessingCount].def = b;
        c->currentBlessing[c->blessingCount].stacks = stacks;
        c->blessingCount++;
    }
}

void enemy_registry_init() {
    if (initialized) return;
    initialized = 1;
    blessing_init_registry();

    // Generate normals
    for (int i = 0; i < 20; ++i) {
        char name[100]; snprintf(name, sizeof(name), "%s %d",
            (i % 4 == 0) ? "Goblin" : (i % 4 == 1) ? "Wolf" : (i % 4 == 2) ? "Bandit" : "Imp", i+1);
        base_from_player(&normals[i], name, CHARACTER_NORMAL);
        normals[i].attributes.maxHp = 60 + i * 8;
        normals[i].attributes.hp = normals[i].attributes.maxHp;
        normals[i].attributes.attack = 8 + i * 2;
        normals[i].attributes.defense = 4 + i;
        // 1-2 common blessings
        add_random_blessing(&normals[i], RARITY_COMMON, 1);
        if (i % 3 == 0) add_random_blessing(&normals[i], RARITY_COMMON, 1);
    }

    // Generate elites
    for (int i = 0; i < 15; ++i) {
        char name[100]; snprintf(name, sizeof(name), "%s %d",
            (i % 3 == 0) ? "Wraith" : (i % 3 == 1) ? "Bruiser" : "Assassin", i+1);
        base_from_player(&elites[i], name, CHARACTER_ELITE);
        elites[i].attributes.maxHp = 150 + i * 15;
        elites[i].attributes.hp = elites[i].attributes.maxHp;
        elites[i].attributes.attack = 18 + i * 3;
        elites[i].attributes.defense = 8 + i * 2;
        // Rare leaning
        add_random_blessing(&elites[i], RARITY_RARE, 1);
        if (i % 2 == 0) add_random_blessing(&elites[i], RARITY_EPIC, 1);
    }

    // Generate bosses
    for (int i = 0; i < 15; ++i) {
        char name[100]; snprintf(name, sizeof(name), "%s %d",
            (i % 3 == 0) ? "Infernal Knight" : (i % 3 == 1) ? "Frost Troll" : "Plague Reaper", i+1);
        base_from_player(&bosses[i], name, CHARACTER_BOSS);
        bosses[i].attributes.maxHp = 300 + i * 30;
        bosses[i].attributes.hp = bosses[i].attributes.maxHp;
        bosses[i].attributes.attack = 28 + i * 5;
        bosses[i].attributes.defense = 15 + i * 3;
        // Epic with chance legendary
        add_random_blessing(&bosses[i], RARITY_EPIC, 1);
        if (i % 3 == 0) add_random_blessing(&bosses[i], RARITY_LEGENDARY, 1);
        // Unique skill
        if (bosses[i].skillCount < MAX_SKILLS) bosses[i].currentSkill[bosses[i].skillCount++] = &BOSS_SKILL_INFERNO;
        if (i % 3 == 1 && bosses[i].skillCount < MAX_SKILLS) bosses[i].currentSkill[bosses[i].skillCount++] = &BOSS_SKILL_ICE_PRISON;
        if (i % 3 == 2 && bosses[i].skillCount < MAX_SKILLS) bosses[i].currentSkill[bosses[i].skillCount++] = &BOSS_SKILL_PLAGUE;
    }
}

const Characters* enemy_pick_blueprint(CharacterType t) {
    if (!initialized) enemy_registry_init();
    if (t == CHARACTER_NORMAL) {
        return &normals[rand() % 20];
    } else if (t == CHARACTER_ELITE) {
        return &elites[rand() % 15];
    } else {
        return &bosses[rand() % 15];
    }
}
