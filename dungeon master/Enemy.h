#pragma once
#include "Player.h"
#include <iostream>
#include <random>
#include <Windows.h>

class Player;

class Enemy
{
public:
	Enemy(int32_t level = 1);

	void attackPlayer(Player& player);

	const char& getEnemySymbol() const;

	void beingAttacked(Player& player, const int32_t& attack);

	const Parameter& getProtection() const;

	const Parameter& getHealth() const;

private:
	int32_t _level;

	char _symbol;

	std::unique_ptr<Parameter> _health;
	std::unique_ptr<Parameter> _attack;
	std::unique_ptr<Parameter> _protection;

	std::unique_ptr<Item> _currentShield;
	std::unique_ptr<Item> _currentSword;
};