#ifndef UTILS_H
#define UTILS_H

#include <common.h>

#define USE_N_VERTED true

enum QUADBLOCKFLAGS {

	Q_NONE = 0,
	Q_INVISIBLE = (1 << 0),
	Q_GRAVITY = (1 << 1),
	Q_REFLECTION = (1 << 2),
	Q_KICKERS = (1 << 3),
	Q_OOF_BOUNDS = (1 << 4),
	Q_NV_USED = (1 << 5),
	Q_TRIGGER_SCRIPT = (1 << 6),
	Q_REVERB = (1 << 7),
	Q_KICKERS2 = (1 << 8),
	Q_MASK_GRAB = (1 << 9),
	Q_TEMPLE_DOOR = (1 << 10),
	Q_COLLISION_TRIGGER = (1 << 11),
	Q_GROUND = (1 << 12),
	Q_WALL = (1 << 13),
	Q_NO_COLL = (1 << 14),
	Q_INV_TRIGGERS = (1 << 15),
	Q_ALL = -1

};

#endif