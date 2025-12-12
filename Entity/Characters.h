#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "../Status/Status.h"
#include "../Blessing/Blessing.h"

#define MAX_BLESSING 100
#define MAX_STATUS   100
#define MAX_SKILLS   100

typedef enum {
    CHARACTER_PLAYER = 0,
    CHARACTER_NORMAL,
    CHARACTER_ELITE,
    CHARACTER_BOSS
} CharacterType;

// Per-instance blessing and status state
typedef struct {
    const Blessing* def;   // static blessing definition
    long long stacks;      // runtime stacks on this character
} ActiveBlessing;

typedef struct {
    StatusType type;
    float amount;      // effective amount (e.g., DoT per tick)
    int remaining;     // remaining turns/seconds
} ActiveStatus;

typedef struct Characters {
    char name[100];
    CharacterType type;
    CombatAttributes attributes;

    ActiveBlessing currentBlessing[MAX_BLESSING];
    int blessingCount;

    ActiveStatus currentStatus[MAX_STATUS];
    int statusCount;

    BlessingSkill* currentSkill[MAX_SKILLS];
    int skillCount;

} Characters;

extern const Characters playerBluePrint;

void renderBaseStats(void* data);
void renderBlessingData(void* data);
void renderStatusData(void* data);
void showPlayerStats(Characters* player);

// ======================================
// Characters API (scalable helpers)
// ======================================

// Compute effective combat attributes from base attributes + all active blessings.
// Result is written to 'out'.
void computeEffectiveAttributes(const Characters* c, CombatAttributes* out);

// Blessing management (simple pointer-based; per-instance stacks can be added later)
int addBlessingStacks(Characters* c, const Blessing* b, long long stacks); // add or increase stacks
int addBlessing(Characters* c, const Blessing* b);                              // +1 stack convenience
int removeBlessingById(Characters* c, int blessingId);                          // returns 1 if removed, 0 if not found
void clearBlessings(Characters* c);

#endif
