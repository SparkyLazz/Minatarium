#include <string.h>
#include "Blessing.h"

Blessing Himalaya() {
    Blessing Himalaya;
    strcpy(Himalaya.Name, "Himalaya");
    strcpy(Himalaya.Description, "Increasing Player Damage by 5% | Max Stat : 25");
    Himalaya.Type = BLESS_DAMAGE_BOOST;
    Himalaya.maxStack = 25;
    Himalaya.value = 5;
    Himalaya.currentStack = 1;
    return Himalaya;
}