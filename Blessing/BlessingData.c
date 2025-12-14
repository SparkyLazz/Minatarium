#include "Blessing.h"
//=====================================
//  SAMPLE BLESSING DATABASE
//=====================================
static Blessing sampleBlessings[] = {
    // COMMON BLESSINGS (0-4)
    {
        .id = 1,
        .name = "Minor Strength",
        .description = "Increases attack slightly",
        .rarity = RARITY_COMMON,
        .stacks = 0,
        .effects = {{DAMAGE_BOOST, 0.05f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 2,
        .name = "Iron Skin",
        .description = "Increases defense",
        .rarity = RARITY_COMMON,
        .stacks = 0,
        .effects = {{DEFENSE_BOOST, 0.05f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 3,
        .name = "Vitality",
        .description = "Increases maximum HP",
        .rarity = RARITY_COMMON,
        .stacks = 0,
        .effects = {{HP_BOOST, 0.10f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 4,
        .name = "Sharp Eye",
        .description = "Increases accuracy",
        .rarity = RARITY_COMMON,
        .stacks = 0,
        .effects = {{ACCURACY_BOOST, 0.05f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 5,
        .name = "Lucky Charm",
        .description = "Slightly increases critical chance",
        .rarity = RARITY_COMMON,
        .stacks = 0,
        .effects = {{CRITICAL_CHANGE, 0.03f}},
        .effectsCount = 1,
        .dotsCount = 0
    },

    // RARE BLESSINGS (5-9)
    {
        .id = 6,
        .name = "Warrior's Might",
        .description = "Significantly boosts damage",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{DAMAGE_BOOST, 0.12f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 7,
        .name = "Flame Touch",
        .description = "Attacks have a chance to burn enemies",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{FIRE_DAMAGE, 0.15f}},
        .effectsCount = 1,
        .dots = {{{BURN, 5.0f, 3}, 35}},
        .dotsCount = 1
    },
    {
        .id = 8,
        .name = "Frost Strike",
        .description = "Attacks have a chance to freeze enemies",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{ICE_DAMAGE, 0.15f}},
        .effectsCount = 1,
        .dots = {{{FREEZE, 0.0f, 2}, 30}},
        .dotsCount = 1
    },
    {
        .id = 9,
        .name = "Life Drain",
        .description = "Steal health from enemies",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{LIFESTEAL, 0.08f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 10,
        .name = "Critical Focus",
        .description = "Increases critical chance and damage",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{CRITICAL_CHANGE, 0.08f}, {CRITICAL_DAMAGE, 0.15f}},
        .effectsCount = 2,
        .dotsCount = 0
    },

    // EPIC BLESSINGS (10-13)
    {
        .id = 11,
        .name = "Berserker's Rage",
        .description = "Massive damage boost with life steal",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {{DAMAGE_BOOST, 0.20f}, {LIFESTEAL, 0.12f}},
        .effectsCount = 2,
        .dotsCount = 0
    },
    {
        .id = 12,
        .name = "Phoenix Blessing",
        .description = "Powerful fire damage and regeneration",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {{FIRE_DAMAGE, 0.25f}, {REGEN, 0.05f}},
        .effectsCount = 2,
        .dots = {{{BURN, 10.0f, 4}, 50}},
        .dotsCount = 1
    },
    {
        .id = 13,
        .name = "Toxic Mastery",
        .description = "Deadly poison damage over time",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {{POISON_DAMAGE, 0.30f}},
        .effectsCount = 1,
        .dots = {{{POISON, 8.0f, 5}, 60}},
        .dotsCount = 1
    },
    {
        .id = 14,
        .name = "Fortress Shield",
        .description = "Massive defense and health boost",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {{HP_BOOST, 0.25f}, {DEFENSE_BOOST, 0.20f}},
        .effectsCount = 2,
        .dotsCount = 0
    },

    // LEGENDARY BLESSINGS (14-16)
    {
        .id = 15,
        .name = "Divine Wrath",
        .description = "Overwhelming power in all aspects",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {DAMAGE_BOOST, 0.35f},
            {CRITICAL_CHANGE, 0.15f},
            {CRITICAL_DAMAGE, 0.30f},
            {ARMOR_PENETRATION, 0.25f}
        },
        .effectsCount = 4,
        .dotsCount = 0
    },
    {
        .id = 16,
        .name = "Immortal Soul",
        .description = "Ultimate survival blessing",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {HP_BOOST, 0.50f},
            {DEFENSE_BOOST, 0.30f},
            {REGEN, 0.10f},
            {LIFESTEAL, 0.15f}
        },
        .effectsCount = 4,
        .dotsCount = 0
    },
    {
        .id = 17,
        .name = "Elemental Chaos",
        .description = "Master of all elements",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {FIRE_DAMAGE, 0.30f},
            {ICE_DAMAGE, 0.30f},
            {POISON_DAMAGE, 0.30f}
        },
        .effectsCount = 3,
        .dots = {
            {{BURN, 15.0f, 5}, 70},
            {{FREEZE, 0.0f, 3}, 70},
            {{POISON, 12.0f, 6}, 70}
        },
        .dotsCount = 3
    }
};
static BlessingDatabase globalDB = {
    .blessings = sampleBlessings,
    .count = sizeof(sampleBlessings) / sizeof(sampleBlessings[0])
};

BlessingDatabase* GetBlessingDatabase() {
    return &globalDB;
}