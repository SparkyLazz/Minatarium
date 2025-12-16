# Game System Documentation

## 📋 Overview

The Game system is the heart of Minatarium, managing combat, damage calculation, rewards, and the main game loop. It orchestrates all interactions between characters, blessings, and status effects to create engaging turn-based gameplay.

## 🎯 Design Philosophy

1. **Deterministic Combat**: Same inputs always produce same outputs
2. **Transparent Calculations**: Players can understand damage sources
3. **Balanced Progression**: Difficulty scales smoothly
4. **Fair RNG**: Random elements add variety without feeling unfair

## 📁 File Structure

```
Game/
├── Game.h          # System interface and structures
├── GameLoop.c      # Main game loop, UI, turn management
├── GameDamage.c    # Damage calculation engine
└── GameReward.c    # Blessing reward system
```

## 🗂️ Data Structures

### Damage Result

```c
typedef struct {
    long long rawDamage;        // Damage before defense
    long long finalDamage;      // Damage after all calculations
    int isCritical;             // Was this a critical hit?
    int didMiss;                // Did the attack miss?
    long long lifeStealAmount;  // HP healed from lifesteal
    long long thornDamage;      // Damage reflected back
    Status appliedStatuses[10]; // Status effects applied
    int statusCount;            // Number of statuses applied
} DamageResult;
```

### Combat Log System

```c
typedef struct {
    char logs[MAX_COMBAT_LOG][256];  // Circular buffer
    int count;                       // Current log count
} CombatLog;
```

## 🔄 Damage Calculation Flow

### Complete Damage Pipeline

```c
DamageResult CalculateDamage(Character* attacker, Character* defender) {
    DamageResult result = {0};
    
    // ═══════════════════════════════════════════════════════
    // STEP 1: ACCURACY CHECK
    // ═══════════════════════════════════════════════════════
    int hitChance = attacker->attribute.current.accuracy;
    if (!RollChance(hitChance)) {
        result.didMiss = 1;
        return result;  // Early exit on miss
    }
    
    // ═══════════════════════════════════════════════════════
    // STEP 2: BASE DAMAGE CALCULATION
    // ═══════════════════════════════════════════════════════
    long long baseDamage = attacker->attribute.current.attack;
    
    // ═══════════════════════════════════════════════════════
    // STEP 3: DAMAGE BOOST APPLICATION
    // ═══════════════════════════════════════════════════════
    float damageMultiplier = 1.0f + 
        ((float)attacker->attribute.current.damageBoost / 100.0f);
    baseDamage = (long long)((float)baseDamage * damageMultiplier);
    
    // ═══════════════════════════════════════════════════════
    // STEP 4: CRITICAL HIT CHECK
    // ═══════════════════════════════════════════════════════
    result.isCritical = RollChance(
        attacker->attribute.current.criticalChance
    );
    
    if (result.isCritical) {
        float critMultiplier = 1.0f + 
            ((float)attacker->attribute.current.criticalDamage / 100.0f);
        baseDamage = (long long)((float)baseDamage * critMultiplier);
    }
    
    result.rawDamage = baseDamage;
    
    // ═══════════════════════════════════════════════════════
    // STEP 5: ARMOR PENETRATION
    // ═══════════════════════════════════════════════════════
    float armorPen = CalculateTotalBlessingEffect(
        attacker, ARMOR_PENETRATION
    );
    long long effectiveDefense = (long long)(
        (float)defender->attribute.current.defense * (1.0f - armorPen)
    );
    
    // ═══════════════════════════════════════════════════════
    // STEP 6: DEFEND STANCE REDUCTION
    // ═══════════════════════════════════════════════════════
    if (defender->combatState.isDefending) {
        float reductionMultiplier = 1.0f - 
            ((float)defender->combatState.damageReduction / 100.0f);
        baseDamage = (long long)((float)baseDamage * reductionMultiplier);
    }
    
    // ═══════════════════════════════════════════════════════
    // STEP 7: APPLY DEFENSE
    // ═══════════════════════════════════════════════════════
    long long damageAfterDefense = baseDamage - effectiveDefense;
    if (damageAfterDefense < 1) {
        damageAfterDefense = 1;  // Minimum 1 damage
    }
    
    // ═══════════════════════════════════════════════════════
    // STEP 8: ELEMENTAL DAMAGE
    // ═══════════════════════════════════════════════════════
    float fireDmg = CalculateTotalBlessingEffect(attacker, FIRE_DAMAGE);
    float iceDmg = CalculateTotalBlessingEffect(attacker, ICE_DAMAGE);
    float poisonDmg = CalculateTotalBlessingEffect(attacker, POISON_DAMAGE);
    
    long long elementalDamage = 0;
    
    // Fire damage (reduced by fire resistance)
    elementalDamage += (long long)(
        (float)baseDamage * fireDmg * 
        (1.0f - (float)defender->attribute.current.fireResistance / 100.0f)
    );
    
    // Ice damage (reduced by ice resistance)
    elementalDamage += (long long)(
        (float)baseDamage * iceDmg * 
        (1.0f - (float)defender->attribute.current.iceResistance / 100.0f)
    );
    
    // Poison damage (reduced by poison resistance)
    elementalDamage += (long long)(
        (float)baseDamage * poisonDmg * 
        (1.0f - (float)defender->attribute.current.poisonResistance / 100.0f)
    );
    
    result.finalDamage = damageAfterDefense + elementalDamage;
    
    // ═══════════════════════════════════════════════════════
    // STEP 9: LIFESTEAL CALCULATION
    // ═══════════════════════════════════════════════════════
    if (attacker->attribute.current.lifeSteal > 0) {
        result.lifeStealAmount = (long long)(
            (float)result.finalDamage * 
            ((float)attacker->attribute.current.lifeSteal / 100.0f)
        );
    }
    
    // ═══════════════════════════════════════════════════════
    // STEP 10: STATUS EFFECT APPLICATION
    // ═══════════════════════════════════════════════════════
    CollectDoTEffects(attacker, &result);
    
    // ═══════════════════════════════════════════════════════
    // STEP 11: THORN DAMAGE
    // ═══════════════════════════════════════════════════════
    float thornEffect = CalculateTotalBlessingEffect(defender, THORN);
    if (thornEffect > 0.0f) {
        result.thornDamage = (long long)(
            (float)result.finalDamage * thornEffect
        );
    }
    
    return result;
}
```

### Damage Formula Breakdown

```
Final Damage = 
    (Base Damage × Damage Boost × Crit Multiplier - Defense) 
    + Elemental Damage

Where:
    Base Damage = Attack Stat
    Damage Boost = 1 + (DamageBoost% / 100)
    Crit Multiplier = 1 + (CritDamage% / 100) [if crit]
    Defense = Effective Defense × (1 - Armor Penetration)
    Elemental Damage = Base × Element% × (1 - Resistance%)
```

### Example Calculation

```
Attacker Stats:
    Attack: 100
    Damage Boost: 20%
    Crit Chance: 30%
    Crit Damage: 150%
    Fire Damage: 25%
    Armor Penetration: 15%

Defender Stats:
    Defense: 40
    Fire Resistance: 10%

Calculation (assuming critical hit):
    1. Base Damage: 100
    2. With Boost: 100 × 1.20 = 120
    3. Critical: 120 × 2.50 = 300 (raw damage)
    4. Effective Defense: 40 × 0.85 = 34
    5. After Defense: 300 - 34 = 266
    6. Fire Damage: 300 × 0.25 × 0.90 = 67.5
    7. Final Damage: 266 + 67 = 333
```

## 🎮 Combat System

### Turn Structure

```c
void ExecuteCombatTurn(Character* attacker, Character* defender) {
    // ═══════════════════════════════════════════════════════
    // PHASE 1: INCAPACITATION CHECK
    // ═══════════════════════════════════════════════════════
    if (IsIncapacitated(attacker)) {
        AddCombatLog("Attacker is stunned/frozen!");
        return;  // Skip turn
    }
    
    // ═══════════════════════════════════════════════════════
    // PHASE 2: DAMAGE CALCULATION
    // ═══════════════════════════════════════════════════════
    DamageResult result = CalculateDamage(attacker, defender);
    
    // ═══════════════════════════════════════════════════════
    // PHASE 3: RESULT APPLICATION
    // ═══════════════════════════════════════════════════════
    ApplyDamageResult(attacker, defender, &result);
}
```

### Turn Management

```c
void StartTurn(Character* character) {
    // Reset temporary effects from previous turn
    ClearCombatState(character);
    
    // Ensure stats are up-to-date
    RecalculateStats(character);
    
    // Process ongoing effects
    ProcessStatusEffects(character);
    ProcessRegeneration(character);
}

void EndTurn(Character* character) {
    // Clear temporary stance effects
    ClearCombatState(character);
}
```

### Status Effect Processing

```c
void ProcessStatusEffects(Character* character) {
    for (int i = 0; i < character->statusCount; i++) {
        Status* status = &character->currentStatus[i];
        
        switch (status->type) {
            case BURN:
                // Apply damage
                character->attribute.currentHP -= 
                    (long long)status->baseAmount;
                AddCombatLog("%s takes %lld burn damage", 
                            character->name, 
                            (long long)status->baseAmount);
                break;
            
            case POISON:
                // Apply damage
                character->attribute.currentHP -= 
                    (long long)status->baseAmount;
                AddCombatLog("%s takes %lld poison damage", 
                            character->name, 
                            (long long)status->baseAmount);
                break;
            
            case STUN:
            case FREEZE:
                // Just log, actual effect checked in ExecuteCombatTurn
                AddCombatLog("%s is incapacitated!", 
                            character->name);
                break;
        }
        
        // Decrement duration
        status->duration--;
        
        // Remove if expired
        if (status->duration <= 0) {
            // Shift array to remove status
            for (int j = i; j < character->statusCount - 1; j++) {
                character->currentStatus[j] = 
                    character->currentStatus[j + 1];
            }
            character->statusCount--;
            i--;  // Adjust index after removal
        }
    }
}
```

### Regeneration Processing

```c
void ProcessRegeneration(Character* character) {
    if (character->attribute.current.regen > 0 && 
        character->attribute.currentHP < character->attribute.maxHP) {
        
        // Calculate heal amount (% of max HP)
        long long healAmount = (long long)(
            (float)character->attribute.maxHP * 
            ((float)character->attribute.current.regen / 100.0f)
        );
        
        // Apply healing
        character->attribute.currentHP += healAmount;
        
        // Clamp to max
        if (character->attribute.currentHP > character->attribute.maxHP) {
            character->attribute.currentHP = character->attribute.maxHP;
        }
        
        AddCombatLog("%s regenerated %lld HP", 
                    character->name, healAmount);
    }
}
```

## 🎁 Reward System

### Blessing Selection

```c
void BlessingWinningReward(Character* player, CharacterType enemyType) {
    // ═══════════════════════════════════════════════════════
    // STEP 1: DETERMINE RARITY RANGE
    // ═══════════════════════════════════════════════════════
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
    
    // ═══════════════════════════════════════════════════════
    // STEP 2: GENERATE 3 RANDOM CHOICES
    // ═══════════════════════════════════════════════════════
    Blessing* choices[3];
    for (int i = 0; i < 3; i++) {
        choices[i] = GetRandomBlessingInRange(
            db, minRarity, maxRarity
        );
    }
    
    // ═══════════════════════════════════════════════════════
    // STEP 3: PLAYER SELECTION UI
    // ═══════════════════════════════════════════════════════
    int selectedIndex = ShowBlessingSelectionUI(choices);
    
    // ═══════════════════════════════════════════════════════
    // STEP 4: ADD BLESSING TO CHARACTER
    // ═══════════════════════════════════════════════════════
    CharacterAddBlessing(player, choices[selectedIndex]);
}
```

### Boss Legendary Handling

```c
void BlessingBossReward(Character* player, Blessing* legendary) {
    // Check if player already has a legendary
    int hasLegendary = 0;
    int legendaryIndex = -1;
    
    for (int i = 0; i < player->blessingCount; i++) {
        if (player->currentBlessing[i].rarity >= RARITY_LEGENDARY) {
            hasLegendary = 1;
            legendaryIndex = i;
            break;
        }
    }
    
    if (!hasLegendary) {
        // Simply add the legendary
        CharacterAddBlessing(player, legendary);
    } else {
        // Offer replacement choice
        // Show current legendary vs new legendary
        // Let player decide whether to replace
        
        if (PlayerChoosesReplace()) {
            // Replace the old legendary
            player->currentBlessing[legendaryIndex] = *legendary;
            player->currentBlessing[legendaryIndex].stacks = 1;
            RecalculateStats(player);
        }
    }
}
```

**Design Decision**: Legendary blessings cannot stack. This prevents one build from dominating and encourages build diversity.

## 🔄 Game Loop

### Main Game Loop

```c
void StartGame() {
    // ═══════════════════════════════════════════════════════
    // INITIALIZATION
    // ═══════════════════════════════════════════════════════
    Character player = GeneratePlayer(playerName);
    int currentFloor = 1;
    int gameRunning = 1;
    
    // ═══════════════════════════════════════════════════════
    // MAIN LOOP
    // ═══════════════════════════════════════════════════════
    while (gameRunning) {
        // Generate enemy for current floor
        Character enemy = GenerateEnemy(currentFloor);
        
        // Run combat
        int won = RunCombat(&player, &enemy, currentFloor);
        
        if (won) {
            // ═══════════════════════════════════════════════
            // VICTORY BRANCH
            // ═══════════════════════════════════════════════
            
            // Apply post-combat healing
            ApplyPostCombatHealing(&player);
            
            // Give blessing reward
            if (enemy.type == BOSS) {
                BlessingBossReward(&player, legendaryBlessing);
            } else {
                BlessingWinningReward(&player, enemy.type);
            }
            
            // Progress to next floor
            currentFloor++;
            
            // Offer continue/quit choice
            int choice = ShowContinueMenu(currentFloor);
            if (choice == QUIT) {
                gameRunning = 0;
            }
            
        } else {
            // ═══════════════════════════════════════════════
            // DEFEAT BRANCH
            // ═══════════════════════════════════════════════
            
            // Save run data
            SaveRun(&player, currentFloor);
            
            // Show game over screen
            ShowGameOverScreen(currentFloor);
            
            // Exit loop
            gameRunning = 0;
        }
    }
}
```

### Combat Loop

```c
int RunCombat(Character* player, Character* enemy, int floor) {
    ClearCombatLog();
    AddCombatLog("Combat started!");
    
    int combatRunning = 1;
    int playerTurn = 1;
    
    while (combatRunning) {
        // Draw combat UI
        DrawCombatUI(player, enemy, floor);
        
        if (playerTurn) {
            // ═══════════════════════════════════════════════
            // PLAYER TURN
            // ═══════════════════════════════════════════════
            StartTurn(player);
            
            // Check if player is dead
            if (player->attribute.currentHP <= 0) {
                break;
            }
            
            // Check incapacitation
            if (IsIncapacitated(player)) {
                AddCombatLog("Player is incapacitated!");
                EndTurn(player);
                playerTurn = 0;
                continue;
            }
            
            // Player action
            int action = GetPlayerAction();
            switch (action) {
                case ACTION_ATTACK:
                    PlayerAttackAction(player, enemy);
                    EndTurn(player);
                    playerTurn = 0;
                    break;
                
                case ACTION_DEFEND:
                    PlayerDefendAction(player);
                    EndTurn(player);
                    playerTurn = 0;
                    break;
                
                case ACTION_VIEW_STATS:
                    CharacterRenderer(player);
                    // Don't end turn, just show info
                    break;
            }
            
            // Check if enemy is dead
            if (enemy->attribute.currentHP <= 0) {
                break;
            }
            
        } else {
            // ═══════════════════════════════════════════════
            // ENEMY TURN
            // ═══════════════════════════════════════════════
            StartTurn(enemy);
            
            // Check if enemy is dead
            if (enemy->attribute.currentHP <= 0) {
                break;
            }
            
            // Check incapacitation
            if (!IsIncapacitated(enemy)) {
                EnemyTurn(player, enemy);
            } else {
                AddCombatLog("Enemy is incapacitated!");
            }
            
            EndTurn(enemy);
            playerTurn = 1;
            
            // Check if player is dead
            if (player->attribute.currentHP <= 0) {
                break;
            }
        }
    }
    
    // Return 1 if player won, 0 if player lost
    return player->attribute.currentHP > 0;
}
```

## 🎨 UI System

### Combat UI Layout

```
╔════════════════════════════════════════════════════════════╗
║ Floor: 15                                        [ELITE]   ║
╚════════════════════════════════════════════════════════════╝

Player Name         [████████████████░░░░░░░░░░] 650/800 [F][P]
Elite Enemy         [██████████████████████████] 1240/1240 [DEFEND]

╔════════════════════════════════════════════════════════════╗
║ Combat Log                                                 ║
╠════════════════════════════════════════════════════════════╣
║ === PLAYER TURN ===                                        ║
║ CRIT! Player dealt 456 damage to Elite Enemy              ║
║ Player healed 36 HP (Lifesteal)                            ║
║ Elite Enemy is afflicted with Burn!                        ║
║ === ENEMY TURN ===                                         ║
║ Elite Enemy takes a defensive stance!                      ║
║                                                            ║
║                                                            ║
╚════════════════════════════════════════════════════════════╝

╔════════════════════════════════════════════════════════════╗
║ [1] Attack      [2] Defend                                 ║
║ [3] View Player Profile    [4] View Enemy Profile          ║
╚════════════════════════════════════════════════════════════╝
```

### HP Bar Rendering

```c
void DrawHPBar(const Character* character, int barWidth) {
    // Calculate filled portion
    int filledBars = (int)(
        (float)character->attribute.currentHP / 
        (float)character->attribute.maxHP * 
        (float)barWidth
    );
    
    // Color based on HP percentage
    float hpPercent = (float)character->attribute.currentHP / 
                      (float)character->attribute.maxHP;
    
    Color hpColor;
    if (hpPercent > 0.6f)      hpColor = COL_GREEN;
    else if (hpPercent > 0.3f) hpColor = COL_YELLOW;
    else                       hpColor = COL_RED;
    
    // Draw bar
    printColor(hpColor, "[");
    for (int i = 0; i < barWidth; i++) {
        if (i < filledBars) {
            printColor(hpColor, "█");
        } else {
            printColor(COL_BRIGHT_BLACK, "░");
        }
    }
    printColor(hpColor, "]");
}
```

### Status Icons

```c
void DrawStatusIcons(const Character* character) {
    if (character->statusCount == 0) return;
    
    printf(" [");
    for (int i = 0; i < character->statusCount && i < 5; i++) {
        switch (character->currentStatus[i].type) {
            case BURN:   printColor(COL_RED, "F");    break;  // Fire
            case POISON: printColor(COL_GREEN, "P");  break;  // Poison
            case STUN:   printColor(COL_YELLOW, "S"); break;  // Stun
            case FREEZE: printColor(COL_CYAN, "I");   break;  // Ice
        }
    }
    printf("]");
}
```

## 🤖 Enemy AI

### Basic AI Strategy

```c
void EnemyTurn(Character* player, Character* enemy) {
    // Simple AI: Defend if HP is low
    float hpPercent = (float)enemy->attribute.currentHP / 
                      (float)enemy->attribute.maxHP;
    
    // 30% chance to defend if below 40% HP
    if (hpPercent < 0.4f && (rand() % 100) < 30) {
        SetDefendingStance(enemy);
        AddCombatLog("%s takes a defensive stance!", enemy->name);
    } else {
        // Attack
        ExecuteCombatTurn(enemy, player);
    }
}
```

### Potential AI Improvements

1. **Aggressive Mode**: Low HP → more attacks
2. **Defensive Mode**: High HP → occasional defends
3. **Smart Defend**: Defend before player's big hit
4. **Status Awareness**: Defend when stunned next turn
5. **Blessing Synergy**: Use strategy based on blessings

## 📊 Balance Considerations

### Post-Combat Healing

```c
void ApplyPostCombatHealing(Character* player) {
    // Base healing: 10% of max HP
    long long baseHeal = player->attribute.maxHP / 10;
    
    // Bonus healing from regen stat
    long long regenHeal = (long long)(
        (float)player->attribute.maxHP * 
        ((float)player->attribute.current.regen / 100.0f)
    );
    
    long long totalHeal = baseHeal + regenHeal;
    
    // Apply healing
    player->attribute.currentHP += totalHeal;
    if (player->attribute.currentHP > player->attribute.maxHP) {
        player->attribute.currentHP = player->attribute.maxHP;
    }
}
```

**Design Decision**: Healing between fights prevents grinding being the only strategy, while regen bonuses reward stat investment.

### Minimum Damage

All attacks deal at least 1 damage, preventing total negation:
```c
if (damageAfterDefense < 1) {
    damageAfterDefense = 1;
}
```

## 🐛 Common Issues

### Issue: Status Effects Not Ticking

**Problem**: Status effects added but don't deal damage.

**Solution**: Ensure `ProcessStatusEffects()` is called in `StartTurn()`:
```c
void StartTurn(Character* character) {
    ClearCombatState(character);
    RecalculateStats(character);
    ProcessStatusEffects(character);  // MUST be here
    ProcessRegeneration(character);
}
```

### Issue: Defend Carries Over to Next Turn

**Problem**: Defending once makes character defend forever.

**Solution**: Call `ClearCombatState()` in both `StartTurn()` and `EndTurn()`:
```c
void EndTurn(Character* character) {
    ClearCombatState(character);  // Clear temporary effects
}
```

### Issue: Lifesteal Doesn't Work

**Problem**: Lifesteal blessing added but no healing occurs.

**Solution**: Check that lifesteal is calculated in `CalculateDamage()` and applied in `ApplyDamageResult()`.

## 🚀 Advanced Features

### Combo System (Future)

```c
typedef struct {
    int consecutiveHits;
    float damageMultiplier;
} ComboState;

// Increase multiplier with consecutive hits
// Reset on miss or getting hit
```

### Skill System (Future)

```c
typedef struct {
    char name[64];
    int cooldown;
    int currentCooldown;
    void (*effect)(Character* user, Character* target);
} Skill;
```

### Difficulty Modes

```c
typedef enum {
    DIFFICULTY_EASY,    // 0.75x enemy stats
    DIFFICULTY_NORMAL,  // 1.0x enemy stats
    DIFFICULTY_HARD,    // 1.5x enemy stats
    DIFFICULTY_INSANE   // 2.0x enemy stats
} DifficultyMode;
```

## 📈 Statistics Tracking

### Combat Statistics

```c
typedef struct {
    long long totalDamageDealt;
    long long totalDamageTaken;
    int totalHits;
    int totalMisses;
    int totalCrits;
    int statusesApplied;
    long long maxHit;
} CombatStats;
```

### Run Statistics

```c
typedef struct {
    int floorsCleared;
    int enemiesDefeated;
    int blessingsCollected;
    long long totalDamageDealt;
    time_t runDuration;
} RunStats;
```

## 🎯 Best Practices

1. **Always log important events** to combat log
2. **Clear temporary state** at turn boundaries
3. **Clamp HP** between 0 and maxHP after any change
4. **Check for death** after applying damage
5. **Process status effects** at start of turn, not end
6. **Recalculate stats** after any blessing change
7. **Minimum 1 damage** to prevent unkillable enemies
8. **Cap percentage stats** to prevent overflow

## 🔮 Future Enhancements

1. **Boss Mechanics**: Unique boss abilities and phases
2. **Environmental Effects**: Floor modifiers (fog, fire, etc.)
3. **Item Drops**: Equipment system separate from blessings
4. **Achievements**: Track player accomplishments
5. **Daily Challenges**: Special run modifiers
6. **Endless Mode**: See how far you can go
7. **PvP System**: Fight other players' builds
8. **Replays**: Save and watch combat replays

---

**Last Updated**: December 2025