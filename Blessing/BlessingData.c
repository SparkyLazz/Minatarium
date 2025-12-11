#include "Blessing.h"

// ======================================
//  BLESSING DATABASE - 80 BLESSINGS
//  20 Common, 20 Rare, 20 Epic, 20 Legendary
// ======================================

Blessing BLESSING_DATA[80] = {

    // ==========================================
    // COMMON BLESSINGS (0-19)
    // ==========================================

    // 0
    {
        .id = 0,
        .name = "Minor Strength",
        .description = "Slightly increases attack power",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_DAMAGE_BOOST, 5.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 1
    {
        .id = 1,
        .name = "Tough Skin",
        .description = "Grants minor defense bonus",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_DEFENSE, 10.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 2
    {
        .id = 2,
        .name = "Vitality",
        .description = "Increases maximum health",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_MAX_HP, 50.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 3
    {
        .id = 3,
        .name = "Sharp Eye",
        .description = "Improves accuracy",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_ACCURACY, 3.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 4
    {
        .id = 4,
        .name = "Quick Feet",
        .description = "Slightly increases dodge chance",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_DODGE, 2.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 5
    {
        .id = 5,
        .name = "Regeneration",
        .description = "Slowly restores health",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_REGEN, 2.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 6
    {
        .id = 6,
        .name = "Critical Strike",
        .description = "Increases critical hit chance",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_CRIT_CHANCE, 3.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 7
    {
        .id = 7,
        .name = "Fire Touch",
        .description = "Adds fire damage to attacks",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_FIRE_DAMAGE, 5.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 8
    {
        .id = 8,
        .name = "Frost Affinity",
        .description = "Adds ice damage to attacks",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_ICE_DAMAGE, 5.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 9
    {
        .id = 9,
        .name = "Toxic Blade",
        .description = "Adds poison damage to attacks",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_POISON_DAMAGE, 5.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 10
    {
        .id = 10,
        .name = "Flame Guard",
        .description = "Resistance to fire damage",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_FIRE_RES, 5.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 11
    {
        .id = 11,
        .name = "Frost Guard",
        .description = "Resistance to ice damage",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_ICE_RES, 5.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 12
    {
        .id = 12,
        .name = "Antidote",
        .description = "Resistance to poison damage",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_POISON_RES, 5.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 13
    {
        .id = 13,
        .name = "Fortified Mind",
        .description = "Reduces stun duration",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_RES_STUN, 10.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 14
    {
        .id = 14,
        .name = "Clear Voice",
        .description = "Reduces silence duration",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_RES_SILENCE, 10.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 15
    {
        .id = 15,
        .name = "Lucky Charm",
        .description = "Slightly increases luck",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_LUCK, 2.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 16
    {
        .id = 16,
        .name = "Vampire Touch",
        .description = "Steal life from enemies",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_LIFESTEAL, 2.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 17
    {
        .id = 17,
        .name = "Armor Break",
        .description = "Penetrates enemy armor",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_ARMOR_PEN, 5.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 18
    {
        .id = 18,
        .name = "Thorny Hide",
        .description = "Reflects damage to attackers",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_THORN, 3.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // 19
    {
        .id = 19,
        .name = "Iron Will",
        .description = "Reduces incoming damage",
        .rarity = RARITY_COMMON,
        .effects = {
            {BEF_DAMAGE_REDUCTION, 3.0f, 1}
        },
        .effectCount = 1,
        .statusCount = 0,
        .skill = {0}
    },

    // ==========================================
    // RARE BLESSINGS (20-39)
    // ==========================================

    // 20
    {
        .id = 20,
        .name = "Warrior's Might",
        .description = "Significant attack and HP boost",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_DAMAGE_BOOST, 12.0f, 1},
            {BEF_MAX_HP, 100.0f, 1}
        },
        .effectCount = 2,
        .statusCount = 0,
        .skill = {0}
    },

    // 21
    {
        .id = 21,
        .name = "Blazing Inferno",
        .description = "Powerful fire damage with burn effect",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_FIRE_DAMAGE, 15.0f, 1}
        },
        .effectCount = 1,
        .statuses = {
            {STATUS_BURN, 8.0f, 25}
        },
        .statusCount = 1,
        .skill = {0}
    },

    // 22
    {
        .id = 22,
        .name = "Frozen Heart",
        .description = "Ice damage with freeze chance",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_ICE_DAMAGE, 15.0f, 1}
        },
        .effectCount = 1,
        .statuses = {
            {STATUS_FREEZE, 0.0f, 15}
        },
        .statusCount = 1,
        .skill = {0}
    },

    // 23
    {
        .id = 23,
        .name = "Deadly Venom",
        .description = "Poison damage with strong DoT",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_POISON_DAMAGE, 15.0f, 1}
        },
        .effectCount = 1,
        .statuses = {
            {STATUS_POISON, 10.0f, 30}
        },
        .statusCount = 1,
        .skill = {0}
    },

    // 24
    {
        .id = 24,
        .name = "Assassin's Edge",
        .description = "High crit chance and damage",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_CRIT_CHANCE, 8.0f, 1},
            {BEF_CRIT_DAMAGE, 25.0f, 1}
        },
        .effectCount = 2,
        .statusCount = 0,
        .skill = {0}
    },

    // 25
    {
        .id = 25,
        .name = "Fortress",
        .description = "Massive defensive bonuses",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_DEFENSE, 25.0f, 1},
            {BEF_DAMAGE_REDUCTION, 7.0f, 1}
        },
        .effectCount = 2,
        .statusCount = 0,
        .skill = {0}
    },

    // 26
    {
        .id = 26,
        .name = "Phantom Step",
        .description = "High dodge and accuracy",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_DODGE, 8.0f, 1},
            {BEF_ACCURACY, 10.0f, 1}
        },
        .effectCount = 2,
        .statusCount = 0,
        .skill = {0}
    },

    // 27
    {
        .id = 27,
        .name = "Blood Pact",
        .description = "High lifesteal and damage",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_LIFESTEAL, 8.0f, 1},
            {BEF_DAMAGE_BOOST, 10.0f, 1}
        },
        .effectCount = 2,
        .statusCount = 0,
        .skill = {0}
    },

    // 28
    {
        .id = 28,
        .name = "Elemental Balance",
        .description = "Bonus to all elemental damage",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_FIRE_DAMAGE, 10.0f, 1},
            {BEF_ICE_DAMAGE, 10.0f, 1},
            {BEF_POISON_DAMAGE, 10.0f, 1}
        },
        .effectCount = 3,
        .statusCount = 0,
        .skill = {0}
    },

    // 29
    {
        .id = 29,
        .name = "Elemental Ward",
        .description = "Resistance to all elements",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_FIRE_RES, 12.0f, 1},
            {BEF_ICE_RES, 12.0f, 1},
            {BEF_POISON_RES, 12.0f, 1}
        },
        .effectCount = 3,
        .statusCount = 0,
        .skill = {0}
    },

    // 30
    {
        .id = 30,
        .name = "Stunning Blow",
        .description = "Attacks can stun enemies",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_DAMAGE_BOOST, 8.0f, 1}
        },
        .effectCount = 1,
        .statuses = {
            {STATUS_STUN, 0.0f, 20}
        },
        .statusCount = 1,
        .skill = {0}
    },

    // 31
    {
        .id = 31,
        .name = "Crippling Strike",
        .description = "Reduces enemy attack power",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_DAMAGE_BOOST, 10.0f, 1}
        },
        .effectCount = 1,
        .statuses = {
            {STATUS_ATK_DOWN, 15.0f, 25}
        },
        .statusCount = 1,
        .skill = {0}
    },

    // 32
    {
        .id = 32,
        .name = "Armor Shatter",
        .description = "Reduces enemy defense",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_ARMOR_PEN, 15.0f, 1}
        },
        .effectCount = 1,
        .statuses = {
            {STATUS_DEF_DOWN, 20.0f, 30}
        },
        .statusCount = 1,
        .skill = {0}
    },

    // 33
    {
        .id = 33,
        .name = "Rapid Regeneration",
        .description = "Strong health regeneration",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_REGEN, 8.0f, 1},
            {BEF_MAX_HP, 80.0f, 1}
        },
        .effectCount = 2,
        .statusCount = 0,
        .skill = {0}
    },

    // 34
    {
        .id = 34,
        .name = "Spellbreaker",
        .description = "Silence enemies on hit",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_RES_SILENCE, 20.0f, 1}
        },
        .effectCount = 1,
        .statuses = {
            {STATUS_SILENCE, 0.0f, 18}
        },
        .statusCount = 1,
        .skill = {0}
    },

    // 35
    {
        .id = 35,
        .name = "Retribution",
        .description = "High thorns and reflect damage",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_THORN, 10.0f, 1},
            {BEF_REFLECT, 12.0f, 1}
        },
        .effectCount = 2,
        .statusCount = 0,
        .skill = {0}
    },

    // 36
    {
        .id = 36,
        .name = "Guardian Angel",
        .description = "Massive shield and healing boost",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_SHIELD, 50.0f, 1},
            {BEF_HEAL_BOOST, 15.0f, 1}
        },
        .effectCount = 2,
        .statusCount = 0,
        .skill = {0}
    },

    // 37
    {
        .id = 37,
        .name = "Berserker Rage",
        .description = "High damage with crit bonus",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_DAMAGE_BOOST, 20.0f, 1},
            {BEF_CRIT_CHANCE, 5.0f, 1}
        },
        .effectCount = 2,
        .statusCount = 0,
        .skill = {0}
    },

    // 38
    {
        .id = 38,
        .name = "Fortune's Favor",
        .description = "Increased luck and crit chance",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_LUCK, 8.0f, 1},
            {BEF_CRIT_CHANCE, 6.0f, 1}
        },
        .effectCount = 2,
        .statusCount = 0,
        .skill = {0}
    },

    // 39
    {
        .id = 39,
        .name = "Executioner",
        .description = "Massive critical damage boost",
        .rarity = RARITY_RARE,
        .effects = {
            {BEF_CRIT_DAMAGE, 40.0f, 1},
            {BEF_ARMOR_PEN, 10.0f, 1}
        },
        .effectCount = 2,
        .statusCount = 0,
        .skill = {0}
    },

    // ==========================================
    // EPIC BLESSINGS (40-59)
    // ==========================================

    // 40
    {
        .id = 40,
        .name = "Titan's Strength",
        .description = "Overwhelming power and vitality",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_DAMAGE_BOOST, 25.0f, 1},
            {BEF_MAX_HP, 200.0f, 1},
            {BEF_DEFENSE, 20.0f, 1}
        },
        .effectCount = 3,
        .statusCount = 0,
        .skill = {0}
    },

    // 41
    {
        .id = 41,
        .name = "Phoenix Fire",
        .description = "Intense fire damage with burn and weakness",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_FIRE_DAMAGE, 30.0f, 1},
            {BEF_CRIT_CHANCE, 10.0f, 1}
        },
        .effectCount = 2,
        .statuses = {
            {STATUS_BURN, 15.0f, 35},
            {STATUS_WEAKEN, 10.0f, 25}
        },
        .statusCount = 2,
        .skill = {0}
    },

    // 42
    {
        .id = 42,
        .name = "Absolute Zero",
        .description = "Devastating ice damage with freeze",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_ICE_DAMAGE, 30.0f, 1},
            {BEF_DAMAGE_BOOST, 15.0f, 1}
        },
        .effectCount = 2,
        .statuses = {
            {STATUS_FREEZE, 0.0f, 25},
            {STATUS_DEF_DOWN, 25.0f, 30}
        },
        .statusCount = 2,
        .skill = {0}
    },

    // 43
    {
        .id = 43,
        .name = "Plague Bearer",
        .description = "Spreading poison with multiple debuffs",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_POISON_DAMAGE, 30.0f, 1}
        },
        .effectCount = 1,
        .statuses = {
            {STATUS_POISON, 20.0f, 40},
            {STATUS_ATK_DOWN, 20.0f, 35},
            {STATUS_DEF_DOWN, 20.0f, 35}
        },
        .statusCount = 3,
        .skill = {0}
    },

    // 44
    {
        .id = 44,
        .name = "Death's Edge",
        .description = "Extreme critical hit potential",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_CRIT_CHANCE, 15.0f, 1},
            {BEF_CRIT_DAMAGE, 60.0f, 1},
            {BEF_ACCURACY, 15.0f, 1}
        },
        .effectCount = 3,
        .statusCount = 0,
        .skill = {0}
    },

    // 45
    {
        .id = 45,
        .name = "Immortal Fortress",
        .description = "Ultimate defensive capabilities",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_DEFENSE, 50.0f, 1},
            {BEF_DAMAGE_REDUCTION, 15.0f, 1},
            {BEF_MAX_HP, 250.0f, 1},
            {BEF_REGEN, 10.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {0}
    },

    // 46
    {
        .id = 46,
        .name = "Shadow Dance",
        .description = "Perfect evasion and counter",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_DODGE, 20.0f, 1},
            {BEF_ACCURACY, 20.0f, 1},
            {BEF_CRIT_CHANCE, 12.0f, 1}
        },
        .effectCount = 3,
        .statusCount = 0,
        .skill = {0}
    },

    // 47
    {
        .id = 47,
        .name = "Vampire Lord",
        .description = "Massive lifesteal with damage",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_LIFESTEAL, 20.0f, 1},
            {BEF_DAMAGE_BOOST, 25.0f, 1},
            {BEF_MAX_HP, 150.0f, 1}
        },
        .effectCount = 3,
        .statusCount = 0,
        .skill = {0}
    },

    // 48
    {
        .id = 48,
        .name = "Elemental Mastery",
        .description = "Supreme control over all elements",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_FIRE_DAMAGE, 25.0f, 1},
            {BEF_ICE_DAMAGE, 25.0f, 1},
            {BEF_POISON_DAMAGE, 25.0f, 1},
            {BEF_DAMAGE_BOOST, 15.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {0}
    },

    // 49
    {
        .id = 49,
        .name = "Elemental Immunity",
        .description = "Near immunity to elemental damage",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_FIRE_RES, 30.0f, 1},
            {BEF_ICE_RES, 30.0f, 1},
            {BEF_POISON_RES, 30.0f, 1},
            {BEF_DEFENSE, 25.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {0}
    },

    // 50
    {
        .id = 50,
        .name = "Warlord's Command",
        .description = "Dominates enemies with debuffs",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_DAMAGE_BOOST, 20.0f, 1},
            {BEF_ARMOR_PEN, 25.0f, 1}
        },
        .effectCount = 2,
        .statuses = {
            {STATUS_ATK_DOWN, 30.0f, 40},
            {STATUS_DEF_DOWN, 30.0f, 40},
            {STATUS_STUN, 0.0f, 30}
        },
        .statusCount = 3,
        .skill = {0}
    },

    // 51
    {
        .id = 51,
        .name = "Eternal Regeneration",
        .description = "Extreme healing and sustainability",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_REGEN, 20.0f, 1},
            {BEF_HEAL_BOOST, 30.0f, 1},
            {BEF_MAX_HP, 200.0f, 1},
            {BEF_LIFESTEAL, 10.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {0}
    },

    // 52
    {
        .id = 52,
        .name = "Arcane Nullifier",
        .description = "Silences and resists magical effects",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_RES_SILENCE, 40.0f, 1},
            {BEF_RES_STUN, 40.0f, 1}
        },
        .effectCount = 2,
        .statuses = {
            {STATUS_SILENCE, 0.0f, 30}
        },
        .statusCount = 1,
        .skill = {0}
    },

    // 53
    {
        .id = 53,
        .name = "Vengeance",
        .description = "Returns massive damage to attackers",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_THORN, 25.0f, 1},
            {BEF_REFLECT, 30.0f, 1},
            {BEF_DEFENSE, 30.0f, 1}
        },
        .effectCount = 3,
        .statusCount = 0,
        .skill = {0}
    },

    // 54
    {
        .id = 54,
        .name = "Divine Protection",
        .description = "Powerful shields and damage reduction",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_SHIELD, 100.0f, 1},
            {BEF_DAMAGE_REDUCTION, 20.0f, 1},
            {BEF_HEAL_BOOST, 25.0f, 1}
        },
        .effectCount = 3,
        .statusCount = 0,
        .skill = {0}
    },

    // 55
    {
        .id = 55,
        .name = "Bloodlust",
        .description = "Extreme offense at the cost of safety",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_DAMAGE_BOOST, 40.0f, 1},
            {BEF_CRIT_CHANCE, 15.0f, 1},
            {BEF_LIFESTEAL, 15.0f, 1}
        },
        .effectCount = 3,
        .statusCount = 0,
        .skill = {0}
    },

    // 56
    {
        .id = 56,
        .name = "Destiny's Call",
        .description = "Fortune smiles upon you",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_LUCK, 20.0f, 1},
            {BEF_CRIT_CHANCE, 12.0f, 1},
            {BEF_DODGE, 15.0f, 1}
        },
        .effectCount = 3,
        .statusCount = 0,
        .skill = {0}
    },

    // 57
    {
        .id = 57,
        .name = "Armor Obliterator",
        .description = "Destroys all enemy defenses",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_ARMOR_PEN, 40.0f, 1},
            {BEF_DAMAGE_BOOST, 20.0f, 1}
        },
        .effectCount = 2,
        .statuses = {
            {STATUS_DEF_DOWN, 40.0f, 45},
            {STATUS_VULNERABLE, 25.0f, 35}
        },
        .statusCount = 2,
        .skill = {0}
    },

    // 58
    {
        .id = 58,
        .name = "Perfect Strike",
        .description = "Never miss with devastating precision",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_ACCURACY, 40.0f, 1},
            {BEF_CRIT_CHANCE, 18.0f, 1},
            {BEF_DAMAGE_BOOST, 22.0f, 1}
        },
        .effectCount = 3,
        .statusCount = 0,
        .skill = {0}
    },

    // 59
    {
        .id = 59,
        .name = "Chaos Storm",
        .description = "Unleashes all elements at once",
        .rarity = RARITY_EPIC,
        .effects = {
            {BEF_FIRE_DAMAGE, 20.0f, 1},
            {BEF_ICE_DAMAGE, 20.0f, 1},
            {BEF_POISON_DAMAGE, 20.0f, 1}
        },
        .effectCount = 3,
        .statuses = {
            {STATUS_BURN, 12.0f, 30},
            {STATUS_FREEZE, 0.0f, 20},
            {STATUS_POISON, 15.0f, 35}
        },
        .statusCount = 3,
        .skill = {0}
    },

    // ==========================================
    // LEGENDARY BLESSINGS (60-79)
    // ==========================================

    // 60
    {
        .id = 60,
        .name = "God of War",
        .description = "Ultimate offensive power incarnate",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_DAMAGE_BOOST, 50.0f, 1},
            {BEF_CRIT_CHANCE, 25.0f, 1},
            {BEF_CRIT_DAMAGE, 100.0f, 1},
            {BEF_ARMOR_PEN, 50.0f, 1}
        },
        .effectCount = 4,
        .statuses = {
            {STATUS_WEAKEN, 30.0f, 50}
        },
        .statusCount = 1,
        .skill = {1, "Wrath of Ares", 60}
    },

    // 61
    {
        .id = 61,
        .name = "Eternal Flame",
        .description = "The primordial fire that never dies",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_FIRE_DAMAGE, 60.0f, 1},
            {BEF_DAMAGE_BOOST, 30.0f, 1},
            {BEF_CRIT_DAMAGE, 50.0f, 1}
        },
        .effectCount = 3,
        .statuses = {
            {STATUS_BURN, 35.0f, 50},
            {STATUS_WEAKEN, 25.0f, 40},
            {STATUS_DEF_DOWN, 35.0f, 45}
        },
        .statusCount = 3,
        .skill = {1, "Supernova", 90}
    },

    // 62
    {
        .id = 62,
        .name = "Fimbulwinter",
        .description = "The endless winter that ends worlds",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_ICE_DAMAGE, 60.0f, 1},
            {BEF_DAMAGE_BOOST, 30.0f, 1},
            {BEF_ICE_RES, 50.0f, 1}
        },
        .effectCount = 3,
        .statuses = {
            {STATUS_FREEZE, 0.0f, 40},
            {STATUS_ATK_DOWN, 40.0f, 50},
            {STATUS_DEF_DOWN, 40.0f, 50}
        },
        .statusCount = 3,
        .skill = {1, "Glacial Apocalypse", 90}
    },

    // 63
    {
        .id = 63,
        .name = "Pestilence",
        .description = "The fourth horseman brings death",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_POISON_DAMAGE, 60.0f, 1},
            {BEF_DAMAGE_BOOST, 30.0f, 1}
        },
        .effectCount = 2,
        .statuses = {
            {STATUS_POISON, 40.0f, 60},
            {STATUS_ATK_DOWN, 35.0f, 55},
            {STATUS_DEF_DOWN, 35.0f, 55}
        },
        .statusCount = 3,
        .skill = {1, "Black Death", 75}
    },

    // 64
    {
        .id = 64,
        .name = "Omnipotence",
        .description = "The power of a supreme being",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_DAMAGE_BOOST, 40.0f, 1},
            {BEF_MAX_HP, 500.0f, 1},
            {BEF_DEFENSE, 75.0f, 1},
            {BEF_DAMAGE_REDUCTION, 30.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {1, "Divine Judgment", 120}
    },

    // 65
    {
        .id = 65,
        .name = "Reaper's Scythe",
        .description = "Death comes for all",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_CRIT_CHANCE, 30.0f, 1},
            {BEF_CRIT_DAMAGE, 150.0f, 1},
            {BEF_ACCURACY, 50.0f, 1},
            {BEF_ARMOR_PEN, 60.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {1, "Soul Reap", 80}
    },

    // 66
    {
        .id = 66,
        .name = "Immortality",
        .description = "Cannot be killed by mortal means",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_MAX_HP, 1000.0f, 1},
            {BEF_DEFENSE, 100.0f, 1},
            {BEF_DAMAGE_REDUCTION, 40.0f, 1},
            {BEF_REGEN, 50.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {1, "Resurrection", 180}
    },

    // 67
    {
        .id = 67,
        .name = "Shadow Sovereign",
        .description = "Master of the void and darkness",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_DODGE, 40.0f, 1},
            {BEF_CRIT_CHANCE, 25.0f, 1},
            {BEF_DAMAGE_BOOST, 45.0f, 1},
            {BEF_ACCURACY, 40.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {1, "Void Step", 50}
    },

    // 68
    {
        .id = 68,
        .name = "Crimson Pact",
        .description = "Ultimate blood magic mastery",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_LIFESTEAL, 40.0f, 1},
            {BEF_DAMAGE_BOOST, 45.0f, 1},
            {BEF_MAX_HP, 400.0f, 1},
            {BEF_CRIT_DAMAGE, 80.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {1, "Blood Sacrifice", 100}
    },

    // 69
    {
        .id = 69,
        .name = "Elemental Chaos",
        .description = "All elements bow before you",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_FIRE_DAMAGE, 50.0f, 1},
            {BEF_ICE_DAMAGE, 50.0f, 1},
            {BEF_POISON_DAMAGE, 50.0f, 1},
            {BEF_DAMAGE_BOOST, 35.0f, 1}
        },
        .effectCount = 4,
        .statuses = {
            {STATUS_BURN, 25.0f, 45},
            {STATUS_FREEZE, 0.0f, 35},
            {STATUS_POISON, 30.0f, 50}
        },
        .statusCount = 3,
        .skill = {1, "Elemental Cataclysm", 110}
    },

    // 70
    {
        .id = 70,
        .name = "Absolute Defense",
        .description = "Impenetrable fortress of protection",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_DEFENSE, 150.0f, 1},
            {BEF_DAMAGE_REDUCTION, 50.0f, 1},
            {BEF_SHIELD, 500.0f, 1},
            {BEF_REFLECT, 100.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {1, "Fortress Mode", 150}
    },

    // 71
    {
        .id = 71,
        .name = "Conqueror",
        .description = "Dominate all who oppose you",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_DAMAGE_BOOST, 50.0f, 1},
            {BEF_ARMOR_PEN, 70.0f, 1},
            {BEF_ACCURACY, 45.0f, 1}
        },
        .effectCount = 3,
        .statuses = {
            {STATUS_STUN, 0.0f, 45},
            {STATUS_ATK_DOWN, 50.0f, 60},
            {STATUS_DEF_DOWN, 50.0f, 60}
        },
        .statusCount = 3,
        .skill = {1, "Total Domination", 100}
    },

    // 72
    {
        .id = 72,
        .name = "Phoenix Rebirth",
        .description = "Rise from ashes stronger than before",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_REGEN, 100.0f, 1},
            {BEF_HEAL_BOOST, 80.0f, 1},
            {BEF_MAX_HP, 600.0f, 1},
            {BEF_FIRE_RES, 75.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {1, "Rebirth", 200}
    },

    // 73
    {
        .id = 73,
        .name = "Nullification",
        .description = "Negate all magical effects",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_RES_SILENCE, 100.0f, 1},
            {BEF_RES_STUN, 100.0f, 1},
            {BEF_FIRE_RES, 50.0f, 1},
            {BEF_ICE_RES, 50.0f, 1}
        },
        .effectCount = 4,
        .statuses = {
            {STATUS_SILENCE, 0.0f, 50}
        },
        .statusCount = 1,
        .skill = {1, "Anti-Magic Field", 120}
    },

    // 74
    {
        .id = 74,
        .name = "Vindication",
        .description = "Ultimate retribution and vengeance",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_THORN, 80.0f, 1},
            {BEF_REFLECT, 100.0f, 1},
            {BEF_DEFENSE, 80.0f, 1},
            {BEF_DAMAGE_REDUCTION, 25.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {1, "Divine Retribution", 90}
    },

    // 75
    {
        .id = 75,
        .name = "Aegis of the Gods",
        .description = "Protected by divine intervention",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_SHIELD, 1000.0f, 1},
            {BEF_DAMAGE_REDUCTION, 45.0f, 1},
            {BEF_HEAL_BOOST, 100.0f, 1},
            {BEF_REGEN, 75.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {1, "Divine Shield", 140}
    },

    // 76
    {
        .id = 76,
        .name = "Infinity",
        .description = "Power without limits",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_DAMAGE_BOOST, 60.0f, 1},
            {BEF_CRIT_CHANCE, 30.0f, 1},
            {BEF_CRIT_DAMAGE, 120.0f, 1},
            {BEF_LIFESTEAL, 30.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {1, "Infinite Power", 160}
    },

    // 77
    {
        .id = 77,
        .name = "Fate Weaver",
        .description = "Control destiny itself",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_LUCK, 100.0f, 1},
            {BEF_CRIT_CHANCE, 35.0f, 1},
            {BEF_DODGE, 50.0f, 1},
            {BEF_ACCURACY, 50.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {1, "Rewrite Fate", 180}
    },

    // 78
    {
        .id = 78,
        .name = "Annihilation",
        .description = "Complete destruction of all enemies",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_DAMAGE_BOOST, 70.0f, 1},
            {BEF_ARMOR_PEN, 100.0f, 1},
            {BEF_CRIT_DAMAGE, 100.0f, 1}
        },
        .effectCount = 3,
        .statuses = {
            {STATUS_VULNERABLE, 50.0f, 60},
            {STATUS_WEAKEN, 50.0f, 60}
        },
        .statusCount = 2,
        .skill = {1, "Total Annihilation", 150}
    },

    // 79
    {
        .id = 79,
        .name = "Transcendence",
        .description = "Beyond mortal comprehension",
        .rarity = RARITY_LEGENDARY,
        .effects = {
            {BEF_DAMAGE_BOOST, 55.0f, 1},
            {BEF_MAX_HP, 800.0f, 1},
            {BEF_DEFENSE, 90.0f, 1},
            {BEF_REGEN, 80.0f, 1}
        },
        .effectCount = 4,
        .statusCount = 0,
        .skill = {1, "Ascension", 200}
    }

};