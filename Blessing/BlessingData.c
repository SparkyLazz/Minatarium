#include "Blessing.h"
Blessing warrior_strength = {
    .id = 1,
    .name = "Warrior's Strength",
    .description = "Basic combat prowess",
    .rarity = RARITY_COMMON,
    .effects = {
        {BEF_DAMAGE_BOOST, 2.0f, 1},
        {BEF_MAX_HP, 10.0f, 1}
    },
    .effectCount = 2,
    .statusCount = 0
};

// 2. Guardian's Shield
Blessing guardian_shield = {
    .id = 2,
    .name = "Guardian's Shield",
    .description = "Enhanced defensive capabilities",
    .rarity = RARITY_COMMON,
    .effects = {
        {BEF_DEFENSE, 3.0f, 1},
        {BEF_SHIELD, 5.0f, 1}
    },
    .effectCount = 2,
    .statusCount = 0
};

// 3. Swift Footwork
Blessing swift_footwork = {
    .id = 3,
    .name = "Swift Footwork",
    .description = "Increased evasion",
    .rarity = RARITY_COMMON,
    .effects = {
        {BEF_DODGE, 5.0f, 1},
        {BEF_ACCURACY, 3.0f, 1}
    },
    .effectCount = 2,
    .statusCount = 0
};

// 4. Minor Regeneration
Blessing minor_regen = {
    .id = 4,
    .name = "Minor Regeneration",
    .description = "Slowly restore health",
    .rarity = RARITY_COMMON,
    .effects = {
        {BEF_REGEN, 2.0f, 1}
    },
    .effectCount = 1,
    .statusCount = 0
};

// 5. Lucky Strike
Blessing lucky_strike = {
    .id = 5,
    .name = "Lucky Strike",
    .description = "Chance favors the bold",
    .rarity = RARITY_COMMON,
    .effects = {
        {BEF_CRIT_CHANCE, 3.0f, 1},
        {BEF_LUCK, 2.0f, 1}
    },
    .effectCount = 2,
    .statusCount = 0
};

// 6. Flame Touch
Blessing flame_touch = {
    .id = 6,
    .name = "Flame Touch",
    .description = "Attacks deal minor fire damage",
    .rarity = RARITY_COMMON,
    .effects = {
        {BEF_FIRE_DAMAGE, 3.0f, 1}
    },
    .effectCount = 1,
    .statuses = {
        {STATUS_BURN, 2.0f, 15}
    },
    .statusCount = 1
};

// 7. Frost Bite
Blessing frost_bite = {
    .id = 7,
    .name = "Frost Bite",
    .description = "Chilling attacks",
    .rarity = RARITY_COMMON,
    .effects = {
        {BEF_ICE_DAMAGE, 3.0f, 1}
    },
    .effectCount = 1,
    .statuses = {
        {STATUS_FREEZE, 5.0f, 20}
    },
    .statusCount = 1
};

// 8. Venom Coating
Blessing venom_coating = {
    .id = 8,
    .name = "Venom Coating",
    .description = "Poison your enemies",
    .rarity = RARITY_COMMON,
    .effects = {
        {BEF_POISON_DAMAGE, 2.0f, 1}
    },
    .effectCount = 1,
    .statuses = {
        {STATUS_POISON, 3.0f, 25}
    },
    .statusCount = 1
};

// 9. Iron Will
Blessing iron_will = {
    .id = 9,
    .name = "Iron Will",
    .description = "Resist mental effects",
    .rarity = RARITY_COMMON,
    .effects = {
        {BEF_RES_STUN, 10.0f, 1},
        {BEF_RES_SILENCE, 10.0f, 1}
    },
    .effectCount = 2,
    .statusCount = 0
};

// 10. Beginner's Fortune
Blessing beginner_fortune = {
    .id = 10,
    .name = "Beginner's Fortune",
    .description = "A bit of everything",
    .rarity = RARITY_COMMON,
    .effects = {
        {BEF_DAMAGE_BOOST, 1.0f, 1},
        {BEF_DEFENSE, 2.0f, 1},
        {BEF_MAX_HP, 5.0f, 1}
    },
    .effectCount = 3,
    .statusCount = 0
};