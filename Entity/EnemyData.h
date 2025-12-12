#ifndef ENEMY_DATA_H
#define ENEMY_DATA_H

#include "Characters.h"

// Initialize enemy blueprints and boss skills.
void enemy_registry_init();

// Get a random blueprint for the requested tier.
// Returns non-NULL.
const Characters* enemy_pick_blueprint(CharacterType t);

#endif
