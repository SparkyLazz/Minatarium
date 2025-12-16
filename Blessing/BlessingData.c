#include "Blessing.h"

//=====================================
//  EXPANDED BLESSING DATABASE (60+ BLESSINGS)
//=====================================
static Blessing sampleBlessings[] = {
    //========================================
    // COMMON BLESSINGS (8 types)
    //========================================
    {
        .id = 1,
        .name = "Minor Strength",
        .description = "Slightly increases damage",
        .rarity = RARITY_COMMON,
        .stacks = 0,
        .effects = {{DAMAGE_BOOST, 0.08f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 2,
        .name = "Vitality Boost",
        .description = "Increases maximum HP",
        .rarity = RARITY_COMMON,
        .stacks = 0,
        .effects = {{HP_BOOST, 0.15f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 3,
        .name = "Iron Skin",
        .description = "Increases defense",
        .rarity = RARITY_COMMON,
        .stacks = 0,
        .effects = {{DEFENSE_BOOST, 0.05f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 4,
        .name = "Lucky Charm",
        .description = "Increases critical chance",
        .rarity = RARITY_COMMON,
        .stacks = 0,
        .effects = {{CRITICAL_CHANGE, 0.03f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 5,
        .name = "Deadly Precision",
        .description = "Increases critical damage",
        .rarity = RARITY_COMMON,
        .stacks = 0,
        .effects = {{CRITICAL_DAMAGE, 0.15f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 6,
        .name = "Sharp Eye",
        .description = "Increases accuracy",
        .rarity = RARITY_COMMON,
        .stacks = 0,
        .effects = {{ACCURACY_BOOST, 0.05f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 7,
        .name = "Minor Regeneration",
        .description = "Slowly regenerates HP",
        .rarity = RARITY_COMMON,
        .stacks = 0,
        .effects = {{REGEN, 0.02f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 8,
        .name = "Fortune's Favor",
        .description = "Increases luck slightly",
        .rarity = RARITY_COMMON,
        .stacks = 0,
        .effects = {{LUCK, 0.05f}},
        .effectsCount = 1,
        .dotsCount = 0
    },

    //========================================
    // RARE BLESSINGS (19 types)
    // All common types + elemental + special
    //========================================
    // Enhanced common types
    {
        .id = 9,
        .name = "Greater Strength",
        .description = "Significantly increases damage",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{DAMAGE_BOOST, 0.18f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 10,
        .name = "Robust Vitality",
        .description = "Greatly increases maximum HP",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{HP_BOOST, 0.30f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 11,
        .name = "Steel Skin",
        .description = "Greatly increases defense",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{DEFENSE_BOOST, 0.12f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 12,
        .name = "Fatal Strike",
        .description = "Greatly increases critical chance",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{CRITICAL_CHANGE, 0.07f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 13,
        .name = "Devastating Blow",
        .description = "Greatly increases critical damage",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{CRITICAL_DAMAGE, 0.30f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 14,
        .name = "Eagle Eye",
        .description = "Greatly increases accuracy",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{ACCURACY_BOOST, 0.10f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 15,
        .name = "Rapid Regeneration",
        .description = "Significantly regenerates HP",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{REGEN, 0.05f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 16,
        .name = "Fortune's Blessing",
        .description = "Greatly increases luck",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{LUCK, 0.12f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    
    // Elemental damage types
    {
        .id = 17,
        .name = "Flame Touch",
        .description = "Attacks deal fire damage and may burn",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{FIRE_DAMAGE, 0.25f}},
        .effectsCount = 1,
        .dots = {{{BURN, 8.0f, 3}, 35}},
        .dotsCount = 1
    },
    {
        .id = 18,
        .name = "Frost Strike",
        .description = "Attacks deal ice damage and may freeze",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{ICE_DAMAGE, 0.25f}},
        .effectsCount = 1,
        .dots = {{{FREEZE, 0.0f, 2}, 30}},
        .dotsCount = 1
    },
    {
        .id = 19,
        .name = "Toxic Blade",
        .description = "Attacks deal poison damage",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{POISON_DAMAGE, 0.25f}},
        .effectsCount = 1,
        .dots = {{{POISON, 10.0f, 4}, 40}},
        .dotsCount = 1
    },
    
    // Elemental resistances
    {
        .id = 20,
        .name = "Fire Ward",
        .description = "Reduces fire damage taken",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{FIRE_RESISTANCE, 0.15f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 21,
        .name = "Frost Ward",
        .description = "Reduces ice damage taken",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{ICE_RESISTANCE, 0.15f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 22,
        .name = "Poison Ward",
        .description = "Reduces poison damage taken",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{POISON_RESISTANCE, 0.15f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    
    // Special mechanics
    {
        .id = 23,
        .name = "Life Drain",
        .description = "Steal health from enemies",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{LIFESTEAL, 0.08f}},
        .effectsCount = 1,
        .dotsCount = 0
    },
    {
        .id = 24,
        .name = "Thorned Armor",
        .description = "Reflects damage back to attackers",
        .rarity = RARITY_RARE,
        .stacks = 0,
        .effects = {{THORN, 0.15f}},
        .effectsCount = 1,
        .dotsCount = 0
    },

    //========================================
    // EPIC BLESSINGS (20 combinations)
    // Combinations of 2 rare effects
    //========================================
    {
        .id = 25,
        .name = "Berserker's Rage",
        .description = "Massive damage with life drain",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {DAMAGE_BOOST, 0.22f},
            {LIFESTEAL, 0.12f}
        },
        .effectsCount = 2,
        .dotsCount = 0
    },
    {
        .id = 26,
        .name = "Phoenix Heart",
        .description = "Fire damage with regeneration",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {FIRE_DAMAGE, 0.30f},
            {REGEN, 0.08f}
        },
        .effectsCount = 2,
        .dots = {{{BURN, 12.0f, 4}, 45}},
        .dotsCount = 1
    },
    {
        .id = 27,
        .name = "Frozen Fortress",
        .description = "Ice damage with high defense",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {ICE_DAMAGE, 0.30f},
            {DEFENSE_BOOST, 0.15f}
        },
        .effectsCount = 2,
        .dots = {{{FREEZE, 0.0f, 2}, 40}},
        .dotsCount = 1
    },
    {
        .id = 28,
        .name = "Toxic Mastery",
        .description = "Deadly poison with damage boost",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {POISON_DAMAGE, 0.35f},
            {DAMAGE_BOOST, 0.18f}
        },
        .effectsCount = 2,
        .dots = {{{POISON, 15.0f, 5}, 50}},
        .dotsCount = 1
    },
    {
        .id = 29,
        .name = "Critical Master",
        .description = "High crit chance and damage",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {CRITICAL_CHANGE, 0.10f},
            {CRITICAL_DAMAGE, 0.35f}
        },
        .effectsCount = 2,
        .dotsCount = 0
    },
    {
        .id = 30,
        .name = "Titan's Endurance",
        .description = "Massive HP and defense",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {HP_BOOST, 0.35f},
            {DEFENSE_BOOST, 0.18f}
        },
        .effectsCount = 2,
        .dotsCount = 0
    },
    {
        .id = 31,
        .name = "Vampiric Assault",
        .description = "Damage boost with lifesteal",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {DAMAGE_BOOST, 0.22f},
            {LIFESTEAL, 0.10f}
        },
        .effectsCount = 2,
        .dotsCount = 0
    },
    {
        .id = 32,
        .name = "Inferno Shield",
        .description = "Fire damage with fire resistance",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {FIRE_DAMAGE, 0.30f},
            {FIRE_RESISTANCE, 0.20f}
        },
        .effectsCount = 2,
        .dots = {{{BURN, 11.0f, 3}, 40}},
        .dotsCount = 1
    },
    {
        .id = 33,
        .name = "Glacial Armor",
        .description = "Ice resistance with thorns",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {ICE_RESISTANCE, 0.20f},
            {THORN, 0.20f}
        },
        .effectsCount = 2,
        .dotsCount = 0
    },
    {
        .id = 34,
        .name = "Venomous Precision",
        .description = "Poison damage with accuracy",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {POISON_DAMAGE, 0.32f},
            {ACCURACY_BOOST, 0.15f}
        },
        .effectsCount = 2,
        .dots = {{{POISON, 13.0f, 4}, 45}},
        .dotsCount = 1
    },
    {
        .id = 35,
        .name = "Resilient Warrior",
        .description = "HP boost with regeneration",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {HP_BOOST, 0.32f},
            {REGEN, 0.08f}
        },
        .effectsCount = 2,
        .dotsCount = 0
    },
    {
        .id = 36,
        .name = "Assassin's Edge",
        .description = "Critical chance with accuracy",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {CRITICAL_CHANGE, 0.10f},
            {ACCURACY_BOOST, 0.15f}
        },
        .effectsCount = 2,
        .dotsCount = 0
    },
    {
        .id = 37,
        .name = "Elemental Barrier",
        .description = "Fire and ice resistance",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {FIRE_RESISTANCE, 0.18f},
            {ICE_RESISTANCE, 0.18f}
        },
        .effectsCount = 2,
        .dotsCount = 0
    },
    {
        .id = 38,
        .name = "Blazing Assault",
        .description = "Fire and poison damage",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {FIRE_DAMAGE, 0.28f},
            {POISON_DAMAGE, 0.28f}
        },
        .effectsCount = 2,
        .dots = {
            {{BURN, 10.0f, 3}, 40},
            {{POISON, 10.0f, 4}, 40}
        },
        .dotsCount = 2
    },
    {
        .id = 39,
        .name = "Fortified Regeneration",
        .description = "Defense with regeneration",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {DEFENSE_BOOST, 0.15f},
            {REGEN, 0.08f}
        },
        .effectsCount = 2,
        .dotsCount = 0
    },
    {
        .id = 40,
        .name = "Predator's Instinct",
        .description = "Damage and critical damage",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {DAMAGE_BOOST, 0.22f},
            {CRITICAL_DAMAGE, 0.32f}
        },
        .effectsCount = 2,
        .dotsCount = 0
    },
    {
        .id = 41,
        .name = "Frost Retaliation",
        .description = "Ice damage with thorns",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {ICE_DAMAGE, 0.30f},
            {THORN, 0.18f}
        },
        .effectsCount = 2,
        .dots = {{{FREEZE, 0.0f, 2}, 35}},
        .dotsCount = 1
    },
    {
        .id = 42,
        .name = "Unstoppable Force",
        .description = "HP and damage boost",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {HP_BOOST, 0.30f},
            {DAMAGE_BOOST, 0.22f}
        },
        .effectsCount = 2,
        .dotsCount = 0
    },
    {
        .id = 43,
        .name = "Perfect Precision",
        .description = "Accuracy with critical chance",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {ACCURACY_BOOST, 0.15f},
            {CRITICAL_CHANCE, 0.08f}
        },
        .effectsCount = 2,
        .dotsCount = 0
    },
    {
        .id = 44,
        .name = "Reflecting Fortress",
        .description = "Defense with thorns",
        .rarity = RARITY_EPIC,
        .stacks = 0,
        .effects = {
            {DEFENSE_BOOST, 0.18f},
            {THORN, 0.20f}
        },
        .effectsCount = 2,
        .dotsCount = 0
    },

    //========================================
    // LEGENDARY BLESSINGS (16 combinations)
    // Combinations of 3 rare effects with higher values
    //========================================
    {
        .id = 45,
        .name = "Divine Wrath",
        .description = "Overwhelming offensive power",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {DAMAGE_BOOST, 0.35f},
            {CRITICAL_CHANGE, 0.12f},
            {CRITICAL_DAMAGE, 0.45f}
        },
        .effectsCount = 3,
        .dotsCount = 0
    },
    {
        .id = 46,
        .name = "Immortal Soul",
        .description = "Ultimate survival blessing",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {HP_BOOST, 0.50f},
            {DEFENSE_BOOST, 0.25f},
            {REGEN, 0.10f}
        },
        .effectsCount = 3,
        .dotsCount = 0
    },
    {
        .id = 47,
        .name = "Elemental Chaos",
        .description = "Master of all elements",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {FIRE_DAMAGE, 0.35f},
            {ICE_DAMAGE, 0.35f},
            {POISON_DAMAGE, 0.35f}
        },
        .effectsCount = 3,
        .dots = {
            {{BURN, 18.0f, 4}, 60},
            {{FREEZE, 0.0f, 3}, 55},
            {{POISON, 16.0f, 5}, 65}
        },
        .dotsCount = 3
    },
    {
        .id = 48,
        .name = "Vampiric Overlord",
        .description = "Drain life with devastating power",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {DAMAGE_BOOST, 0.32f},
            {LIFESTEAL, 0.15f},
            {CRITICAL_DAMAGE, 0.42f}
        },
        .effectsCount = 3,
        .dotsCount = 0
    },
    {
        .id = 49,
        .name = "Infernal Tyrant",
        .description = "Supreme fire mastery",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {FIRE_DAMAGE, 0.42f},
            {FIRE_RESISTANCE, 0.25f},
            {DAMAGE_BOOST, 0.28f}
        },
        .effectsCount = 3,
        .dots = {{{BURN, 20.0f, 5}, 70}},
        .dotsCount = 1
    },
    {
        .id = 50,
        .name = "Glacial Sovereign",
        .description = "Supreme ice mastery",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {ICE_DAMAGE, 0.42f},
            {ICE_RESISTANCE, 0.25f},
            {DEFENSE_BOOST, 0.22f}
        },
        .effectsCount = 3,
        .dots = {{{FREEZE, 0.0f, 3}, 65}},
        .dotsCount = 1
    },
    {
        .id = 51,
        .name = "Plague Bringer",
        .description = "Supreme poison mastery",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {POISON_DAMAGE, 0.48f},
            {POISON_RESISTANCE, 0.25f},
            {ACCURACY_BOOST, 0.18f}
        },
        .effectsCount = 3,
        .dots = {{{POISON, 24.0f, 6}, 75}},
        .dotsCount = 1
    },
    {
        .id = 52,
        .name = "Perfect Assassin",
        .description = "Flawless precision and lethality",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {CRITICAL_CHANGE, 0.15f},
            {CRITICAL_DAMAGE, 0.50f},
            {ACCURACY_BOOST, 0.20f}
        },
        .effectsCount = 3,
        .dotsCount = 0
    },
    {
        .id = 53,
        .name = "Titan's Legacy",
        .description = "Unbreakable defense and vitality",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {HP_BOOST, 0.55f},
            {DEFENSE_BOOST, 0.28f},
            {THORN, 0.25f}
        },
        .effectsCount = 3,
        .dotsCount = 0
    },
    {
        .id = 54,
        .name = "Eternal Phoenix",
        .description = "Rebirth through fire and healing",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {FIRE_DAMAGE, 0.40f},
            {REGEN, 0.12f},
            {LIFESTEAL, 0.14f}
        },
        .effectsCount = 3,
        .dots = {{{BURN, 19.0f, 5}, 65}},
        .dotsCount = 1
    },
    {
        .id = 55,
        .name = "Elemental Aegis",
        .description = "Protection from all elements",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {FIRE_RESISTANCE, 0.25f},
            {ICE_RESISTANCE, 0.25f},
            {POISON_RESISTANCE, 0.25f}
        },
        .effectsCount = 3,
        .dotsCount = 0
    },
    {
        .id = 56,
        .name = "Apocalypse Bringer",
        .description = "Multi-elemental devastation",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {FIRE_DAMAGE, 0.32f},
            {POISON_DAMAGE, 0.32f},
            {DAMAGE_BOOST, 0.28f}
        },
        .effectsCount = 3,
        .dots = {
            {{BURN, 15.0f, 4}, 55},
            {{POISON, 18.0f, 5}, 60}
        },
        .dotsCount = 2
    },
    {
        .id = 57,
        .name = "Warlord's Dominance",
        .description = "Complete combat mastery",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {DAMAGE_BOOST, 0.35f},
            {HP_BOOST, 0.45f},
            {LIFESTEAL, 0.15f}
        },
        .effectsCount = 3,
        .dotsCount = 0
    },
    {
        .id = 58,
        .name = "Frost Apocalypse",
        .description = "Freezing devastation",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {ICE_DAMAGE, 0.45f},
            {CRITICAL_CHANCE, 0.12f},
            {ACCURACY_BOOST, 0.18f}
        },
        .effectsCount = 3,
        .dots = {{{FREEZE, 0.0f, 3}, 70}},
        .dotsCount = 1
    },
    {
        .id = 59,
        .name = "Berserker King",
        .description = "Unstoppable offensive fury",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {DAMAGE_BOOST, 0.38f},
            {CRITICAL_DAMAGE, 0.48f},
            {LIFESTEAL, 0.16f}
        },
        .effectsCount = 3,
        .dotsCount = 0
    },
    {
        .id = 60,
        .name = "Guardian Colossus",
        .description = "Impenetrable fortress",
        .rarity = RARITY_LEGENDARY,
        .stacks = 0,
        .effects = {
            {HP_BOOST, 0.55f},
            {DEFENSE_BOOST, 0.30f},
            {REGEN, 0.12f}
        },
        .effectsCount = 3,
        .dotsCount = 0
    }
};

static BlessingDatabase globalDB = {
    .blessings = sampleBlessings,
    .count = sizeof(sampleBlessings) / sizeof(sampleBlessings[0])
};

BlessingDatabase* GetBlessingDatabase() {
    return &globalDB;
}