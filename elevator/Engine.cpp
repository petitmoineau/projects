#include "Engine.h"
#include <fstream>
#ifdef _WIN32
#define console_clear system("CLS")
#else
#define console_clear system("clear")
#endif

namespace
{
	std::random_device rd;
	std::mt19937 random_engine(rd());

	void replaceSymbol(const std::array<std::deque<Passenger>, Building::floors>& passengersBefore, std::vector<std::string>& building, const char symbol)
	{
		uint32_t counter = Building::floors;
		std::for_each(passengersBefore.begin(), passengersBefore.end(), [&](const std::deque<Passenger>& passOnFloor)
			{
				building[counter].replace(building[counter].size() - Building::passLimitOnFloor - 1, passOnFloor.size(), passOnFloor.size(), symbol);
				counter--;
			});
	}
}

Engine::Engine()
{
	loadLevel();
	loadScenario();
}

void Engine::loadLevel()
{
	std::ifstream databaseFile("Level.txt", std::ios::in);
	std::string currentLine;
	while (!databaseFile.eof())
	{
		getline(databaseFile, currentLine);
		building.push_back(currentLine);
	}
	databaseFile.close();
}

void Engine::loadScenario()
{
	std::fstream map_file("Scenario.csv", std::ios::in);
	std::string scenarioString;
	map_file >> scenarioString;
	if (scenarioString.size() != 0)
	{
		const char delim = ',';
		for (uint32_t i = 0; scenarioString.length() != 0; i++)
		{
			std::deque<Passenger> passengersOnCurrentTick;
			uint32_t tick;
			if (scenarioString.at(0) == 's')
			{
				scenarioString.erase(0, scenarioString.find(delim) + 1);
				std::string tickString = scenarioString.substr(0, scenarioString.find(delim));
				tick = std::atoi(tickString.c_str());
				scenarioString.erase(0, tickString.size() + 1);
			}

			uint32_t passData[2];
			while (!scenarioString.empty() && scenarioString.at(0) == 'p')
			{
				for (uint32_t i = 0; i < 2; i++)
				{
					scenarioString.erase(0, scenarioString.find(delim) + 1);
					passData[i] = std::atoi(scenarioString.substr(0, scenarioString.find(delim)).c_str());
				}

				if (scenarioString.find(delim) != std::string::npos)
				{
					scenarioString.erase(0, scenarioString.find(delim) + 1);
				}
				else
				{
					scenarioString.clear();
				}


				passengersOnCurrentTick.push_back(Passenger(passData[0], passData[1]));
			}
			scenario.insert(std::make_pair(tick, passengersOnCurrentTick));
		}
	}
	map_file.close();
}

std::bernoulli_distribution Engine::probabilityOfPassenger{ 0.50 };
std::uniform_int_distribution<uint32_t> Engine::floorGenerator{ 0, Building::floors - 1 };

std::deque<Passenger> Engine::generatePassengers()
{
	std::deque<Passenger> generatedPassengers;

	if (!scenario.empty())
	{
		if (scenario.find(gameTick) != scenario.end())
		{
			while (!scenario[gameTick].empty())
			{
				generatedPassengers.push_back(scenario[gameTick].at(0));
				scenario[gameTick].pop_front();
			}
		}
	}
	else if (probabilityOfPassenger(random_engine))
	{
		uint32_t passCurrentFloor = floorGenerator(random_engine);
		uint32_t floorToGo;

		if (passCurrentFloor == 0)
		{
			while ((floorToGo = floorGenerator(random_engine)) == passCurrentFloor);
		}
		else
		{
			floorToGo = 0;
		}
		generatedPassengers.push_back(Passenger(passCurrentFloor, floorToGo));
	}
	gameTick++;
	return generatedPassengers;
}

void Engine::render()
{
	passengersBefore = build.getPassengersBeforeRide();
	passengersAfter = build.getPassengersAfterRide();

	replaceSymbol(passengersBefore, building, pass);

	uint32_t counter = Building::floors - build.getElevatorFloor();
	auto numberOfPass = std::min(passengersAfter.size(), (size_t)8);
	building[counter].replace(1, numberOfPass, numberOfPass, pass);

	console_clear;
	counter = Building::floors;
	std::for_each(building.begin(), building.end(), [&](std::string line)
		{
			if (counter == build.getElevatorFloor())
			{
				if (build.elevatorDoorsOpen())
				{
					line.replace(line.length()/2, 1, "O");
				}
				line.append("<--- Elevator is here");
			}
			std::cout << line << "\n";
			if (counter != 0)
			{
				line.replace(line.length() / 2 + 1, 1, "|");
			}
			counter--;
		}
	);
	std::cout << "Passengers in elevator: " << build.getElevatorPassAmount() << "\n";
	std::cout << "Elevator speed: " << build.getElevatorSpeed() << "\n";
	std::cout << "Elevator direction: " << build.getElevatorDirection() << "\n";
	
	replaceSymbol(passengersBefore, building, empty);

	counter = Building::floors - build.getElevatorFloor();
	building[counter].replace(1, numberOfPass, numberOfPass, empty);
}

void Engine::run()
{
	if (gameTick == 400)
	{
		gamestate = GameState::GameOver;
	}
	build.momentOfTime(generatePassengers());
}
