#pragma once
#include <iostream>

struct CoordinatePair
{
	int32_t _x;
	int32_t _y;

	CoordinatePair(int32_t x, int32_t y);

	CoordinatePair& operator=(const CoordinatePair& another);
};
bool operator==(const CoordinatePair& cp1, const CoordinatePair& cp2);