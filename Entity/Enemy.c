
// Enemies.c
#include <string.h>
#include "Characters.h"
#include "../Blessing/Blessing.h"

// Blessings defined in BlessingData.c
extern Blessing warrior_strength;     // id = 1
extern Blessing guardian_shield;      // id = 2
extern Blessing swift_footwork;       // id = 3
extern Blessing minor_regen;          // id = 4
extern Blessing lucky_strike;         // id = 5
extern Blessing flame_touch;          // id = 6
extern Blessing frost_bite;           // id = 7
extern Blessing venom_coating;        // id = 8
extern Blessing iron_will;            // id = 9
extern Blessing beginner_fortune;     // id = 10

// Storage for your 10 enemies
Characters enemies[10];

void init_enemies(void)
{
    // 1. Goblin Skirmisher (Normal)
    strcpy(enemies[0].name, "Goblin Skirmisher");
    enemies[0].type = CHARACTER_NORMAL;
    enemies[0].attributes = (CombatAttributes){0};
    enemies[0].currentBlessing[0] = warrior_strength;
    enemies[0].currentBlessing[1] = swift_footwork;
    enemies[0].currentBlessing[2] = lucky_strike;
    enemies[0].blessingCount = 3;
    enemies[0].statusCount = 0;
    enemies[0].skillCount = 0;

    // 2. Cave Wolf (Normal)
    strcpy(enemies[1].name, "Cave Wolf");
    enemies[1].type = CHARACTER_NORMAL;
    enemies[1].attributes = (CombatAttributes){0};
    enemies[1].currentBlessing[0] = swift_footwork;
    enemies[1].currentBlessing[1] = minor_regen;
    enemies[1].currentBlessing[2] = iron_will;
    enemies[1].blessingCount = 3;
    enemies[1].statusCount = 0;
    enemies[1].skillCount = 0;

    // 3. Fire Imp (Normal)
    strcpy(enemies[2].name, "Fire Imp");
    enemies[2].type = CHARACTER_NORMAL;
    enemies[2].attributes = (CombatAttributes){0};
    enemies[2].currentBlessing[0] = flame_touch;
    enemies[2].currentBlessing[1] = lucky_strike;
    enemies[2].currentBlessing[2] = guardian_shield;
    enemies[2].blessingCount = 3;
    enemies[2].statusCount = 0;
    enemies[2].skillCount = 0;

    // 4. Frozen Wraith (Elite)
    strcpy(enemies[3].name, "Frozen Wraith");
    enemies[3].type = CHARACTER_ELITE;
    enemies[3].attributes = (CombatAttributes){0};
    enemies[3].currentBlessing[0] = frost_bite;
    enemies[3].currentBlessing[1] = iron_will;
    enemies[3].currentBlessing[2] = guardian_shield;
    enemies[3].blessingCount = 3;
    enemies[3].statusCount = 0;
    enemies[3].skillCount = 0;

    // 5. Venom Serpent (Elite)
    strcpy(enemies[4].name, "Venom Serpent");
    enemies[4].type = CHARACTER_ELITE;
    enemies[4].attributes = (CombatAttributes){0};
    enemies[4].currentBlessing[0] = venom_coating;
    enemies[4].currentBlessing[1] = swift_footwork;
    enemies[4].currentBlessing[2] = minor_regen;
    enemies[4].blessingCount = 3;
    enemies[4].statusCount = 0;
    enemies[4].skillCount = 0;

    // 6. Shielded Bruiser (Elite)
    strcpy(enemies[5].name, "Shielded Bruiser");
    enemies[5].type = CHARACTER_ELITE;
    enemies[5].attributes = (CombatAttributes){0};
    enemies[5].currentBlessing[0] = guardian_shield;
    enemies[5].currentBlessing[1] = warrior_strength;
    enemies[5].currentBlessing[2] = iron_will;
    enemies[5].blessingCount = 3;
    enemies[5].statusCount = 0;
    enemies[5].skillCount = 0;

    // 7. Bandit Captain (Elite)
    strcpy(enemies[6].name, "Bandit Captain");
    enemies[6].type = CHARACTER_ELITE;
    enemies[6].attributes = (CombatAttributes){0};
    enemies[6].currentBlessing[0] = warrior_strength;
    enemies[6].currentBlessing[1] = lucky_strike;
    enemies[6].currentBlessing[2] = beginner_fortune;
    enemies[6].blessingCount = 3;
    enemies[6].statusCount = 0;
    enemies[6].skillCount = 0;

    // 8. Infernal Knight (Boss)
    strcpy(enemies[7].name, "Infernal Knight");
    enemies[7].type = CHARACTER_BOSS;
    enemies[7].attributes = (CombatAttributes){0};
    enemies[7].currentBlessing[0] = flame_touch;
    enemies[7].currentBlessing[1] = guardian_shield;
    enemies[7].currentBlessing[2] = warrior_strength;
    enemies[7].blessingCount = 3;
    enemies[7].statusCount = 0;
    enemies[7].skillCount = 0;

    // 9. Frost Troll (Boss)
    strcpy(enemies[8].name, "Frost Troll");
    enemies[8].type = CHARACTER_BOSS;
    enemies[8].attributes = (CombatAttributes){0};
    enemies[8].currentBlessing[0] = frost_bite;
    enemies[8].currentBlessing[1] = iron_will;
    enemies[8].currentBlessing[2] = minor_regen;
    enemies[8].blessingCount = 3;
    enemies[8].statusCount = 0;
    enemies[8].skillCount = 0;

    // 10. Plague Reaper (Boss)
    strcpy(enemies[9].name, "Plague Reaper");
    enemies[9].type = CHARACTER_BOSS;
    enemies[9].attributes = (CombatAttributes){0};
    enemies[9].currentBlessing[0] = venom_coating;
    enemies[9].currentBlessing[1] = iron_will;
    enemies[9].currentBlessing[2] = beginner_fortune;
    enemies[9].blessingCount = 3;
    enemies[9].statusCount = 0;
    enemies[9].skillCount = 0;
}
