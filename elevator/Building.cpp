#include "Building.h"
#include "Elevator.h"
#include "State.h"
#include <algorithm>

Building::Building()
	: elevatorOpen(false), elevator(new Elevator()), doorstate(DoorStates::Closed)
{
	elevator->bindBuildingSystem(this);
}

void Building::momentOfTime(std::deque<Passenger> generatedPassengers)
{
	if (elevatorOpen)
	{
		elevator->passengersComeOut(passengersAfterRide);
		elevator->passengersComeIn(passengersBeforeRide);
	}
	else
	{
		passengersAfterRide.clear();
		passengerAppears(generatedPassengers);
	}
	elevator->work();
}

const std::vector<Passenger>& Building::getPassengersAfterRide() const
{
	return passengersAfterRide;
}

const std::array<std::deque<Passenger>, Building::floors>& Building::getPassengersBeforeRide() const
{
	return passengersBeforeRide;
}

void Building::updateSystem(const DoorStates& currentDoorstate)
{
	doorstate = currentDoorstate;
	if (doorstate == DoorStates::Closed)
	{
		elevatorOpen = false;
	}
	else
	{
		elevatorOpen = true;
	}
}

uint32_t Building::getElevatorFloor() const
{
	return elevator->getFloor();
}

uint32_t Building::getElevatorPassAmount() const
{
	return elevator->getPassengersInElevator();
}

uint32_t Building::getElevatorSpeed() const
{
	return elevator->getSpeed();
}

bool Building::elevatorDoorsOpen() const
{
	return doorstate == DoorStates::Closed ? false : true;
}

std::string Building::getElevatorDirection() const
{
	return elevator->direction() == Direction::Up ? "Up" : elevator->direction() == Direction::Down ? "Down" : "Wait";
}

void Building::passengerAppears(std::deque<Passenger>& generatedPassengers)
{
	std::for_each(generatedPassengers.begin(), generatedPassengers.end(), [&](Passenger& passenger)
		{
			if (passengersBeforeRide.at(passenger.getCurrentFloor()).size() >= Building::passLimitOnFloor)
			{
				postponedPassengers.push_back(passenger);
			}
			else
			{
				passengersBeforeRide.at(passenger.getCurrentFloor()).push_back(passenger);
				callElevator(passenger);
			}
		});


	if (!postponedPassengers.empty())
	{
		std::for_each(postponedPassengers.begin(), postponedPassengers.end(), [&](Passenger& passenger)
			{
				if (passengersBeforeRide.at(passenger.getCurrentFloor()).size() < Building::passLimitOnFloor)
				{
					passengersBeforeRide.at(passenger.getCurrentFloor()).push_back(passenger);
					callElevator(passenger);
				}
			});
	}
}

void Building::callElevator(const Passenger& passenger)
{
	elevator->addCallInStack(passenger.getCurrentFloor());
}