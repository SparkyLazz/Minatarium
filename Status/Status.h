#ifndef STATUS_H
#define STATUS_H

// ======================================
//  STATUS TYPES
//  (your original list, unchanged)
// ======================================
typedef enum {
    STATUS_NONE = 0,

    // Damage Over Time
    STATUS_BURN,
    STATUS_POISON,

    // Crowd Control
    STATUS_STUN,
    STATUS_FREEZE,
    STATUS_SILENCE,

    // Debuffs
    STATUS_DEF_DOWN,
    STATUS_ATK_DOWN,
    STATUS_DMG_TAKEN_UP,
    STATUS_CRIT_RES_DOWN,

    // Special
    STATUS_WEAKEN,
    STATUS_VULNERABLE

} StatusType;


// ======================================
//  STATUS ENTRY
//  - Clean
//  - No stacks
//  - Blessing stacks scale effects
// ======================================
typedef struct {

    StatusType type;
    float baseAmount;
    int duration;

} Status;


// ======================================
//  COMBAT ATTRIBUTES
//  (Your original struct — unchanged)
// ======================================
typedef struct {

    // --- Primary Stats ---
    long long maxHp;
    long long hp;
    long long attack;
    long long defense;

    // --- Offensive Stats ---
    struct {
        int critChance;
        int critDamage;
        int penetration;
        int damageBoost;
        int accuracy;
    } off;

    // --- Defensive Stats ---
    struct {
        int dodge;
        int blockRate;
        int damageReduction;
    } def;

    // --- Elemental ---
    struct {
        int fireDamage;
        int iceDamage;
        int poisonDamage;

        int fireRes;
        int iceRes;
        int poisonRes;
    } elem;

    // --- Sustain ---
    struct {
        int lifeSteal;
        int regen;
    } sustain;

    // --- Status Control ---
    struct {
        int effectHitRate;
        int resistanceEffect;
    } status;

    // --- Utility ---
    struct {
        int reflectDamage;
    } util;

} CombatAttributes;

#endif
