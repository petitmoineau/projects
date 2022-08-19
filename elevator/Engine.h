#pragma once
#include "Building.h"
#include "Passenger.h"
#include <array>
#include <deque>
#include <random>
#include <string>
#include <map>

class Engine
{
public:
	Engine();
	
	void render();
	void run();
	GameState gamestate = GameState::GameRuns;

private:
	Building build;
	std::array<std::deque<Passenger>, Building::floors> passengersBefore;
	std::vector<Passenger> passengersAfter;
	std::vector<std::string> building;

	void loadLevel();
	void loadScenario();
	std::deque<Passenger> generatePassengers();
	std::map<uint32_t, std::deque<Passenger>> scenario;

	static std::bernoulli_distribution probabilityOfPassenger;
	static std::uniform_int_distribution<uint32_t> floorGenerator;

	uint32_t gameTick = 0;

	const char pass = 'P';
	const char empty = '_';
};