#include "Field.h"

Field::Field(Player& player)
{
	initField();

	initPlayer(player);
}

void Field::readDatabase()
{
	std::string currentLine;

	databaseFile.open("field.txt", std::ios::in);
	while (!databaseFile.eof())
	{
		getline(databaseFile, currentLine);
		_field.push_back(currentLine);
	}
	databaseFile.close();
}

void Field::initField()
{
	_field.clear();
	readDatabase();
	_itemCounter = 0;
	_enemyCounter = 0;
	_bufferedItem = ' ';

	_height = _field.size();
	_width = _field.at(0).length();
}

const int32_t& Field::getHeight() const
{
	return this->_height;
}

const int32_t& Field::getWidth() const
{
	return this->_width;
}

void Field::checkCollision(Player& player)
{
	switch (_field.at(player.playerPosY()).at(player.playerPosX()))
	{
	case '0':
	{
		std::unique_ptr<Item> shield(new Item("shit-kartonka", player.level()));
		if (!player.pickUpItem(shield))
		{
			_bufferedItem = '0';
		}
		else
			--_itemCounter;
	}
	break;
	case '!':
	{
		std::unique_ptr<Item> sword(new Item("dubinka", player.level()));
		if (!player.pickUpItem(sword))
		{
			_bufferedItem = '!';
		}
		else
			--_itemCounter;
	}
	break;
	case '8':
	{
		std::unique_ptr<Item> potion(new Item("butylka-piva"));
		if (!player.pickUpItem(potion))
		{
			_bufferedItem = '8';
		}
		else
			--_itemCounter;
	}
	break;
	default:
		break;
	}
}

void Field::checkEnemyAround(Player& player)
{

	int32_t coordX = player.playerPosX();
	int32_t coordY = player.playerPosY();
	bool _enemyFound = false;

	if (coordY < 5 && _field.at(coordY + 1).at(coordX) == 'e'
		|| coordY < 5 && _field.at(coordY + 1).at(coordX) == 'Y'
		|| coordY < 5 && _field.at(coordY + 1).at(coordX) == '$')
	{
		++coordY;
		_enemyFound = true;
	}
	else if (coordY > 1 && _field.at(coordY - 1).at(coordX) == 'e'
		|| coordY > 1 && _field.at(coordY - 1).at(coordX) == 'Y'
		|| coordY > 1 && _field.at(coordY - 1).at(coordX) == '$')
	{
		--coordY;
		_enemyFound = true;
	}
	else if (coordX < 9 && _field.at(coordY).at(coordX + 1) == 'e'
		|| coordX < 9 && _field.at(coordY).at(coordX + 1) == 'Y'
		|| coordX < 9 && _field.at(coordY).at(coordX + 1) == '$')
	{
		++coordX;
		_enemyFound = true;
	}
	else if (coordX > 1 && _field.at(coordY).at(coordX - 1) == 'e'
		|| coordX > 1 && _field.at(coordY).at(coordX - 1) == 'Y'
		|| coordX > 1 && _field.at(coordY).at(coordX - 1) == '$')
	{
		--coordX;
		_enemyFound = true;
	}
	
	
	if (_enemyFound)
	{
		Enemy enemy(player.level());

		player.attackEnemy(enemy);

		if (enemy.getHealth().getParamBar() <= 0)
		{
			deleteFromField(coordX, coordY);
			--_enemyCounter;

			player.addExperience();
			player.addBittenEnemy(player.level());

			Sleep(5000);
		}
		if (player.getHealth().getParamBar() <= 0)
		{
			deleteFromField(player.playerPosX(), player.playerPosY());
			_gameOver = true;
		}
	}
}

void Field::insertEnemyInField(const int32_t& coordX, const int32_t& coordY, const char& symbol)
{
	if (_enemyCounter < 2)
	{
		++_enemyCounter;
		_field.at(coordY).at(coordX) = symbol;
	}
}

void Field::insertItemInField(const int32_t& coordX, const int32_t& coordY, const char& symbol)
{
	if (_itemCounter < 2)
	{
		++_itemCounter;
		_field.at(coordY).at(coordX) = symbol;
	}
}

void Field::insertInField(const int32_t& coordX, const int32_t& coordY, Player& player)//check necessity
{
	if (coordX == 0 || coordX == 10
		|| coordY == 0 || (coordX > 0 && coordX < 10 && coordY == 6))
	{
		initPlayer(player);
		initField();
		insertInField(player.playerPosX(), player.playerPosY(), player);
	}
	else
	{
		checkCollision(player);
		_field.at(coordY).at(coordX) = player.playerSymbol();
		checkEnemyAround(player);
	}
}

void Field::deleteFromField(const int32_t& coordX, const int32_t& coordY)
{
	if (_bufferedItem != ' ')
	{
		_field.at(coordY).at(coordX) = _bufferedItem;
		_bufferedItem = ' ';
	}
	else if (coordX == 0 || coordX == 10)
	{
		_field.at(coordY).at(coordX) = '|';
	}
	else if (coordY == 0 || (coordX > 0 && coordX < 10 && coordY == 5))
	{
		_field.at(coordY).at(coordX) = '_';
	}
	else
	{
		_field.at(coordY).at(coordX) = ' ';
	}
}

void Field::movePlayer(Player& player)
{
	if (player.direction() == DIRECTION::UP)
	{
		deleteFromField(player.playerPosX(), player.playerPosY() + 1);
	}
	else if (player.direction() == DIRECTION::DOWN)
	{
		deleteFromField(player.playerPosX(), player.playerPosY() - 1);
	}
	else if (player.direction() == DIRECTION::LEFT)
	{
		deleteFromField(player.playerPosX() + 1, player.playerPosY());
	}
	else if (player.direction() == DIRECTION::RIGHT)
	{
		deleteFromField(player.playerPosX() - 1, player.playerPosY());
	}

	insertInField(player.playerPosX(), player.playerPosY(), player);
}


void Field::initPlayer(Player& player)
{
	player.setInitPosition(_width / 2, _height / 2);
}

void Field::draw()
{
	for (int i = 0; i < _field.size(); i++)
	{
		std::cout << _field.at(i) << "\n";
	}
}

void Field::addItemOnField(Item item)
{
	int32_t x(0);
	int32_t y(0);
	while (true)
	{
		std::uniform_int_distribution<int32_t> distX(1, 10);
		x = distX(mt);

		std::uniform_int_distribution<int32_t> distY(1, 5);
		y = distY(mt);

		if (_field.at(y).at(x) == ' ')
		{
			break;
		}
	}

	insertItemInField(x, y, item.getItemSymbol());
}

void Field::addEnemyOnField(Enemy enemy)
{
	int32_t x(0);
	int32_t y(0);
	while (true)
	{
		std::uniform_int_distribution<int32_t> distX(1, 10);
		x = distX(mt);

		std::uniform_int_distribution<int32_t> distY(1, 5);
		y = distY(mt);

		if (_field.at(y).at(x) == ' ')
		{
			break;
		}
	}

	insertEnemyInField(x, y, enemy.getEnemySymbol());
}