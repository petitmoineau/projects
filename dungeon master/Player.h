#pragma once
#include <iostream>
#include "Parameter.h"
#include "Inventory.h"
#include <random>
#include "Enemy.h"
#include <Windows.h>
#include <unordered_map>

class Enemy;

enum class DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Player
{
public:
	Player(int32_t level = 1);

	void setInitPosition(const int32_t& playerPosX, const int32_t& playerPosY);

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	const char& playerSymbol() const;

	const int32_t& playerPosX() const;

	const int32_t& playerPosY() const;

	const DIRECTION& direction() const;

	const int32_t& level() const;

	const int32_t& experience() const;

	void levelUp();

	bool pickUpItem(std::unique_ptr<Item>& item);
	
	void beingAttacked(Enemy& enemy, const int32_t& attack);

	bool usePotion();

	void attackEnemy(Enemy& enemy);

	Inventory& getInventory();

	const Inventory& showInventory() const;

	const Parameter& getProtection() const;

	const Parameter& getHealth() const;

	void addBittenEnemy(const int32_t& level);

	void addExperience();

	void printStats() const;

	void printBittenEnemies() const;

private:

	const char _playerSymbol;
	int32_t _playerPosX;
	int32_t _playerPosY;
	DIRECTION _playerDirection;
	std::string _playerName;
	int32_t _level;
	int32_t _experience;

	Inventory _inventory;

	std::unique_ptr<Parameter> _health;
	std::unique_ptr<Parameter> _attack;
	std::unique_ptr<Parameter> _protection;

	std::unique_ptr<Item> _currentShield;
	std::unique_ptr<Item> _currentSword;

	std::unordered_multimap<std::string, int32_t> _enemiesBitten;
};
