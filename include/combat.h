// combat.h

#ifndef __COMBAT__
#define __COMBAT__

#define TYPE_REGULAR	0
#define TYPE_RIPOSTE	1
#define TYPE_QUICK	2
#define TYPE_SUPER	3
#define TYPE_BUSY	4
#define TYPE_RUSH	5
#define TYPE_KILL	6

#define RESULT_DODGE	-1
#define RESULT_PARRY	-2

// This is used as skill_power()'s argument to indicate which type of skill
// usage will be used in calculating skill power.
#define SKILL_USAGE_ATTACK		1
#define SKILL_USAGE_DEFENSE		2

#endif
