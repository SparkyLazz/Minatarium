#ifndef COMBAT_H
#define COMBAT_H

#include "../Entity/Characters.h"

// Run a simple endless-floor demo loop to showcase systems
void roguelike_demo();

// Start endless-floor mode using an already-initialized player (name, etc.).
// If 'initial' is NULL, a default player will be used.
void roguelike_start(const Characters* initial);

#endif
