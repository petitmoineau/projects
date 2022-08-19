#pragma once

#include "Utils.h"
#include <iostream>

class Passenger
{
public:
	Passenger(const uint32_t currentFloor = 0, const uint32_t neededFloor = 0);

	inline Direction direction() const { return std::max(currentFloor, neededFloor) == currentFloor ? Direction::Down : Direction::Up; }
	inline uint32_t getCurrentFloor() const { return currentFloor; }
	inline uint32_t getNeededFloor() const { return neededFloor; }

private:
	uint32_t currentFloor;
	uint32_t neededFloor;
};