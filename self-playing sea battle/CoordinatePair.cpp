#include "CoordinatePair.h"

CoordinatePair::CoordinatePair(int32_t x, int32_t y)
	: _x(x), _y(y)
{}

CoordinatePair& CoordinatePair::operator=(const CoordinatePair& another)
{
	_x = another._x;
	_y = another._y;
	return *this;
}

bool operator==(const CoordinatePair& cp1, const CoordinatePair& cp2)
{
	return (cp1._x == cp2._x) && (cp1._y == cp2._y);
}