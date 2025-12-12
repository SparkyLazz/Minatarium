#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Combat.h"
#include "../Blessing/Blessing.h"
#include "../Entity/EnemyData.h"

// Blessings are provided by the registry (can grow to 100+ without code changes)

static int rand_range(int minInclusive, int maxInclusive) {
    if (maxInclusive <= minInclusive) return minInclusive;
    return minInclusive + (rand() % (maxInclusive - minInclusive + 1));
}

static const char* rarity_str(BlessingRarity r) {
    switch (r) {
        case RARITY_COMMON: return "Common";
        case RARITY_RARE: return "Rare";
        case RARITY_EPIC: return "Epic";
        case RARITY_LEGENDARY: return "Legendary";
        default: return "Unknown";
    }
}

// Forward decl for scaling helper used by generator
static void scale_enemy_blessings_for_floor(Characters* e, int floor);

static void add_status(Characters* target, StatusType type, float amount, int duration) {
    if (target->statusCount >= MAX_STATUS) return;
    // If same type exists, refresh/increase
    for (int i = 0; i < target->statusCount; ++i) {
        if (target->currentStatus[i].type == type) {
            target->currentStatus[i].amount += amount;
            if (duration > target->currentStatus[i].remaining) {
                target->currentStatus[i].remaining = duration;
            }
            return;
        }
    }
    target->currentStatus[target->statusCount].type = type;
    target->currentStatus[target->statusCount].amount = amount;
    target->currentStatus[target->statusCount].remaining = duration;
    target->statusCount++;
}

static int has_status(const Characters* c, StatusType t) {
    for (int i = 0; i < c->statusCount; ++i) if (c->currentStatus[i].type == t) return 1;
    return 0;
}

static void tick_statuses(Characters* c) {
    // Apply DoT for burn/poison and decrease remaining
    int write = 0;
    for (int i = 0; i < c->statusCount; ++i) {
        ActiveStatus s = c->currentStatus[i];
        if (s.type == STATUS_BURN || s.type == STATUS_POISON) {
            long long dmg = (long long)s.amount;
            if (dmg < 1) dmg = 1;
            if (c->attributes.hp > dmg) c->attributes.hp -= dmg; else c->attributes.hp = 0;
        }
        // decrement duration
        s.remaining -= 1;
        if (s.remaining > 0) {
            c->currentStatus[write++] = s;
        }
    }
    c->statusCount = write;
}

static void apply_on_hit_statuses(const Characters* attacker, Characters* defender) {
    // Chance vs. resist
    CombatAttributes a; computeEffectiveAttributes(attacker, &a);
    CombatAttributes d; computeEffectiveAttributes(defender, &d);
    for (int i = 0; i < attacker->blessingCount; ++i) {
        const ActiveBlessing* ab = &attacker->currentBlessing[i];
        const Blessing* b = ab->def;
        if (!b || ab->stacks <= 0) continue;
        for (int j = 0; j < b->statusCount; ++j) {
            const BlessingStatus* bs = &b->statuses[j];
            int chance = bs->chance + a.status.effectHitRate - d.status.resistanceEffect;
            if (chance < 0) chance = 0; if (chance > 100) chance = 100;
            int roll = rand_range(1, 100);
            if (roll <= chance) {
                float amount = bs->magnitude * (float)ab->stacks;
                int duration = 3;
                add_status(defender, bs->type, amount, duration);
            }
        }
    }
}

static long long calc_damage_after_modifiers(const Characters* attacker, const Characters* defender) {
    CombatAttributes a; computeEffectiveAttributes(attacker, &a);
    CombatAttributes d; computeEffectiveAttributes(defender, &d);

    long long raw = a.attack;
    // Apply damage boost as percentage
    raw += (raw * a.off.damageBoost) / 100;

    // Simple crit
    int critRoll = rand_range(1, 100);
    if (critRoll <= a.off.critChance) {
        raw += (raw * a.off.critDamage) / 100; // critDamage as extra percent
    }

    long long mitigated = raw - d.defense;
    if (mitigated < 1) mitigated = 1;
    // Damage reduction percent on defender
    mitigated -= (mitigated * d.def.damageReduction) / 100;
    if (mitigated < 1) mitigated = 1;
    return mitigated;
}

static void deal_damage(Characters* defender, long long dmg) {
    if (dmg < 1) dmg = 1;
    if (defender->attributes.hp > dmg) defender->attributes.hp -= dmg; else defender->attributes.hp = 0;
}

static void clone_base_from_blueprint(Characters* dst, const Characters* src, const char* name, CharacterType t) {
    memset(dst, 0, sizeof(*dst));
    *dst = *src; // copy base including nested attributes values
    strncpy(dst->name, name, sizeof(dst->name)-1);
    dst->type = t;
    dst->blessingCount = 0;
    dst->statusCount = 0;
    dst->skillCount = 0;
}

static void generate_enemy_for_floor(int floor, Characters* outEnemy) {
    // Decide type by floor
    CharacterType t;
    if (floor % 10 == 0) t = CHARACTER_BOSS;
    else if (floor % 5 == 0) t = CHARACTER_ELITE;
    else t = CHARACTER_NORMAL;

    // Pick a blueprint from registry
    const Characters* bp = enemy_pick_blueprint(t);
    clone_base_from_blueprint(outEnemy, bp, bp->name, t);

    long long scaleHp, scaleAtk, scaleDef;
    if (t == CHARACTER_NORMAL) {
        scaleHp = 50 + floor * 10;
        scaleAtk = 5 + floor * 2;
        scaleDef = 2 + floor * 1;
    } else if (t == CHARACTER_ELITE) {
        scaleHp = 120 + floor * 18;
        scaleAtk = 12 + floor * 4;
        scaleDef = 6 + floor * 2;
    } else { // BOSS
        scaleHp = 250 + floor * 30;
        scaleAtk = 20 + floor * 6;
        scaleDef = 10 + floor * 3;
    }

    outEnemy->attributes.maxHp = scaleHp;
    outEnemy->attributes.hp = scaleHp;
    outEnemy->attributes.attack = scaleAtk;
    outEnemy->attributes.defense = scaleDef;

    // Scale blessings count and stacks based on floor and tier
    scale_enemy_blessings_for_floor(outEnemy, floor);
}

static int collect_pool_by_rarity(BlessingRarity r, const Blessing* out[], int maxOut) {
    return blessing_collect_by_rarity(r, out, maxOut);
}

static void pick_three_blessings_for_type(CharacterType enemyType, const Blessing* choices[3]) {
    const Blessing* pool[200];
    int poolCount = 0;

    if (enemyType == CHARACTER_NORMAL) {
        // 80% Common, 20% Rare; no Epic/Legendary
        int commonCount = collect_pool_by_rarity(RARITY_COMMON, pool, 200);
        const Blessing* rarePool[200]; int rareCount = collect_pool_by_rarity(RARITY_RARE, rarePool, 200);
        for (int k = 0; k < 3; ++k) {
            int roll = rand_range(1, 100);
            const Blessing* pick = NULL;
            if (roll <= 80 && commonCount > 0) pick = pool[rand_range(0, commonCount - 1)];
            else if (rareCount > 0) pick = rarePool[rand_range(0, rareCount - 1)];
            choices[k] = pick;
        }
        return;
    } else if (enemyType == CHARACTER_ELITE) {
        // 70% Rare, 30% Epic; no Common/Legendary
        int rareCount = collect_pool_by_rarity(RARITY_RARE, pool, 200);
        const Blessing* epicPool[200]; int epicCount = collect_pool_by_rarity(RARITY_EPIC, epicPool, 200);
        for (int k = 0; k < 3; ++k) {
            int roll = rand_range(1, 100);
            const Blessing* pick = NULL;
            if (roll <= 70 && rareCount > 0) pick = pool[rand_range(0, rareCount - 1)];
            else if (epicCount > 0) pick = epicPool[rand_range(0, epicCount - 1)];
            choices[k] = pick;
        }
        return;
    } else { // BOSS
        // 85% Epic, 15% Legendary
        int epicCount = collect_pool_by_rarity(RARITY_EPIC, pool, 200);
        const Blessing* legPool[200]; int legCount = collect_pool_by_rarity(RARITY_LEGENDARY, legPool, 200);
        for (int k = 0; k < 3; ++k) {
            int roll = rand_range(1, 100);
            const Blessing* pick = NULL;
            if (roll <= 85 && epicCount > 0) pick = pool[rand_range(0, epicCount - 1)];
            else if (legCount > 0) pick = legPool[rand_range(0, legCount - 1)];
            else if (epicCount > 0) pick = pool[rand_range(0, epicCount - 1)];
            choices[k] = pick;
        }
        return;
    }
}

static void show_blessing_choices(const Blessing* choices[3]) {
    printf("Choose a blessing reward:\n");
    for (int i = 0; i < 3; ++i) {
        if (choices[i]) {
            printf("[%d] %s (%s) - %s\n", i + 1, choices[i]->name, rarity_str(choices[i]->rarity), choices[i]->description);
        }
    }
}

static void grant_blessing_reward(Characters* player, CharacterType enemyType) {
    const Blessing* choices[3];
    pick_three_blessings_for_type(enemyType, choices);
    show_blessing_choices(choices);
    printf("Your pick: ");
    int pick = 0; scanf("%d", &pick);
    if (pick < 1 || pick > 3 || choices[pick - 1] == NULL) {
        printf("Invalid choice. Skipping reward.\n");
        return;
    }
    long long stacks = (rand_range(0, 1) == 0) ? 5 : 7; // 5 or 7 stacks
    addBlessingStacks(player, choices[pick - 1], stacks);
    printf("Gained %lld stacks of %s!\n", stacks, choices[pick - 1]->name);
}

static void tick_skill_cooldowns(Characters* c) {
    for (int i = 0; i < c->skillCount; ++i) {
        BlessingSkill* s = c->currentSkill[i];
        if (!s) continue;
        if (s->currentCooldown > 0) s->currentCooldown -= 1;
        if (s->currentCooldown < 0) s->currentCooldown = 0;
    }
}

static void use_skill(BlessingSkill* s, Characters* self, Characters* target) {
    if (!s || !self || !target) return;
    if (s->currentCooldown > 0) return;
    if (s->effect) s->effect(self, target);
    s->currentCooldown = s->cooldown;
}

// =============================
// Enemy blessing scaling helpers
// =============================

static int has_blessing_id(const Characters* c, int id) {
    for (int i = 0; i < c->blessingCount; ++i) {
        if (c->currentBlessing[i].def && c->currentBlessing[i].def->id == id) return 1;
    }
    return 0;
}

static void add_random_blessing_for_tier(Characters* e, CharacterType t, long long stacks) {
    const Blessing* pick = NULL;
    if (t == CHARACTER_NORMAL) {
        const Blessing* commons[256]; int cN = collect_pool_by_rarity(RARITY_COMMON, commons, 256);
        const Blessing* rares[256];   int rN = collect_pool_by_rarity(RARITY_RARE, rares, 256);
        int roll = rand_range(1,100);
        if (roll <= 80 && cN > 0) pick = commons[rand_range(0, cN-1)];
        else if (rN > 0) pick = rares[rand_range(0, rN-1)];
    } else if (t == CHARACTER_ELITE) {
        const Blessing* rares[256]; int rN = collect_pool_by_rarity(RARITY_RARE, rares, 256);
        const Blessing* epics[256]; int eN = collect_pool_by_rarity(RARITY_EPIC, epics, 256);
        int roll = rand_range(1,100);
        if (roll <= 70 && rN > 0) pick = rares[rand_range(0, rN-1)];
        else if (eN > 0) pick = epics[rand_range(0, eN-1)];
    } else { // BOSS
        const Blessing* epics[256]; int eN = collect_pool_by_rarity(RARITY_EPIC, epics, 256);
        const Blessing* legs[256];  int lN = collect_pool_by_rarity(RARITY_LEGENDARY, legs, 256);
        int roll = rand_range(1,100);
        if (roll <= 85 && eN > 0) pick = epics[rand_range(0, eN-1)];
        else if (lN > 0) pick = legs[rand_range(0, lN-1)];
        else if (eN > 0) pick = epics[rand_range(0, eN-1)];
    }
    if (pick) {
        // avoid duplicates growing the list; use addBlessingStacks to merge if exists
        addBlessingStacks(e, pick, stacks > 0 ? stacks : 1);
    }
}

static void scale_enemy_blessings_for_floor(Characters* e, int floor) {
    // Increase stacks on existing blessings
    long long addStacks;
    if (e->type == CHARACTER_NORMAL) addStacks = (floor / 12);
    else if (e->type == CHARACTER_ELITE) addStacks = 1 + (floor / 10);
    else addStacks = 2 + (floor / 8); // boss
    long long variance = (rand_range(0, 100) < 30) ? 1 : 0; // 30% +1
    addStacks += variance;
    if (addStacks < 0) addStacks = 0;
    for (int i = 0; i < e->blessingCount; ++i) {
        if (e->currentBlessing[i].def) e->currentBlessing[i].stacks += addStacks;
    }

    // Ensure minimum blessing count by floor and tier
    int minCount;
    if (e->type == CHARACTER_NORMAL) minCount = 1 + (floor / 8);          // 1, then +1 every 8 floors
    else if (e->type == CHARACTER_ELITE) minCount = 2 + (floor / 6);       // 2, +1 every 6 floors
    else minCount = 3 + (floor / 5);                                       // 3, +1 every 5 floors

    if (minCount > MAX_BLESSING) minCount = MAX_BLESSING;
    while (e->blessingCount < minCount) {
        long long stacks;
        if (e->type == CHARACTER_NORMAL) stacks = 1 + (floor / 12);
        else if (e->type == CHARACTER_ELITE) stacks = 2 + (floor / 10);
        else stacks = 3 + (floor / 8);
        if (stacks < 1) stacks = 1;
        add_random_blessing_for_tier(e, e->type, stacks);
        if (e->blessingCount >= MAX_BLESSING) break;
    }
}

void roguelike_demo() {
    srand((unsigned int)time(NULL));
    blessing_init_registry();
    enemy_registry_init();
    // Initialize player
    Characters player = playerBluePrint;
    if (strlen(player.name) == 0) strcpy(player.name, "Hero");

    int floor = 1;
    int running = 1;
    while (running) {
        Characters enemy; generate_enemy_for_floor(floor, &enemy);
        printf("\n====================================================\n");
        printf("                 Floor %d\n", floor);
        printf("Encounter: %s (%s)\n", enemy.name,
               enemy.type == CHARACTER_BOSS ? "Boss" : (enemy.type == CHARACTER_ELITE ? "Elite" : "Normal"));
        printf("====================================================\n");

        // Simple combat loop
        int battle = 1;
        while (battle) {
            // Tick statuses
            tick_statuses(&player);
            tick_statuses(&enemy);

            if (player.attributes.hp <= 0 || enemy.attributes.hp <= 0) break;

            printf("\n[Player] %s  HP: %lld/%lld\n[Enemy ] %s  HP: %lld/%lld\n",
                   player.name,
                   player.attributes.hp, player.attributes.maxHp,
                   enemy.name,
                   enemy.attributes.hp, enemy.attributes.maxHp);
            int playerHasSkills = player.skillCount > 0 && !has_status(&player, STATUS_SILENCE);
            printf("1) Attack  2) Defend  %s3) Skills  %s4) View Player  5) View Enemy  6) Run\n",
                   playerHasSkills ? "" : "",
                   "");
            printf("Action: ");
            int action; scanf("%d", &action);

            int playerDefend = 0;
            if (action == 4) {
                showPlayerStats(&player);
                continue;
            } else if (action == 5) {
                showPlayerStats(&enemy);
                continue;
            } else if (action == 6) {
                printf("You fled the battle...\n");
                running = 0; // exit demo
                break;
            }

            if (action == 2) {
                playerDefend = 1; // grant temporary reduction this turn
            }

            // Player turn (attack if selected)
            if (has_status(&player, STATUS_STUN)) {
                printf("You are stunned and cannot act!\n");
            } else if (action == 1) {
                long long dmg = calc_damage_after_modifiers(&player, &enemy);
                deal_damage(&enemy, dmg);
                printf("You hit the enemy for %lld damage!\n", dmg);
                apply_on_hit_statuses(&player, &enemy);
            } else if (action == 3) {
                if (!playerHasSkills) {
                    printf("No usable skills.\n");
                } else {
                    printf("Choose a skill:\n");
                    int list[100]; int listCount = 0;
                    for (int i = 0; i < player.skillCount; ++i) {
                        BlessingSkill* s = player.currentSkill[i];
                        if (!s) continue;
                        printf("[%d] %s %s\n", i + 1, s->name, (s->currentCooldown > 0 ? "(CD)" : ""));
                        list[listCount++] = i;
                    }
                    int choice = 0; printf("Skill #: "); scanf("%d", &choice);
                    if (choice >= 1 && choice <= player.skillCount) {
                        BlessingSkill* s = player.currentSkill[choice - 1];
                        if (s && s->currentCooldown == 0) {
                            use_skill(s, &player, &enemy);
                            printf("You used %s!\n", s->name);
                        } else {
                            printf("Skill on cooldown.\n");
                        }
                    }
                }
            } else {
                printf("You take a defensive stance.\n");
            }

            if (enemy.attributes.hp <= 0) break;

            // Enemy turn (simple AI: always attack)
            if (has_status(&enemy, STATUS_STUN)) {
                printf("Enemy is stunned and misses the turn!\n");
            } else {
                int enemyUsedSkill = 0;
                if (!has_status(&enemy, STATUS_SILENCE)) {
                    for (int i = 0; i < enemy.skillCount; ++i) {
                        BlessingSkill* s = enemy.currentSkill[i];
                        if (s && s->currentCooldown == 0 && rand_range(1, 100) <= 30) {
                            use_skill(s, &enemy, &player);
                            printf("Enemy used %s!\n", s->name);
                            enemyUsedSkill = 1;
                            break;
                        }
                    }
                }
                if (!enemyUsedSkill) {
                    long long dmgToPlayer = calc_damage_after_modifiers(&enemy, &player);
                    if (playerDefend) {
                        dmgToPlayer -= (dmgToPlayer * 20) / 100; // 20% less when defending
                    }
                    deal_damage(&player, dmgToPlayer);
                    printf("Enemy hits you for %lld damage!\n", dmgToPlayer);
                    apply_on_hit_statuses(&enemy, &player);
                }
            }

            // Tick cooldowns at end of round
            tick_skill_cooldowns(&player);
            tick_skill_cooldowns(&enemy);
        }

        if (player.attributes.hp <= 0) {
            printf("\nYou were defeated on floor %d. Game over.\n", floor);
            break;
        } else if (enemy.attributes.hp <= 0) {
            printf("\nYou defeated the enemy on floor %d!\n", floor);
            grant_blessing_reward(&player, enemy.type);
            // Heal a bit between floors
            long long heal = player.attributes.maxHp / 5; // 20% heal
            player.attributes.hp += heal;
            if (player.attributes.hp > player.attributes.maxHp) player.attributes.hp = player.attributes.maxHp;
            floor++;
        }

        if (player.attributes.hp > 0) {
            printf("\nContinue to next floor? (1=Yes / 0=No): ");
            int cont; scanf("%d", &cont);
            if (!cont) running = 0;
        }
    }
}

void roguelike_start(const Characters* initial) {
    srand((unsigned int)time(NULL));
    blessing_init_registry();
    enemy_registry_init();
    Characters player = initial ? *initial : playerBluePrint;
    if (strlen(player.name) == 0) strcpy(player.name, "Hero");

    int floor = 1;
    int running = 1;
    while (running) {
        Characters enemy; generate_enemy_for_floor(floor, &enemy);
        printf("\n====================================================\n");
        printf("                 Floor %d\n", floor);
        printf("Encounter: %s (%s)\n", enemy.name,
               enemy.type == CHARACTER_BOSS ? "Boss" : (enemy.type == CHARACTER_ELITE ? "Elite" : "Normal"));
        printf("====================================================\n");

        int battle = 1;
        while (battle) {
            tick_statuses(&player);
            tick_statuses(&enemy);
            if (player.attributes.hp <= 0 || enemy.attributes.hp <= 0) break;

            printf("\n[Player] %s  HP: %lld/%lld\n[Enemy ] %s  HP: %lld/%lld\n",
                   player.name,
                   player.attributes.hp, player.attributes.maxHp,
                   enemy.name,
                   enemy.attributes.hp, enemy.attributes.maxHp);
            int playerHasSkills = player.skillCount > 0 && !has_status(&player, STATUS_SILENCE);
            printf("1) Attack  2) Defend  %s3) Skills  %s4) View Player  5) View Enemy  6) Run\n",
                   playerHasSkills ? "" : "",
                   "");
            printf("Action: ");
            int action; scanf("%d", &action);

            int playerDefend = 0;
            if (action == 4) { showPlayerStats(&player); continue; }
            else if (action == 5) { showPlayerStats(&enemy); continue; }
            else if (action == 6) { printf("You fled the battle...\n"); running = 0; break; }
            if (action == 2) playerDefend = 1;

            if (has_status(&player, STATUS_STUN)) {
                printf("You are stunned and cannot act!\n");
            } else if (action == 1) {
                long long dmg = calc_damage_after_modifiers(&player, &enemy);
                deal_damage(&enemy, dmg);
                printf("You hit the enemy for %lld damage!\n", dmg);
                apply_on_hit_statuses(&player, &enemy);
            } else if (action == 3) {
                if (!playerHasSkills) {
                    printf("No usable skills.\n");
                } else {
                    printf("Choose a skill:\n");
                    for (int i = 0; i < player.skillCount; ++i) {
                        BlessingSkill* s = player.currentSkill[i];
                        if (!s) continue;
                        printf("[%d] %s %s\n", i + 1, s->name, (s->currentCooldown > 0 ? "(CD)" : ""));
                    }
                    int choice = 0; printf("Skill #: "); scanf("%d", &choice);
                    if (choice >= 1 && choice <= player.skillCount) {
                        BlessingSkill* s = player.currentSkill[choice - 1];
                        if (s && s->currentCooldown == 0) { use_skill(s, &player, &enemy); printf("You used %s!\n", s->name); }
                        else { printf("Skill on cooldown.\n"); }
                    }
                }
            } else {
                printf("You take a defensive stance.\n");
            }

            if (enemy.attributes.hp <= 0) break;

            if (has_status(&enemy, STATUS_STUN)) {
                printf("Enemy is stunned and misses the turn!\n");
            } else {
                int enemyUsedSkill = 0;
                if (!has_status(&enemy, STATUS_SILENCE)) {
                    for (int i = 0; i < enemy.skillCount; ++i) {
                        BlessingSkill* s = enemy.currentSkill[i];
                        if (s && s->currentCooldown == 0 && rand_range(1, 100) <= 30) { use_skill(s, &enemy, &player); printf("Enemy used %s!\n", s->name); enemyUsedSkill = 1; break; }
                    }
                }
                if (!enemyUsedSkill) {
                    long long dmgToPlayer = calc_damage_after_modifiers(&enemy, &player);
                    if (playerDefend) dmgToPlayer -= (dmgToPlayer * 20) / 100;
                    deal_damage(&player, dmgToPlayer);
                    printf("Enemy hits you for %lld damage!\n", dmgToPlayer);
                    apply_on_hit_statuses(&enemy, &player);
                }
            }
            tick_skill_cooldowns(&player);
            tick_skill_cooldowns(&enemy);
        }

        if (player.attributes.hp <= 0) { printf("\nYou were defeated on floor %d. Game over.\n", floor); break; }
        else if (enemy.attributes.hp <= 0) {
            printf("\nYou defeated the enemy on floor %d!\n", floor);
            grant_blessing_reward(&player, enemy.type);
            long long heal = player.attributes.maxHp / 5; player.attributes.hp += heal; if (player.attributes.hp > player.attributes.maxHp) player.attributes.hp = player.attributes.maxHp;
            floor++;
        }

        if (player.attributes.hp > 0) {
            printf("\nContinue to next floor? (1=Yes / 0=No): "); int cont; scanf("%d", &cont); if (!cont) running = 0;
        }
    }
}
