#pragma once
#include "Elevator.h"
#include "Utils.h"

class State
{
protected:
	Elevator* elevator;
public:
	virtual ~State() {}

	void setElevator(Elevator* elevator)
	{
		this->elevator = elevator;
	}

	virtual void move() = 0;
	virtual void openDoors() = 0;
	virtual void closeDoors() = 0;
};

class MoveState : public State
{
public:
	void move() override
	{
		elevator->move();
	}
	
	void openDoors() override {}
	void closeDoors() override {}
};

class OpeningDoorsState : public State
{
public:
	void move() override{}
	void closeDoors() override {}
	void openDoors() override 
	{
		elevator->notifyBuildingSystem(DoorStates::Open);
	}
};

class ClosingDoorsState : public State
{
public:
	void move() {}
	void openDoors() override {}
	void closeDoors() override 
	{
		elevator->notifyBuildingSystem(DoorStates::Closed);
		if (elevator->direction() != Direction::Wait)
		{
			elevator->changeState(new MoveState());
		}
	}
};