#pragma once
#include <fstream>
#include <cassert>
#include <vector>
#include <string>
#include "Player.h"

extern bool _gameOver;

class Field
{
public:
	Field(Player& player);

	const int32_t& getHeight() const;

	const int32_t& getWidth() const;

	void checkCollision(Player& player);

	void checkEnemyAround(Player& player);

	void insertEnemyInField(const int32_t& coordX, const int32_t& coordY, const char& symbol);

	void insertItemInField(const int32_t& coordX, const int32_t& coordY, const char& symbol);

	void insertInField(const int32_t& coordX, const int32_t& coordY, Player& player);

	void deleteFromField(const int32_t& coordX, const int32_t& coordY);

	void movePlayer(Player& player);

	void initPlayer(Player& player);

	void initField();

	void readDatabase();

	static void draw();

	void addItemOnField(Item item);

	void addEnemyOnField(Enemy enemy);

private:
	std::ifstream databaseFile;
	int32_t _width;
	int32_t _height;
	static std::vector<std::string> _field;
	static std::mt19937 mt;

	int32_t _itemCounter;
	int32_t _enemyCounter;

	char _bufferedItem;
};
