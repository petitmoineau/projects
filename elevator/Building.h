#pragma once
#include "Utils.h"
#include "Passenger.h"
#include <array>
#include <deque>
#include <vector>
#include <iostream>

class Elevator;

class Building
{
public:
	Building();
	void momentOfTime(std::deque<Passenger>);

	static constexpr uint32_t floors = 12;
	static constexpr uint32_t passLimitOnFloor = 5;
	const std::vector<Passenger>& getPassengersAfterRide() const;
	const std::array<std::deque<Passenger>, floors>& getPassengersBeforeRide() const;
	void updateSystem(const DoorStates&);

	uint32_t getElevatorFloor() const;
	uint32_t getElevatorPassAmount() const;
	uint32_t getElevatorSpeed() const;
	bool elevatorDoorsOpen() const;
	std::string getElevatorDirection() const;

private:
	void passengerAppears(std::deque<Passenger>&);
	void callElevator(const Passenger&);

	bool elevatorOpen;
	Elevator* elevator;
	DoorStates doorstate;

	std::array<std::deque<Passenger>, floors> passengersBeforeRide;
	std::vector<Passenger> passengersAfterRide;
	std::vector<Passenger> postponedPassengers;
};