#include "Ship.h"

Ship::Ship(SHIPTYPE type, CoordinatePair cp1, CoordinatePair cp2, bool isDead)
	: _type(type), _isDead(isDead), _cp1(cp1), _cp2(cp2), _cooldown(0), _isWounded(false)
{}

void Ship::wasShot(SHIPTYPE type)
{
	if (type == SHIPTYPE::SINGLE_DECK || _isWounded == true)
		_isDead = true;
	else
		_isWounded = true;
}

bool Ship::revive()
{
	std::bernoulli_distribution revivalDist(0.3);

	if (_isDead)
	{
		++_cooldown;
		if (_cooldown > 1 && _cooldown < 6)
		{
			
			return revivalDist(mt);
		}
		else
			return false;
	}
	return false;
}

bool operator==(const Ship& ship1, const Ship& ship2)
{
	return (ship1._cp1 == ship2._cp1) && (ship1._cp2 == ship2._cp2);
}