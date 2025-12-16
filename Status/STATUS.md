# Status Effect System Documentation

## 📋 Overview

The Status system manages temporary debuffs and crowd control effects that can be applied to characters during combat. Status effects add tactical depth by creating damage-over-time (DoT), crowd control (CC), and strategic decisions around timing and resistance.

## 🎯 Design Philosophy

1. **Simple but Impactful**: Each status has a clear, understandable effect
2. **Limited Duration**: Status effects are temporary, not permanent
3. **Stacking by Duration**: Same status extends duration instead of stacking damage
4. **Counterplay**: Resistances reduce effectiveness
5. **Visual Feedback**: Status icons clearly communicate active effects

## 📁 File Structure

```
Status/
└── Status.h  # Complete status system definition
```

**Note**: The Status system is intentionally simple and header-only. All logic is implemented in the Game and Character modules.

## 🗂️ Data Structures

### Status Type Enum

```c
typedef enum {
    BURN,      // Fire DoT - deals damage each turn
    POISON,    // Poison DoT - deals damage each turn, ignores defense
    STUN,      // Crowd control - prevents action for 1 turn
    FREEZE,    // Crowd control - prevents action for multiple turns
} StatusType;
```

### Status Structure

```c
typedef struct {
    StatusType type;      // Which status effect
    float baseAmount;     // Damage per turn (for DoT effects)
    int duration;         // Turns remaining
} Status;
```

## 📊 Status Effect Details

### 1. Burn

**Type**: Damage over Time (DoT)  
**Source**: Fire damage blessings  
**Effect**: Deals damage each turn based on attacker's fire damage stat

```c
Status burnStatus = {
    .type = BURN,
    .baseAmount = 10.0f,  // 10 damage per turn
    .duration = 3         // Lasts 3 turns
};
```

**Characteristics:**
- Damage scales with blessing stacks
- Reduced by defender's fire resistance
- Applied on hit based on chance (typically 30-70%)
- Visual indicator: Red "F" icon

**Application Flow:**
```
Attacker has Fire Damage blessing
    ↓
Roll chance to apply (e.g., 35%)
    ↓
If success, create Burn status
    ↓
baseAmount = blessing base × stacks
    ↓
Add to defender's status list
```

### 2. Poison

**Type**: Damage over Time (DoT)  
**Source**: Poison damage blessings  
**Effect**: Deals damage each turn, bypasses defense

```c
Status poisonStatus = {
    .type = POISON,
    .baseAmount = 8.0f,   // 8 damage per turn
    .duration = 5         // Lasts 5 turns
};
```

**Characteristics:**
- Damage scales with blessing stacks
- Reduced by defender's poison resistance
- Applied on hit based on chance (typically 50-70%)
- Visual indicator: Green "P" icon
- Longer duration than burn

**Key Difference from Burn:**
- Poison typically deals less damage per tick
- But lasts longer (5 turns vs 3 turns)
- Making poison better for sustained damage

### 3. Stun

**Type**: Crowd Control (CC)  
**Source**: [Currently unused, reserved for future blessings]  
**Effect**: Target cannot act for the duration

```c
Status stunStatus = {
    .type = STUN,
    .baseAmount = 0.0f,   // No damage
    .duration = 1         // Typically 1 turn
};
```

**Characteristics:**
- Completely prevents action
- No damage component
- Visual indicator: Yellow "S" icon
- Short duration (usually 1 turn)

**Incapacitation Check:**
```c
int IsIncapacitated(const Character* character) {
    for (int i = 0; i < character->statusCount; i++) {
        if (character->currentStatus[i].type == STUN) {
            return 1;  // Character cannot act
        }
    }
    return 0;
}
```

### 4. Freeze

**Type**: Crowd Control (CC)  
**Source**: Ice damage blessings  
**Effect**: Target cannot act for the duration

```c
Status freezeStatus = {
    .type = FREEZE,
    .baseAmount = 0.0f,   // No damage
    .duration = 2         // Typically 2-3 turns
};
```

**Characteristics:**
- Completely prevents action
- No damage component
- Reduced by defender's ice resistance
- Visual indicator: Cyan "I" icon
- Longer duration than stun (2-3 turns)

**Key Difference from Stun:**
- Freeze lasts longer
- Associated with ice damage
- Can be resisted

## 🔄 Status Effect Lifecycle

### 1. Application

```c
// In GameDamage.c
void CollectDoTEffects(const Character* attacker, DamageResult* result) {
    result->statusCount = 0;
    
    // Check all blessings for DoT effects
    for (int i = 0; i < attacker->blessingCount; i++) {
        const Blessing* blessing = &attacker->currentBlessing[i];
        
        // Check each DoT in the blessing
        for (int j = 0; j < blessing->dotsCount; j++) {
            // Roll chance to apply
            if (RollChance(blessing->dots[j].change)) {
                // Add to result
                Status status = blessing->dots[j].DoT;
                
                // Scale damage with stacks
                status.baseAmount *= (float)blessing->stacks;
                
                result->appliedStatuses[result->statusCount] = status;
                result->statusCount++;
            }
        }
    }
}
```

### 2. Addition to Character

```c
// In CharacterUtil.c
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

**Design Decision**: Status effects don't stack in power, only in duration. This prevents runaway status effect damage while still making them relevant.

### 3. Processing (Each Turn)

```c
// In GameDamage.c
void ProcessStatusEffects(Character* character) {
    for (int i = 0; i < character->statusCount; i++) {
        Status* status = &character->currentStatus[i];
        
        switch (status->type) {
            case BURN:
                character->attribute.currentHP -= 
                    (long long)status->baseAmount;
                AddCombatLog("%s takes %lld burn damage", 
                            character->name, 
                            (long long)status->baseAmount);
                break;
            
            case POISON:
                character->attribute.currentHP -= 
                    (long long)status->baseAmount;
                AddCombatLog("%s takes %lld poison damage", 
                            character->name, 
                            (long long)status->baseAmount);
                break;
            
            case STUN:
            case FREEZE:
                AddCombatLog("%s is incapacitated!", 
                            character->name);
                break;
        }
        
        // Decrement duration
        status->duration--;
        
        // Remove if expired
        if (status->duration <= 0) {
            // Log expiration
            AddCombatLog("%s's %s wore off", 
                        character->name, 
                        GetStatusName(status->type));
            
            // Remove by shifting array
            for (int j = i; j < character->statusCount - 1; j++) {
                character->currentStatus[j] = 
                    character->currentStatus[j + 1];
            }
            character->statusCount--;
            i--;  // Adjust index
        }
    }
}
```

### 4. Visual Display

```c
// In GameLoop.c
void DrawStatusIcons(const Character* character) {
    if (character->statusCount == 0) return;
    
    printf(" [");
    
    // Show up to 5 status icons
    for (int i = 0; i < character->statusCount && i < 5; i++) {
        switch (character->currentStatus[i].type) {
            case BURN:
                printColor(COL_RED, "F");    // Fire
                break;
            case POISON:
                printColor(COL_GREEN, "P");  // Poison
                break;
            case STUN:
                printColor(COL_YELLOW, "S"); // Stun
                break;
            case FREEZE:
                printColor(COL_CYAN, "I");   // Ice
                break;
        }
    }
    
    printf("]");
}
```

**Display Example:**
```
Player HP: [████████████░░░░] 450/600 [F][P][I]
           ^                          ^  ^  ^
           HP Bar                     |  |  |
                              Burn ---|  |  |
                                Poison ---|  |
                                    Freeze ---|
```

## 📐 Damage Calculation

### DoT Damage Formula

```
Actual Damage = baseAmount × (1 - Resistance%)

Where:
    baseAmount = blessing base × stacks
    Resistance = Fire/Poison/Ice Resistance stat
```

### Example Calculations

**Burn Example:**
```
Attacker:
    - Phoenix Blessing: 10 base burn, 50% chance
    - 5 stacks

Defender:
    - 20% fire resistance

Calculation:
    1. Roll 50% chance → Success
    2. Base damage: 10 × 5 stacks = 50
    3. Resistance reduction: 50 × (1 - 0.20) = 40
    4. Final: 40 damage per turn for 3 turns
```

**Poison Example:**
```
Attacker:
    - Toxic Mastery: 8 base poison, 60% chance
    - 10 stacks

Defender:
    - 0% poison resistance

Calculation:
    1. Roll 60% chance → Success
    2. Base damage: 8 × 10 stacks = 80
    3. No resistance: 80 × (1 - 0) = 80
    4. Final: 80 damage per turn for 5 turns
    5. Total: 400 damage over full duration
```

## 🎯 Strategic Considerations

### DoT vs Direct Damage

**Advantages of DoT:**
- Guaranteed damage even if you die
- Can't be dodged after applied
- Stacks with other damage sources
- Good for wearing down high-defense enemies

**Disadvantages of DoT:**
- Takes time to deal full damage
- Can be resisted
- Enemies might die before full duration
- Only extends duration, doesn't stack damage

### CC Effects (Stun/Freeze)

**Strategic Uses:**
- Skip dangerous enemy attacks
- Buy time for healing/buffs
- Interrupt enemy combos
- Set up for follow-up attacks

**Counterplay:**
- Ice resistance reduces freeze chance
- Can't chain CC infinitely (duration extension is limited)
- Player can still defend while frozen

## 🐛 Common Issues & Solutions

### Issue: Status Effects Not Dealing Damage

**Problem**: Status added but no damage occurs.

**Solution**: Ensure `ProcessStatusEffects()` is called at the **start** of turn:
```c
void StartTurn(Character* character) {
    ClearCombatState(character);
    RecalculateStats(character);
    ProcessStatusEffects(character);  // MUST be here, not in EndTurn
    ProcessRegeneration(character);
}
```

### Issue: Status Effects Stack Infinitely

**Problem**: Same status applied multiple times creates multiple entries.

**Solution**: Check for existing status by type:
```c
// In CharacterAddStatus
for (int i = 0; i < character->statusCount; i++) {
    if (character->currentStatus[i].type == status->type) {
        // Found existing, just extend duration
        character->currentStatus[i].duration += 1;
        return;  // Don't add new entry
    }
}
```

### Issue: Frozen Character Can Still Act

**Problem**: Freeze status doesn't prevent actions.

**Solution**: Check `IsIncapacitated()` before allowing action:
```c
if (IsIncapacitated(character)) {
    AddCombatLog("Character is incapacitated!");
    return;  // Skip action
}
```

### Issue: Status Never Expires

**Problem**: Status effects last forever.

**Solution**: Decrement duration and remove when <= 0:
```c
status->duration--;
if (status->duration <= 0) {
    // Remove status by shifting array
}
```

## 🔧 Adding New Status Effects

### Step 1: Add to StatusType Enum

```c
// In Status.h
typedef enum {
    BURN,
    POISON,
    STUN,
    FREEZE,
    BLEED,     // NEW: Physical DoT
} StatusType;
```

### Step 2: Handle in ProcessStatusEffects

```c
case BLEED:
    // Calculate damage (maybe based on max HP?)
    long long bleedDamage = (long long)(
        (float)character->attribute.maxHP * 0.02f  // 2% max HP
    );
    character->attribute.currentHP -= bleedDamage;
    AddCombatLog("%s takes %lld bleed damage", 
                character->name, bleedDamage);
    break;
```

### Step 3: Add Visual Indicator

```c
case BLEED:
    printColor(COL_BRIGHT_RED, "B");  // Blood
    break;
```

### Step 4: Create Blessing That Applies It

```c
// In BlessingData.c
{
    .id = 18,
    .name = "Hemorrhage",
    .description = "Causes enemies to bleed",
    .rarity = RARITY_RARE,
    .stacks = 0,
    .effects = {{DAMAGE_BOOST, 0.10f}},
    .effectsCount = 1,
    .dots = {{{BLEED, 0.0f, 4}, 40}},  // 40% chance, 4 turns
    .dotsCount = 1
}
```

### Step 5: Add Status Name Mapping

```c
const char* GetStatusName(StatusType type) {
    switch(type) {
        case BURN: return "Burn";
        case POISON: return "Poison";
        case STUN: return "Stun";
        case FREEZE: return "Freeze";
        case BLEED: return "Bleed";  // NEW
        default: return "Unknown";
    }
}
```

## 📊 Status Effect Comparison

| Status | Type | Damage | Duration | Resistance | Icon |
|--------|------|--------|----------|------------|------|
| Burn | DoT | High (10/turn) | Short (3t) | Fire Res | F (Red) |
| Poison | DoT | Medium (8/turn) | Long (5t) | Poison Res | P (Green) |
| Stun | CC | None | Very Short (1t) | None | S (Yellow) |
| Freeze | CC | None | Short (2-3t) | Ice Res | I (Cyan) |

### Total Damage Potential

Assuming no resistance:

| Status | Base Damage | Stacks | Turns | Total Damage |
|--------|-------------|--------|-------|--------------|
| Burn | 10 | 1 | 3 | 30 |
| Burn | 10 | 5 | 3 | 150 |
| Burn | 10 | 10 | 3 | 300 |
| Poison | 8 | 1 | 5 | 40 |
| Poison | 8 | 5 | 5 | 200 |
| Poison | 8 | 10 | 5 | 400 |

**Observation**: Poison deals more total damage than burn at high stacks due to longer duration.

## 🎮 Status Effect Strategies

### Offensive DoT Build

```
Focus: Maximize status damage
Blessings:
- Elemental Chaos (all 3 DoTs)
- Phoenix Blessing (burn)
- Toxic Mastery (poison)
- High blessing stacks

Result: 
- Multiple DoTs active simultaneously
- Hundreds of damage per turn from status alone
- Can kill enemies without attacking
```

### CC Control Build

```
Focus: Prevent enemy actions
Blessings:
- Frost Strike (freeze)
- [Future stun blessings]
- High application chance

Result:
- Enemies frequently can't act
- Safer combat at the cost of slower kills
- Good for survival-focused runs
```

### Hybrid Build

```
Focus: Balance direct damage and DoT
Blessings:
- Divine Wrath (raw power)
- Phoenix Blessing (fire DoT)
- Life Drain (sustain)

Result:
- High burst damage
- Burn for finish-off
- Sustainable through lifesteal
```

## 🔬 Advanced Mechanics

### Status Effect Interaction with Resistance

```c
// In CalculateDamage (for application chance)
float effectiveChance = baseChance * (1.0f - resistance / 100.0f);

Example:
    Base chance: 50%
    Ice resistance: 30%
    Effective chance: 50% × 0.70 = 35%
```

### Status Cleansing (Future Feature)

```c
void RemoveStatusByType(Character* character, StatusType type) {
    for (int i = 0; i < character->statusCount; i++) {
        if (character->currentStatus[i].type == type) {
            // Shift array to remove
            for (int j = i; j < character->statusCount - 1; j++) {
                character->currentStatus[j] = 
                    character->currentStatus[j + 1];
            }
            character->statusCount--;
            i--;
        }
    }
}
```

### Status Immunity (Future Feature)

```c
typedef struct {
    StatusType immuneTo[4];
    int immunityCount;
} StatusImmunity;

// Check before applying
if (IsImmuneToStatus(character, status->type)) {
    return;  // Don't apply
}
```

## 📈 Balance Considerations

### DoT Scaling

**Current System:**
- Damage scales linearly with stacks
- Duration is fixed
- Result: Predictable, stackable damage

**Alternative Systems:**
1. **Diminishing Returns**: Each stack adds less damage
2. **Duration Stacking**: Longer duration per stack
3. **Intensity Stacking**: More damage per tick, same duration

### Resistance Caps

- Current cap: 75% resistance
- Prevents total immunity
- Still allows 25% effectiveness at max

### CC Duration Limits

- Stun: 1-2 turns max
- Freeze: 2-3 turns max
- Prevents permanent lockdown
- Duration extension limited to prevent abuse

## 🔮 Future Status Effects

### Potential Additions

1. **Bleed** - Physical DoT, scales with max HP
2. **Vulnerable** - Increases damage taken
3. **Weakened** - Reduces damage dealt
4. **Corroded** - Reduces defense over time
5. **Enraged** - Increases attack but reduces defense
6. **Regeneration** - Opposite of DoT, healing over time
7. **Shielded** - Absorbs X damage before breaking
8. **Invisible** - Cannot be targeted for 1 turn
9. **Rooted** - Cannot defend (if defend mechanics expand)
10. **Cursed** - DoT that ignores all resistances

### Buff Status Effects

Currently only debuffs exist. Consider adding buffs:
```c
typedef enum {
    // Debuffs
    BURN, POISON, STUN, FREEZE,
    
    // Buffs
    HASTE,          // +damage boost for X turns
    FORTIFY,        // +defense for X turns
    BLESSED,        // +crit chance for X turns
    EMPOWERED,      // Next attack deals 2x damage
} StatusType;
```

## 🎯 Best Practices

1. **Always check incapacitation** before allowing actions
2. **Process status at turn start**, not end
3. **Clamp HP after DoT damage** (can't go below 0)
4. **Log status application and expiration** for clarity
5. **Limit status duration extension** to prevent infinite CC
6. **Cap resistances** to ensure statuses always have some effect
7. **Show status icons** for clear visual feedback
8. **Remove expired statuses immediately** to avoid clutter

---

**Last Updated**: December 2025