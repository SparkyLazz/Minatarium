# Blessing System Documentation

## 📋 Overview

The Blessing system is the core progression mechanic in Minatarium. Blessings are permanent power-ups that players collect after defeating enemies. Each blessing can stack infinitely, with effects multiplying with each stack.

## 🎯 Design Philosophy

1. **Stacking Power**: Unlike most games, blessings stack infinitely, creating exponential power growth
2. **Rarity Matters**: Higher rarity = more effects and stronger synergies
3. **Build Diversity**: Different blessing combinations enable various playstyles
4. **Clear Communication**: Effects are transparent and predictable

## 📁 File Structure

```
Blessing/
├── Blessing.h      # Data structures, enums, and interface
└── BlessingData.c  # Database of all 17 blessings
```

## 🗂️ Data Structures

### Rarity Levels

```c
typedef enum {
    RARITY_COMMON,      // Basic stat boosts
    RARITY_RARE,        // Elemental damage, specialized effects
    RARITY_EPIC,        // Multi-effect combinations
    RARITY_LEGENDARY    // Game-changing power
} BlessingRarity;
```

**Distribution:**
- Common (5): Single effect, straightforward
- Rare (5): Introduces elemental damage and status effects
- Epic (4): 2-3 effects per blessing
- Legendary (3): 3-4 effects, build-defining

### Effect Types

```c
typedef enum {
    // Offensive
    DAMAGE_BOOST,        // Increases all damage by X%
    CRITICAL_CHANGE,     // Increases crit chance (capped at 50%)
    CRITICAL_DAMAGE,     // Increases crit multiplier
    ARMOR_PENETRATION,   // Reduces enemy defense
    ACCURACY_BOOST,      // Increases hit chance
    
    // Elemental
    FIRE_DAMAGE,         // Bonus fire damage + burn chance
    ICE_DAMAGE,          // Bonus ice damage + freeze chance
    POISON_DAMAGE,       // Bonus poison damage + poison chance
    
    // Defensive
    HP_BOOST,            // Increases max HP
    DEFENSE_BOOST,       // Increases defense stat
    SHIELD_BOOST,        // [Not implemented yet]
    
    // Sustain
    REGEN,               // HP regen per turn
    LIFESTEAL,           // Steal % of damage dealt
    REGEN_BOOST,         // [Not implemented yet]
    
    // Unique
    THORN,               // Reflect damage back
    LUCK,                // [Not implemented yet]
    INVULNERABLE         // [Not implemented yet]
} BlessingEffectType;
```

### Blessing Effect Structure

```c
typedef struct {
    BlessingEffectType type;
    float baseValue;  // 0.2 = 20%
} BlessingEffect;
```

**Example:**
```c
{DAMAGE_BOOST, 0.05f}  // +5% damage per stack
```

### DoT (Damage over Time) Structure

```c
typedef struct {
    Status DoT;      // Status effect to apply
    int change;      // Chance to apply (0-100)
} BlessingDot;
```

**Example:**
```c
{
    .DoT = {BURN, 5.0f, 3},  // 5 damage, 3 turns
    .change = 35              // 35% chance on hit
}
```

### Main Blessing Structure

```c
typedef struct {
    int id;                    // Unique identifier
    char name[64];             // Display name
    char description[128];     // Flavor text
    
    BlessingRarity rarity;     // Rarity tier
    long long int stacks;      // How many times collected
    
    BlessingEffect effects[4]; // Up to 4 effects
    int effectsCount;          // Number of active effects
    
    BlessingDot dots[4];       // Up to 4 DoT effects
    int dotsCount;             // Number of DoT effects
} Blessing;
```

### Blessing Database

```c
typedef struct {
    Blessing* blessings;  // Array of all blessings
    int count;            // Total blessing count
} BlessingDatabase;
```

## 🔄 Code Flow

### 1. Database Initialization

```c
// In BlessingData.c
static Blessing sampleBlessings[] = {
    // Common blessing example
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
    // ... more blessings
};

// Singleton pattern for global access
static BlessingDatabase globalDB = {
    .blessings = sampleBlessings,
    .count = sizeof(sampleBlessings) / sizeof(sampleBlessings[0])
};

BlessingDatabase* GetBlessingDatabase() {
    return &globalDB;
}
```

### 2. Blessing Acquisition Flow

```c
// In GameReward.c
void BlessingWinningReward(Character* player, CharacterType enemyType) {
    // Step 1: Determine rarity range based on enemy type
    BlessingRarity minRarity, maxRarity;
    switch (enemyType) {
        case NORMAL: 
            minRarity = maxRarity = RARITY_COMMON;
            break;
        case ELITE:
            minRarity = maxRarity = RARITY_RARE;
            break;
        case BOSS:
            minRarity = RARITY_EPIC;
            maxRarity = RARITY_LEGENDARY;
            break;
    }
    
    // Step 2: Generate 3 random blessings in rarity range
    Blessing* choices[3];
    for (int i = 0; i < 3; i++) {
        choices[i] = GetRandomBlessingInRange(db, minRarity, maxRarity);
    }
    
    // Step 3: Player selects one
    int selectedIndex = ShowBlessingSelection(choices);
    
    // Step 4: Add blessing to character
    CharacterAddBlessing(player, choices[selectedIndex]);
}
```

### 3. Blessing Application

```c
// In CharacterUtil.c
void CharacterAddBlessing(Character* target, const Blessing* blessing) {
    // Check if blessing already exists
    int existingIndex = FindBlessingByID(target, blessing->id);
    
    if (existingIndex >= 0) {
        // CASE 1: Stack existing blessing
        target->currentBlessing[existingIndex].stacks += 1;
    } else {
        // CASE 2: Add new blessing with 1 stack
        target->currentBlessing[target->blessingCount] = *blessing;
        target->currentBlessing[target->blessingCount].stacks = 1;
        target->blessingCount++;
    }
    
    // Apply blessing effects to bonus stats
    for (int i = 0; i < blessing->effectsCount; i++) {
        float value = blessing->effects[i].baseValue;
        
        switch (blessing->effects[i].type) {
            case DAMAGE_BOOST:
                target->attribute.bonus.damageBoost += (int)(value * 100.0f);
                break;
            case HP_BOOST:
                target->attribute.bonus.hp += 
                    (long long)(target->attribute.base.hp * value);
                break;
            // ... other effect types
        }
    }
    
    // CRITICAL: Recalculate all stats
    RecalculateStats(target);
}
```

### 4. Effect Calculation During Combat

```c
// In GameDamage.c
static float CalculateTotalBlessingEffect(Character* character, 
                                          BlessingEffectType effectType) {
    float totalEffect = 0.0f;
    
    // Iterate through all blessings
    for (int i = 0; i < character->blessingCount; i++) {
        Blessing* blessing = &character->currentBlessing[i];
        
        // Check each effect in the blessing
        for (int j = 0; j < blessing->effectsCount; j++) {
            if (blessing->effects[j].type == effectType) {
                // Multiply by stacks
                totalEffect += blessing->effects[j].baseValue 
                             * (float)blessing->stacks;
            }
        }
    }
    
    return totalEffect;
}
```

## 📊 Blessing Catalog

### Common Blessings (5)

| ID | Name | Effect | Base Value |
|----|------|--------|------------|
| 1 | Minor Strength | Damage Boost | +5% |
| 2 | Iron Skin | Defense Boost | +5% |
| 3 | Vitality | HP Boost | +10% |
| 4 | Sharp Eye | Accuracy | +5% |
| 5 | Lucky Charm | Crit Chance | +3% |

### Rare Blessings (5)

| ID | Name | Effects | Values |
|----|------|---------|--------|
| 6 | Warrior's Might | Damage Boost | +12% |
| 7 | Flame Touch | Fire Damage + Burn (35% chance) | +15% + 5 dmg/3 turns |
| 8 | Frost Strike | Ice Damage + Freeze (30% chance) | +15% + 2 turns |
| 9 | Life Drain | Lifesteal | +8% |
| 10 | Critical Focus | Crit Chance + Crit Damage | +8%, +15% |

### Epic Blessings (4)

| ID | Name | Effects | Values |
|----|------|---------|--------|
| 11 | Berserker's Rage | Damage + Lifesteal | +20%, +12% |
| 12 | Phoenix Blessing | Fire Damage + Regen + Burn | +25%, +5%, 10dmg/4t (50%) |
| 13 | Toxic Mastery | Poison Damage + Poison | +30% + 8dmg/5t (60%) |
| 14 | Fortress Shield | HP + Defense | +25%, +20% |

### Legendary Blessings (3)

| ID | Name | Effects | Values |
|----|------|---------|--------|
| 15 | Divine Wrath | Damage + Crit Chance + Crit Dmg + Armor Pen | +35%, +15%, +30%, +25% |
| 16 | Immortal Soul | HP + Defense + Regen + Lifesteal | +50%, +30%, +10%, +15% |
| 17 | Elemental Chaos | Fire + Ice + Poison + All DoTs | +30% each + DoTs (70%) |

## 🎮 Stacking Mechanics

### Linear Stacking
Most effects stack linearly:
```
1 stack:  +5% damage
5 stacks: +25% damage
10 stacks: +50% damage
```

### HP/Defense Scaling
HP and Defense bonuses scale with BASE stats:
```c
bonusHP = baseHP × effectValue × stacks

Example with base HP = 300:
1 stack (10%):  300 × 0.10 × 1 = +30 HP
5 stacks:       300 × 0.10 × 5 = +150 HP
```

### DoT Stacking
Status effect damage scales with stacks:
```c
dotDamage = baseDamage × stacks

Burn (5 base damage):
1 stack:  5 damage/turn
10 stacks: 50 damage/turn
```

### Crit Chance Capping
Critical chance is hard-capped at 50%:
```c
if (character->attribute.current.criticalChance > MAX_CRIT_CHANCE) {
    character->attribute.current.criticalChance = MAX_CRIT_CHANCE;
}
```

## 🔥 Synergy Examples

### 1. Glass Cannon Build
```
Divine Wrath (Legendary) × 10 stacks
+ Critical Focus (Rare) × 20 stacks
+ Warrior's Might (Rare) × 30 stacks
= 50% crit (capped), 450% crit damage, massive attack boost
```

### 2. Tank Build
```
Immortal Soul (Legendary) × 5 stacks
+ Fortress Shield (Epic) × 15 stacks
+ Iron Skin (Common) × 40 stacks
= Extreme HP and defense, good sustain
```

### 3. DoT/Status Build
```
Elemental Chaos (Legendary) × 5 stacks
+ Phoenix Blessing (Epic) × 10 stacks
+ Toxic Mastery (Epic) × 10 stacks
= Multiple status effects, 70%+ application chance
```

## 🐛 Common Issues & Solutions

### Issue 1: Stats Not Updating After Blessing
**Problem:** Adding blessing doesn't change visible stats.

**Solution:** Always call `RecalculateStats()` after modifying blessings:
```c
CharacterAddBlessing(player, blessing);
// Stats are automatically recalculated inside CharacterAddBlessing
```

### Issue 2: Blessing Not Stacking
**Problem:** Getting same blessing twice creates two entries.

**Solution:** Check by `id`, not by name:
```c
for (int i = 0; i < character->blessingCount; i++) {
    if (character->currentBlessing[i].id == blessing->id) {
        // Found existing blessing
        existingIndex = i;
        break;
    }
}
```

### Issue 3: Effect Not Applied in Combat
**Problem:** Blessing exists but doesn't affect damage.

**Solution:** Check if effect is properly calculated:
```c
// Make sure CalculateTotalBlessingEffect is called
float armorPen = CalculateTotalBlessingEffect(attacker, ARMOR_PENETRATION);
// Then apply it
long long effectiveDefense = (long long)(defense * (1.0f - armorPen));
```

## 🚀 Adding New Blessings

### Step 1: Define Effect Type (if needed)
```c
// In Blessing.h
typedef enum {
    // ... existing effects
    NEW_EFFECT_TYPE,  // Add your new effect
} BlessingEffectType;
```

### Step 2: Add Blessing to Database
```c
// In BlessingData.c
{
    .id = 18,  // Next available ID
    .name = "New Blessing",
    .description = "Does something cool",
    .rarity = RARITY_RARE,
    .stacks = 0,
    .effects = {
        {NEW_EFFECT_TYPE, 0.15f}
    },
    .effectsCount = 1,
    .dotsCount = 0
}
```

### Step 3: Handle Effect in CharacterAddBlessing
```c
// In CharacterUtil.c
case NEW_EFFECT_TYPE:
    target->attribute.bonus.newStat += (int)(value * 100.0f);
    break;
```

### Step 4: Apply Effect in Combat
```c
// In GameDamage.c
float newEffect = CalculateTotalBlessingEffect(attacker, NEW_EFFECT_TYPE);
// Use newEffect in your damage calculation
```

### Step 5: Add Display Name
```c
// In CharacterUtil.c - BlessingEffectString()
case NEW_EFFECT_TYPE: return "New Effect Name";
```

## 📈 Balance Guidelines

1. **Common**: 3-7% per stack (single stat)
2. **Rare**: 8-15% per stack OR special effect
3. **Epic**: 15-25% per stack (multiple stats)
4. **Legendary**: 30-50% per stack (3-4 effects)

**DoT Balance:**
- Normal: 5-8 damage, 3-4 turns
- Strong: 10-15 damage, 4-5 turns
- Application chance: 30-70%

## 🎨 Visual Feedback

### Stack Count Colors
```c
void BlessingStackColor(long long stacks) {
    if (stacks <= 10)       printColor(COL_WHITE);      // White
    else if (stacks <= 30)  printColor(COL_GREEN);      // Green
    else if (stacks <= 50)  printColor(COL_CYAN);       // Cyan
    else if (stacks <= 75)  printColor(COL_MAGENTA);    // Magenta
    else if (stacks <= 100) printColor(COL_YELLOW);     // Yellow
    else                    printColor(COL_BRIGHT_RED); // Red (insane!)
}
```

This provides visual feedback for how powerful a blessing has become.

## 🔮 Future Enhancements

1. **Blessing Tiers**: Upgrade blessings to stronger versions
2. **Negative Blessings**: High risk, high reward curses
3. **Set Bonuses**: Synergy bonuses for collecting related blessings
4. **Unique Blessings**: One-per-run special blessings
5. **Blessing Removal**: Ability to remove unwanted blessings
6. **Metamorphosis**: Transform one blessing into another

---

**Last Updated**: December 2025