#pragma once

#include <random>
#include "CoordinatePair.h"

enum class SHIPTYPE
{
	DOUBLE_DECK,
	SINGLE_DECK
};

struct Ship
{
	int32_t _cooldown;
	bool _isDead;
	bool _isWounded;
	SHIPTYPE _type;
	CoordinatePair _cp1;
	CoordinatePair _cp2;//nullable

	static std::mt19937 mt;

	Ship(SHIPTYPE type, CoordinatePair cp1, CoordinatePair cp2 = CoordinatePair(-1, -1), bool isDead = false);

	void wasShot(SHIPTYPE type);

	bool revive();
};

bool operator==(const Ship& ship1, const Ship& ship2);