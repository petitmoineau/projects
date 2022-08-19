#include <iostream>
#include "ConsoleView.h"

int32_t Item::_freeID = 0;
std::vector<std::string> Field::_field;
std::mt19937 Field::mt(std::random_device{}());
std::chrono::high_resolution_clock::time_point _time;
bool _gameOver;

int main()
{
	while (true)
	{
		_gameOver = false;
		ConsoleView cv;
		char move;
		std::string action;
		while (!_gameOver)
		{
			system("CLS");
			_time = std::chrono::high_resolution_clock::now();
			cv.update();
			std::cout << "Command <items> to see inventory, <wasd> to move: ";
			std::cin >> action;
			if (action.length() == 1)
			{
				move = action.at(0);
			}
			else if (action == "items")
			{
				cv.showItems();
				std::cout << "Write <drop> <n> to throw the item, or <close> to close the inventory\n";
				std::cout << "Command: ";
				std::cin.ignore();
				getline(std::cin, action);
				if (action == "close")
				{
					continue;
				}
				else
				{
					while (true)
					{
						if (action == "close") break;
						if (action.substr(0, 4) == "drop")
						{
							action.erase(0, 5);
							int32_t number = std::stoi(action);
							cv.getPlayer().getInventory().dropOrUseItem("", number);
							break;
						}
						std::cout << "Command: ";
						getline(std::cin, action);
					}
					continue;
				}
			}
			cv.movePlayer(move);
		}

		system("CLS");
		std::cout << "Game Over!\n";
			
		cv.getPlayer().printStats();
		cv.getPlayer().showInventory().printInventory();
		cv.getPlayer().printBittenEnemies();
			
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