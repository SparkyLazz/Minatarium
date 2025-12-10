#ifndef STATUS_H
#define STATUS_H

// -----------------------------
//       STATUS ENUM + STRUCT
// -----------------------------
typedef enum {
    STATUS_NONE = 0,
    STATUS_DAMAGE_REDUCTION,
    STATUS_HP_REDUCTION,
} StatusType;

typedef struct {
    StatusType type;
    float value;
    int duration;
    int maxDuration;
} Status;


// -----------------------------
//       COMBAT ATTRIBUTES
// -----------------------------
typedef struct {
    // --- Defensive Attributes ---
    long long int defense;
    int defenseBoost;
    int damageReduction;
    int blockRate;
    int shield;
    int dodge;
    int damageMitigation;

    // --- Resistances ---
    int resistanceFire;
    int resistanceIce;
    int resistancePoison;
    int resistanceStun;
    int resistanceSilence;
    int resistanceEffect;

    // --- HP & Recovery ---
    long long int hp;
    long long int maxHp;
    int regen;
    int regenBoost;
    int hpBoost;
    int hpAlteration;
    int lifeSteal;
    int healBoost;

    // --- Debuffs inflicted on enemy ---
    int AR;
    int DR;

    // --- Status Inflict Rates ---
    int rateFire;
    int rateIce;
    int ratePoison;
    int rateStun;
    int rateSilence;
    int effectHitRate;

    // --- Elemental Effects ---
    int elementalFire;
    int elementalIce;
    int elementalPoison;

    // --- Offensive ---
    long long int attack;
    int damageBoost;
    int critChance;
    int critDamage;
    int penetration;
    int armorPenetration;
    int statusPenetration;

    // --- Utility ---
    int accuracy;
    int reflectDamage;
    int invulnerability;
    int silence;

} CombatAttributes;
#endif
