#pragma once
#include "Passenger.h"
#include "Building.h"
#include <iostream>
#include <vector>
#include <set>

class State;

class Elevator
{
public:
	Elevator();
	void changeState(State*);
	const Direction direction() const;
	void move();
	void work();
	void passengersComeIn(std::array<std::deque<Passenger>, Building::floors>&);
	void passengersComeOut(std::vector<Passenger>&);

	void bindBuildingSystem(Building*);
	void notifyBuildingSystem(const DoorStates&);

	void addCallInStack(const uint32_t&);

	inline int32_t getSpeed() const { return speed; }
	inline int32_t getFloor() const { return floor; }
	inline int32_t getPassengersInElevator() const { return inElevator.size(); }

private:
	static constexpr uint32_t maxPassengersAmount = 13;

	uint32_t floor = 0;
	uint32_t speed = 0;
	Direction currentDirection = Direction::Wait;

	std::set <uint32_t> upCallStack;
	std::set<uint32_t, std::greater<int32_t>> downCallStack;

	std::vector<Passenger> inElevator;
	
	void analizeCallStack();

	Building* building;
	State* state;
};