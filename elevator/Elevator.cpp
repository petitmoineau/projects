#include "Elevator.h"
#include "State.h"
#include <algorithm>

Elevator::Elevator()
	: state(nullptr)
{
	changeState(new ClosingDoorsState());
}

void Elevator::changeState(State* newState)
{
	if (this->state != nullptr)
		delete this->state;
	this->state = newState;
	this->state->setElevator(this);
}

const Direction Elevator::direction() const
{
	return currentDirection;
}

void Elevator::work()
{
	analizeCallStack();

	state->move();
	state->closeDoors();
	state->openDoors();
}

void Elevator::passengersComeIn(std::array<std::deque<Passenger>, Building::floors>& passengersBeforeRide)
{
	if (!passengersBeforeRide[floor].empty())
	{
		for (uint32_t placesLeft = std::min(maxPassengersAmount - inElevator.size(), passengersBeforeRide[floor].size()); placesLeft > 0; placesLeft--)
		{
			inElevator.push_back(passengersBeforeRide[floor].front());
			addCallInStack(passengersBeforeRide[floor].front().getNeededFloor());

			passengersBeforeRide[floor].pop_front();
		}
	}
	
	this->changeState(new ClosingDoorsState());
}

void Elevator::passengersComeOut(std::vector<Passenger>& passengersAfterRide)
{
	auto it = std::find_if(inElevator.begin(), inElevator.end(), [&](Passenger& passenger)
		{return passenger.getNeededFloor() == floor; });

	while (it != inElevator.end())
	{
		passengersAfterRide.push_back(*it);
		inElevator.erase(it);

		it = std::find_if(inElevator.begin(), inElevator.end(), [&](Passenger& passenger)
			{return passenger.getNeededFloor() == floor; });
	}
}

void Elevator::bindBuildingSystem(Building* building)
{
	this->building = building;
}

void Elevator::notifyBuildingSystem(const DoorStates& currentDoorstate)
{
	building->updateSystem(currentDoorstate);
}

void Elevator::addCallInStack(const uint32_t& call)
{
	call > floor ? upCallStack.insert(call) : downCallStack.insert(call);
}

void Elevator::analizeCallStack()
{
	if (currentDirection == Direction::Wait && (!upCallStack.empty() || !downCallStack.empty()))
	{
		if (!upCallStack.empty())
		{
			currentDirection = Direction::Up;
		}
		else if (!downCallStack.empty())
		{
			currentDirection = Direction::Down;
		}
		this->changeState(new MoveState());
	}

	if (currentDirection == Direction::Up && upCallStack.empty())
	{
		if (!downCallStack.empty())
		{
			currentDirection = Direction::Down;
		}
		else
		{
			currentDirection = Direction::Wait;
		}
	}
	else if (currentDirection == Direction::Down && downCallStack.empty())
	{
		if (!upCallStack.empty())
		{
			currentDirection = Direction::Up;
		}
		else
		{
			currentDirection = Direction::Wait;
		}
	}

	if (currentDirection == Direction::Up)
	{
		auto passengerIt = upCallStack.find(floor);
		if (passengerIt != upCallStack.end())
		{
			speed = 0;
			this->changeState(new OpeningDoorsState());
			upCallStack.erase(passengerIt);
		}
	}
	else if (currentDirection == Direction::Down)
	{
		auto passengerIt = downCallStack.find(floor);
		if (passengerIt != downCallStack.end())
		{
			speed = 0;
			this->changeState(new OpeningDoorsState());
			downCallStack.erase(passengerIt);
		}
	}

	if (inElevator.empty() && upCallStack.empty() && downCallStack.empty())
	{
		currentDirection = Direction::Wait;
	}
}

void Elevator::move()
{
	if (currentDirection == Direction::Up)
	{
		if ((*(upCallStack.begin()) - floor) >= std::pow(speed+1, speed+1))
		{
			++speed;
		}
		
		uint32_t util = 0;
		for (uint32_t i = speed; i > 0; i--) util += i;

		if ((*(upCallStack.begin()) - floor) < util)
		{
			--speed;
		}
		floor += speed;
	}
	else
	{
		if ((floor - *(downCallStack.begin())) >= std::pow(speed + 1, speed + 1))
		{
			++speed;
		}

		uint32_t util = 0;
		for (uint32_t i = speed; i > 0; i--) util += i;

		if ((floor - *(downCallStack.begin())) < util)
		{
			--speed;
		}
		floor -= speed;
	}
}