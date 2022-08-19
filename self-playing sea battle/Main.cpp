#include <thread>
#include <chrono>
#include <mutex>
#include "NavalForcesCommand.h"

#ifdef _WIN32
#include <Windows.h>
#define consoleClear system("cls")
#else
#define consoleClear system("clear")
#endif

bool _gameOver;
std::mt19937 Ship::mt(std::random_device{}());
std::vector<std::string> NavalForces::_gameLogs;
std::mt19937 NavalForces::mt(std::random_device{}());

bool NavalForces::_berserkMode = false;

void turn(NavalForces& team, NavalForces& enemy)
{
	while (!_gameOver)
	{
		team.yourTurn(enemy);

		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
}

int main()
{
	while (true)
	{
		consoleClear;
		_gameOver = false;
		NavalForces seaLions("seaLions");
		NavalForces silentChewbaccas("silentChewbaccas");

		std::bernoulli_distribution whoStarts(0.5);

		bool teamFirstStarts = whoStarts(NavalForces::mt);

		if (teamFirstStarts)
		{
			std::thread teamFirstThread(&turn, std::ref(seaLions), std::ref(silentChewbaccas));
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			std::thread teamSecondThread(&turn, std::ref(silentChewbaccas), std::ref(seaLions));

			teamFirstThread.join();
			teamSecondThread.join();
		}
		else
		{
			std::thread teamSecondThread(&turn, std::ref(silentChewbaccas), std::ref(seaLions));
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			std::thread teamFirstThread(&turn, std::ref(seaLions), std::ref(silentChewbaccas));

			teamFirstThread.join();
			teamSecondThread.join();
		}
		NavalForces::_gameLogs.clear();
		std::cout << "Game Over!\n";

		std::cout << "Try again ? Y N\n";
		std::string answer("");
		while (true)
		{
			std::cin >> answer;
			if (answer != "N" && answer != "n" && answer != "Y" && answer != "y")
			{
				std::cout << "Wrong input. Try again\n";
			}
			else
			{
				break;
			}
		}

		if (answer == "Y" || answer == "y")
		{
			continue;
		}
		else if (answer == "N" || answer == "n")
		{
			std::cout << "\nBye!";
			break;
		}
	}
}