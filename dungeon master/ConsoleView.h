#pragma once
#include <iostream>
#include <string>
#include "Field.h"
#include <chrono>

extern std::chrono::high_resolution_clock::time_point _time;

class ConsoleView
{
public:

	ConsoleView();

	void update();

	void movePlayer(const char& move);

	void showItems();

	const Player& getPlayer() const;
	Player& getPlayer();

private:
	std::unique_ptr<Field> _field;
	std::unique_ptr<Player> _player;

	int32_t _enemyCounter;
};
