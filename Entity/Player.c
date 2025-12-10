#include "Player.h"
Player CreateDefaultPlayer() {
    Player p;

    // --- Base Stats ---
    p.attributes.maxHp = 100;
    p.attributes.hp = 100;
    p.attributes.defense = 5;
    p.attributes.attack = 10;

    // --- Defensive Attributes ---
    p.attributes.defenseBoost = 0;
    p.attributes.damageReduction = 0;
    p.attributes.blockRate = 0;
    p.attributes.shield = 0;
    p.attributes.dodge = 2;
    p.attributes.damageMitigation = 0;

    // --- Resistances ---
    p.attributes.resistanceFire = 5;
    p.attributes.resistanceIce = 5;
    p.attributes.resistancePoison = 5;
    p.attributes.resistanceStun = 5;
    p.attributes.resistanceSilence = 5;
    p.attributes.resistanceEffect = 5;

    // --- HP & Recovery ---
    p.attributes.regen = 0;
    p.attributes.regenBoost = 0;
    p.attributes.hpBoost = 0;
    p.attributes.hpAlteration = 0;
    p.attributes.lifeSteal = 0;
    p.attributes.healBoost = 0;

    // --- Debuffs inflicted on enemy ---
    p.attributes.AR = 0;
    p.attributes.DR = 0;

    // --- Status Inflict Rates ---
    p.attributes.rateFire = 0;
    p.attributes.rateIce = 0;
    p.attributes.ratePoison = 0;
    p.attributes.rateStun = 0;
    p.attributes.rateSilence = 0;
    p.attributes.effectHitRate = 5;

    // --- Elemental ---
    p.attributes.elementalFire = 0;
    p.attributes.elementalIce = 0;
    p.attributes.elementalPoison = 0;

    // --- Offensive ---
    p.attributes.damageBoost = 0;
    p.attributes.critChance = 0;
    p.attributes.critDamage = 50;
    p.attributes.penetration = 0;
    p.attributes.armorPenetration = 0;
    p.attributes.statusPenetration = 0;

    // --- Utility ---
    p.attributes.accuracy = 10;
    p.attributes.reflectDamage = 0;
    p.attributes.invulnerability = 0;
    p.attributes.silence = 0;
    p.blessingCount = 0;
    p.statusCount = 0;
    return p;
}