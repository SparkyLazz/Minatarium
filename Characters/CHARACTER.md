# Character System Documentation

## 📋 Overview

The Character system manages all entities in the game (player and enemies), including their stats, blessings, status effects, and combat state. It implements a sophisticated stat management system that prevents bugs through immutable base stats and calculated derived stats.

## 🎯 Design Philosophy

1. **Immutable Base Stats**: Base stats never change, preventing corruption
2. **Calculated Current Stats**: Current = Base + Bonus (always fresh)
3. **Separation of Concerns**: Stats, combat state, and effects are separate
4. **Type Safety**: Strong typing prevents invalid state combinations

## 📁 File Structure

```
Characters/
├── Character.h      # Data structures and interface
└── CharacterUtil.c  # Implementation of all character logic
```

## 🗂️ Data Structures

### Stat Block

```c
typedef struct {
    // Core Stats
    long long hp;              // Hit points
    long long attack;          // Base damage
    long long defense;         // Damage reduction
    
    // Offensive Stats (percentages)
    int criticalChance;        // 0-50 (capped)
    int criticalDamage;        // Crit multiplier
    int damageBoost;           // Overall damage increase
    int accuracy;              // Hit chance (0-100)
    
    // Resistances (0-75 capped)
    int fireResistance;
    int iceResistance;
    int poisonResistance;
    
    // Sustain Stats
    int lifeSteal;             // % of damage healed
    int regen;                 // % of max HP per turn
} StatBlock;
```

### Combat Attribute (The Core System)

```c
typedef struct {
    long long maxHP;        // Calculated from base + bonus HP
    long long currentHP;    // Dynamic HP value (0 to maxHP)

    StatBlock base;         // NEVER changes after creation
    StatBlock bonus;        // FROM blessings/equipment
    StatBlock current;      // base + bonus (READ-ONLY, calculated)
} CombatAttribute;
```

**Critical Design Decision:**
```
┌──────────────────────────────────────┐
│        STAT FLOW DIAGRAM             │
│                                      │
│  Character Creation                  │
│         │                            │
│         ▼                            │
│    [BASE STATS] ◄── IMMUTABLE       │
│         │                            │
│         │                            │
│    Add Blessing                      │
│         │                            │
│         ▼                            │
│   [BONUS STATS] ◄── Accumulated     │
│         │                            │
│         │                            │
│  RecalculateStats()                  │
│         │                            │
│         ▼                            │
│  [CURRENT STATS] ◄── base + bonus   │
│         │                            │
│         ▼                            │
│    Used in Combat                    │
└──────────────────────────────────────┘
```

### Combat State

```c
typedef struct {
    int isDefending;        // Currently in defensive stance
    int damageReduction;    // Percentage (e.g., 50 = 50% reduction)
    int reflectChance;      // Chance to reflect damage
} CombatState;
```

**Important**: Combat state is temporary and cleared at the end of each turn.

### Character Type

```c
typedef enum {
    PLAYER,    // Human-controlled character
    NORMAL,    // Regular enemy (1x multiplier)
    ELITE,     // Stronger enemy (1.4-1.8x multiplier)
    BOSS       // Boss enemy (1.8-3.0x multiplier)
} CharacterType;
```

### Main Character Structure

```c
typedef struct {
    char name[100];              // Character name
    CharacterType type;          // Entity type
    
    CombatAttribute attribute;   // All stats
    CombatState combatState;     // Temporary combat flags
    
    Blessing currentBlessing[100]; // Up to 100 blessing slots
    int blessingCount;           // Number of blessings owned
    
    Status currentStatus[100];   // Active status effects
    int statusCount;             // Number of active statuses
} Character;
```

## 🔄 Core Functions

### 1. Stat Recalculation

```c
void RecalculateStats(Character* character) {
    // STEP 1: Calculate max HP
    character->attribute.maxHP = 
        character->attribute.base.hp + character->attribute.bonus.hp;
    
    // STEP 2: Clamp current HP
    if (character->attribute.currentHP > character->attribute.maxHP) {
        character->attribute.currentHP = character->attribute.maxHP;
    }
    
    // STEP 3: Current stats = base + bonus
    character->attribute.current.attack = 
        character->attribute.base.attack + 
        character->attribute.bonus.attack;
    
    // STEP 4: Apply caps to percentage stats
    character->attribute.current.criticalChance = 
        character->attribute.base.criticalChance + 
        character->attribute.bonus.criticalChance;
    
    if (character->attribute.current.criticalChance > MAX_CRIT_CHANCE) {
        character->attribute.current.criticalChance = MAX_CRIT_CHANCE;
    }
    
    // ... repeat for all stats with appropriate capping
}
```

**When to Call:**
- After adding/removing blessings
- After initialization
- When loading saved character
- Before displaying stats

**Never Modify current stats directly!**

### 2. Character Initialization

```c
void InitializeCharacterStats(Character* character, 
                              StatBlock baseStats, 
                              long long baseHP) {
    // Set immutable base stats
    character->attribute.base = baseStats;
    character->attribute.base.hp = baseHP;
    
    // Zero out bonus stats
    memset(&character->attribute.bonus, 0, sizeof(StatBlock));
    
    // Initialize HP
    character->attribute.maxHP = baseHP;
    character->attribute.currentHP = baseHP;
    
    // Calculate current stats
    RecalculateStats(character);
    
    // Clear combat state
    ClearCombatState(character);
}
```

### 3. Blessing Management

```c
void CharacterAddBlessing(Character* target, const Blessing* blessing) {
    // STEP 1: Check for existing blessing
    int existingIndex = -1;
    for (int i = 0; i < target->blessingCount; i++) {
        if (target->currentBlessing[i].id == blessing->id) {
            existingIndex = i;
            break;
        }
    }
    
    // STEP 2: Stack or add new
    if (existingIndex >= 0) {
        // Increment stacks
        target->currentBlessing[existingIndex].stacks += 1;
    } else {
        // Add as new blessing with 1 stack
        target->currentBlessing[target->blessingCount] = *blessing;
        target->currentBlessing[target->blessingCount].stacks = 1;
        target->blessingCount++;
    }
    
    // STEP 3: Apply blessing effects to BONUS stats
    for (int i = 0; i < blessing->effectsCount; i++) {
        float effectValue = blessing->effects[i].baseValue;
        
        switch (blessing->effects[i].type) {
            case DAMAGE_BOOST:
                target->attribute.bonus.damageBoost += 
                    (int)(effectValue * 100.0f);
                break;
            case HP_BOOST:
                // Scale with base HP
                target->attribute.bonus.hp += 
                    (long long)(target->attribute.base.hp * effectValue);
                break;
            // ... handle all effect types
        }
    }
    
    // STEP 4: CRITICAL - Recalculate everything
    RecalculateStats(target);
}
```

### 4. Status Effect Management

```c
void CharacterAddStatus(Character* character, const Status* status) {
    // Check if status already exists
    for (int i = 0; i < character->statusCount; i++) {
        if (character->currentStatus[i].type == status->type) {
            // Extend duration instead of stacking
            character->currentStatus[i].duration += 1;
            return;
        }
    }
    
    // Add new status
    if (character->statusCount < 100) {
        character->currentStatus[character->statusCount] = *status;
        character->statusCount++;
    }
}
```

### 5. Combat State Management

```c
void SetDefendingStance(Character* character) {
    character->combatState.isDefending = 1;
    character->combatState.damageReduction = 50;  // 50% reduction
    character->combatState.reflectChance = 0;
}

void ClearCombatState(Character* character) {
    // Called at end of turn
    character->combatState.isDefending = 0;
    character->combatState.damageReduction = 0;
    character->combatState.reflectChance = 0;
}
```

**Important**: Combat state is temporary and must be cleared each turn.

## 🎮 Character Generation

### Player Generation

```c
Character GeneratePlayer(const char* playerName) {
    Character player = {0};
    player.type = PLAYER;
    strcpy(player.name, playerName);
    
    // Define base stats
    StatBlock baseStats = {0};
    baseStats.attack = 30;
    baseStats.defense = 15;
    baseStats.criticalChance = 20;
    baseStats.criticalDamage = 80;
    baseStats.accuracy = 75;
    // ... other stats
    
    // Initialize with 300 base HP
    InitializeCharacterStats(&player, baseStats, 300);
    
    return player;
}
```

### Enemy Generation (Scaling Algorithm)

```c
Character GenerateEnemy(int floor) {
    Character enemy = {0};
    
    // STEP 1: Determine enemy type based on floor
    CharacterType type = DetermineEnemyType(floor);
    
    // STEP 2: Set type multipliers
    float hpMult, atkMult, defMult;
    switch(type) {
        case NORMAL:
            hpMult = 1.0f;
            atkMult = 1.0f;
            defMult = 1.0f;
            break;
        case ELITE:
            hpMult = 1.8f;
            atkMult = 1.4f;
            defMult = 1.3f;
            break;
        case BOSS:
            hpMult = 3.0f;
            atkMult = 1.8f;
            defMult = 1.6f;
            break;
    }
    
    // STEP 3: Calculate scaled stats
    long long scaledHP = ScaleHP(baseHP, floor, hpMult);
    long long scaledAtk = ScaleLinearStat(baseAtk, floor, atkMult);
    long long scaledDef = ScaleLinearStat(baseDef, floor, defMult);
    
    // STEP 4: Initialize character
    InitializeCharacterStats(&enemy, baseStats, scaledHP);
    
    // STEP 5: Add blessings
    int blessingCount = CalculateBlessingCount(floor, type);
    for (int i = 0; i < blessingCount; i++) {
        Blessing* blessing = GetRandomBlessing();
        blessing->stacks = CalculateBlessingStacks(floor, type);
        CharacterAddBlessing(&enemy, blessing);
    }
    
    return enemy;
}
```

### Scaling Formulas

#### HP Scaling (Exponential)
```c
static long long ScaleHP(long long base, int floor, float multiplier) {
    return (long long)(
        (double)base 
        * pow(1.15, floor * 0.8)      // Exponential growth
        * (1.0 + floor * 0.3)          // Linear component
        * multiplier                    // Type multiplier
    );
}
```

**Example:**
```
Floor 1 Normal:  80 HP
Floor 10 Normal: 412 HP
Floor 50 Normal: 28,431 HP
Floor 10 Boss:   1,236 HP (3x multiplier)
```

#### Linear Stat Scaling
```c
static long long ScaleLinearStat(long long base, int floor, float multiplier) {
    return (long long)(
        (double)base 
        * pow(1.12, floor * 0.7)      // Moderate exponential
        + (floor * 2.5 * multiplier)   // Linear component
    );
}
```

#### Percentage Stat Scaling
```c
static int ScalePercentage(int base, int floor, float multiplier, int cap) {
    int scaled = base + (int)(log2(floor + 1) * 5 * multiplier);
    return (scaled > cap) ? cap : scaled;
}
```

### Enemy Type Determination

```c
CharacterType DetermineEnemyType(int floor) {
    if (floor % 10 == 0) return BOSS;         // Every 10th floor
    if (floor % 7 == 0 || 
        floor % 8 == 0 || 
        floor % 9 == 0) return ELITE;         // Floors 7,8,9,17,18,19...
    return NORMAL;                             // All other floors
}
```

## 📊 Stat Caps

```c
#define MAX_CRIT_CHANCE 50  // Critical chance hard cap
```

| Stat | Min | Max | Notes |
|------|-----|-----|-------|
| Critical Chance | 0 | 50 | Hard capped to prevent 100% crit |
| Accuracy | 0 | 100 | Can't exceed perfect accuracy |
| Fire Resistance | 0 | 75 | Caps prevent immunity |
| Ice Resistance | 0 | 75 | Caps prevent immunity |
| Poison Resistance | 0 | 75 | Caps prevent immunity |
| HP | 0 | Unlimited | Can grow infinitely |
| Attack | 0 | Unlimited | Can grow infinitely |
| Defense | 0 | Unlimited | Can grow infinitely |

## 🎨 Character Rendering

### Stats Tab

```c
void CharacterStatsTab(void* data) {
    Character* character = (Character*)data;
    
    // Display in organized sections
    printf("Primary Stats\n");
    printf("   HP: %lld / %lld\n", 
           character->attribute.currentHP, 
           character->attribute.maxHP);
    
    printf("   Attack: %lld (base: %lld, bonus: %lld)\n",
           character->attribute.current.attack,
           character->attribute.base.attack,
           character->attribute.bonus.attack);
    
    // ... more stat displays
}
```

### Blessing Tab

```c
void CharacterBlessingTab(void* data) {
    Character* character = (Character*)data;
    
    for (int i = 0; i < character->blessingCount; i++) {
        // Display blessing name with color based on rarity
        printf("[%d] %s\n", i + 1, 
               character->currentBlessing[i].name);
        
        // Show stacks with color coding
        BlessingStackColor(character->currentBlessing[i].stacks);
        
        // Display all effects
        for (int j = 0; j < character->currentBlessing[i].effectsCount; j++) {
            // Show base value and total value (base × stacks)
        }
    }
}
```

### Tab Panel System

```c
void CharacterRenderer(Character* character) {
    Tab tabs[2] = {
        {"Attribute Stat", CharacterStatsTab},
        {"Blessing Stat", CharacterBlessingTab}
    };
    
    // Show interactive tab panel
    showTabPanel(tabs, 2, character);
}
```

## 🔧 Common Operations

### Check if Character is Alive

```c
if (character->attribute.currentHP > 0) {
    // Character is alive
}
```

### Damage Character

```c
character->attribute.currentHP -= damageAmount;
if (character->attribute.currentHP < 0) {
    character->attribute.currentHP = 0;
}
```

### Heal Character

```c
character->attribute.currentHP += healAmount;
if (character->attribute.currentHP > character->attribute.maxHP) {
    character->attribute.currentHP = character->attribute.maxHP;
}
```

### Check for Status Effect

```c
int HasStatus(Character* character, StatusType type) {
    for (int i = 0; i < character->statusCount; i++) {
        if (character->currentStatus[i].type == type) {
            return 1;
        }
    }
    return 0;
}
```

## 🐛 Common Pitfalls

### ❌ WRONG: Modifying current stats directly

```c
// DON'T DO THIS!
character->attribute.current.attack += 10;
```

### ✅ CORRECT: Modify bonus, then recalculate

```c
character->attribute.bonus.attack += 10;
RecalculateStats(character);
```

### ❌ WRONG: Modifying base stats

```c
// DON'T DO THIS!
character->attribute.base.defense += 5;
```

### ✅ CORRECT: Base stats should never change

```c
// Base stats are set once during InitializeCharacterStats
// and never modified again
```

### ❌ WRONG: Forgetting to recalculate after blessing

```c
CharacterAddBlessing(player, blessing);
// Oops! Stats are outdated now
```

### ✅ CORRECT: Always recalculate (done automatically)

```c
CharacterAddBlessing(player, blessing);
// RecalculateStats() is called inside CharacterAddBlessing
```

## 📈 Stat Progression Example

```
Player starts:
    Base: 30 ATK, 15 DEF, 300 HP
    Bonus: 0
    Current: 30 ATK, 15 DEF, 300 HP

Floor 5 - Get "Minor Strength" (+5% damage boost):
    Base: 30 ATK (unchanged)
    Bonus: +5 damage boost
    Current: 30 ATK, +5% = effective 31.5 ATK

Floor 10 - Get "Minor Strength" again (now 2 stacks):
    Base: 30 ATK (unchanged)
    Bonus: +10 damage boost
    Current: 30 ATK, +10% = effective 33 ATK

Floor 15 - Get "Vitality" (+10% HP):
    Base: 300 HP (unchanged)
    Bonus: +30 HP (10% of base)
    Current: 330 max HP

Floor 20 - Get "Vitality" again (now 2 stacks):
    Base: 300 HP (unchanged)
    Bonus: +60 HP (20% of base)
    Current: 360 max HP
```

## 🚀 Advanced Topics

### Custom Stat Effects

To add a new stat effect:

1. Add stat to StatBlock
```c
typedef struct {
    // ... existing stats
    int newStat;
} StatBlock;
```

2. Handle in RecalculateStats
```c
character->attribute.current.newStat = 
    character->attribute.base.newStat + 
    character->attribute.bonus.newStat;
```

3. Add blessing effect type
```c
typedef enum {
    // ... existing effects
    NEW_STAT_BOOST,
} BlessingEffectType;
```

4. Handle in CharacterAddBlessing
```c
case NEW_STAT_BOOST:
    target->attribute.bonus.newStat += (int)(value * 100.0f);
    break;
```

### Temporary Buffs

For buffs that last X turns:

```c
typedef struct {
    int turnsRemaining;
    StatBlock tempBonus;
} TemporaryBuff;
```

Apply before turn, remove after turn ends.

## 🎯 Best Practices

1. **Always use RecalculateStats()** after any stat modification
2. **Never modify base stats** after initialization
3. **Use bonus for all modifications** from blessings/equipment
4. **Clear combat state** at end of each turn
5. **Cap percentage stats** to prevent overflow
6. **Clamp HP** between 0 and maxHP
7. **Use long long** for stats that can grow large (HP, Attack, Defense)
8. **Use int** for percentage stats (they're smaller and capped)

## 🔮 Future Enhancements

1. **Equipment System**: Separate from blessings, removable
2. **Stat Conversion**: Convert one stat to another
3. **Diminishing Returns**: Higher stacks = reduced effectiveness
4. **Stat Synergies**: Bonuses for having related stats
5. **Character Classes**: Different base stat distributions
6. **Prestige System**: Reset with permanent bonuses
7. **Stat Respec**: Redistribute stat points
8. **Level System**: Independent progression from floors

---

**Last Updated**: December 2025