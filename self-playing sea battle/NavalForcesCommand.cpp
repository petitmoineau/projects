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

NavalForces::NavalForces(const std::string& teamName)
	: _teamName(teamName)
{
	readDatabase();
	_height = _field.size();
	_width = _field.at(0).length();
	initShipPlacement();
}

char NavalForces::receiveMissile(const CoordinatePair& cp)
{
	if (_field.at(cp._y).at(cp._x) == _wreckedShip)
	{
		return '!';
	}
	else if (_field.at(cp._y).at(cp._x) == _empty || _field.at(cp._y).at(cp._x) == _aroundShip)
	{
		return _sea;
	}
	else if (_field.at(cp._y).at(cp._x) == _ship)
	{
		auto it = searchThroughFlotilla(false, cp);

		if (it->_type == SHIPTYPE::SINGLE_DECK)
		{
			it->wasShot(SHIPTYPE::SINGLE_DECK); 
			checkIfGameOver();
			_field.at(cp._y).at(cp._x) = _wreckedShip;
			
			return _wreckedShip;
		}
		else
		{
			it->wasShot(SHIPTYPE::DOUBLE_DECK);
			checkIfGameOver();
			if (it->_isDead)
			{
				_field.at(it->_cp2._y).at(it->_cp2._x) = _wreckedShip;
				_field.at(it->_cp1._y).at(it->_cp1._x) = _wreckedShip;
				
				return _wreckedShip;
			}
			else
			{
				_field.at(cp._y).at(cp._x) = _woundedShip;
				return _woundedShip;
			}
		}
	}
}

const std::string& NavalForces::getTeamName() const
{
	return _teamName;
}

void NavalForces::informAboutRevival(const std::string& teamName)
{
	std::lock_guard<std::mutex> lg(mtx3);
	_gameLogs.push_back(_teamName + "`s ship has revived!\n");
	_enemyShipRevived = true;
}

void NavalForces::printDeck()
{
	std::lock_guard<std::mutex> lg(mtx);
	consoleClear;
	std::cout << "Own field\n" + _teamName + "\n";
	for (int32_t i = 0; i < _field.size(); i++)
	{
		std::cout << _field.at(i) << "\n";
	}

	std::cout << "Enemy field\n";
	for (int32_t i = 0; i < _enemyField.size(); i++)
	{
		std::cout << _enemyField.at(i) << "\n";
	}
	printLogs();
}

void NavalForces::printLogs()
{
	int32_t i;
	if (_gameLogs.size() < 5)
		i = _gameLogs.size() - 1;
	else
		i = 3;
	for (i; i >= 0; i--)
	{
		std::cout << *(_gameLogs.rbegin() + i);
	}
}

void NavalForces::readDatabase()
{
	std::string currentLine;
	databaseFile.open("field.txt", std::ios::in);
	while (!databaseFile.eof())
	{
		getline(databaseFile, currentLine);
		_field.push_back(currentLine);
		_enemyField.push_back(currentLine);
	}
	databaseFile.close();
}

void NavalForces::initShipPlacement()
{
	std::uniform_int_distribution<int32_t> distCoord(1, 5);

	for (int32_t i = 0; i < 3; i++)
	{
		while (true)
		{
			CoordinatePair cp(distCoord(mt), distCoord(mt));
			cp = checkToPlace(cp);

			if (cp._x != -1)
			{
				_field.at(cp._y).at(cp._x) = _ship;
				_flotilla.push_back(Ship(SHIPTYPE::SINGLE_DECK, cp));
				fillCellsAroundShip(_field, cp);
				break;
			}
		}
	}

	CoordinatePair cp1(-1, -1);
	CoordinatePair cp2(-1, -1);
	int32_t shipCounter = 0;
	int32_t index = 0;

	for (auto it : _field)
	{
		int32_t x = it.find("__");
		if (x > 0 && x < _width - 1 && index != 0 && shipCounter < 2)
		{
			cp1 = CoordinatePair(x, index);
			cp2 = CoordinatePair(x + 1, index);
			placeDoubleDeckOnField(cp1, cp2);
			++shipCounter;
		}
		++index;
	}

	for (int32_t i = 1; i < _height; i++)
	{
		for (int32_t j = 1; j < _width - 1; j++)
		{
			if (_field.at(i).at(j) == _empty && i + 1 < _height && shipCounter < 2)
			{
				if (_field.at(i + 1).at(j) == _empty)
				{
					cp1 = CoordinatePair(j, i);
					cp2 = CoordinatePair(j, i + 1);
					placeDoubleDeckOnField(cp1, cp2);
					++shipCounter;
				}
			}
		}
	}
}

void NavalForces::placeDoubleDeckOnField(const CoordinatePair& cp1, const CoordinatePair& cp2)
{
	_field.at(cp1._y).at(cp1._x) = _ship;
	_field.at(cp2._y).at(cp2._x) = _ship;
	_flotilla.push_back(Ship(SHIPTYPE::DOUBLE_DECK, cp1, cp2));
	fillCellsAroundShip(_field, cp1, cp2);
}

const CoordinatePair NavalForces::checkToPlace(const CoordinatePair& cp) const
{
	if (_field.at(cp._y).at(cp._x) == _empty)
	{
		return cp;
	}
	return CoordinatePair(-1, -1);
}

bool NavalForces::checkToShoot(const CoordinatePair& cp) const
{
	if (_enemyField.at(cp._y).at(cp._x) == _empty)
		return true;
	return false;
}

void NavalForces::yourTurn(NavalForces& enemy)
{
	printDeck();
	
	if (_enemyShipRevived)
	{
		std::for_each(_enemyFlotilla.begin(), _enemyFlotilla.end(), [this, &enemy](Ship& ship)
			{
				ship._isDead = false;
				ship._isWounded = false;
				if (ship._type == SHIPTYPE::SINGLE_DECK)
				{
					_enemyField.at(ship._cp1._y).at(ship._cp1._x) = _empty;
				}
				else
				{
					_enemyField.at(ship._cp1._y).at(ship._cp1._x) = _empty;
					_enemyField.at(ship._cp2._y).at(ship._cp2._x) = _empty;
				}
			});
		_enemyShipRevived = false;
	}

	auto it1 = std::find_if(_enemyFlotilla.begin(), _enemyFlotilla.end(), [this, &enemy](Ship& ship)
		{
			if (!ship._isDead)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				if (ship._type == SHIPTYPE::SINGLE_DECK)
				{
					return reshootRevivedShips(enemy, ship._cp1);
				}
				else
				{
					return reshootRevivedShips(enemy, ship._cp1, ship._cp2);
				}
			}
			return false;
		});

	
	if (it1 != _enemyFlotilla.end())
	{
		mtx1.lock();
		_gameLogs.push_back(_teamName + " has shot a missile and hit enemy`s ship!\n");
		mtx1.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		yourTurn(enemy);
	}

	if (!_gameOver && shoot(enemy))
	{
		mtx2.lock();
		_gameLogs.push_back(_teamName + " has shot a missile and hit enemy`s ship!\n");
		mtx2.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		yourTurn(enemy);
	}

	
	if (!_gameOver)
	{
		auto it = searchThroughFlotilla(true);
		if (it != _flotilla.end())
		{
			it->_isDead = false;
			it->_isWounded = false;
			_field.at(it->_cp1._y).at(it->_cp1._x) = _ship;
			if (it->_cp2._x != -1)
			{
				_field.at(it->_cp2._y).at(it->_cp2._x) = _ship;
			}
			enemy.informAboutRevival(_teamName);
		}
	}
}

std::vector<Ship>::iterator NavalForces::searchThroughFlotilla(bool reviveAction, const CoordinatePair& cp /*= CoordinatePair(-1, -1))*/)
{
	std::lock_guard<std::mutex> lg(reviveMutex);
	std::vector<Ship>::iterator it;
	if (reviveAction)
	{
		it = std::find_if(_flotilla.begin(), _flotilla.end(), [](Ship& ship) { return ship.revive(); });
	}
	else
	{
		it = std::find_if(_flotilla.begin(), _flotilla.end(), [&cp](Ship& ship) { return ship._cp1 == cp || ship._cp2 == cp; });
	}
	return it;
}

void NavalForces::checkIfGameOver() const
{
	if (std::all_of(_flotilla.begin(), _flotilla.end(), [](const Ship& ship)
		{
			return ship._isDead;
		}))
	{
		_gameOver = true;
	}
}

void NavalForces::fillCellsAroundShip(std::vector<std::string>& field, const CoordinatePair& cp1, const CoordinatePair& cp2)
{
	for (int32_t i = cp1._y - 1; i < cp1._y + 2 && i < _height; i++)
	{
		for (int32_t j = cp1._x - 1; j < cp1._x + 2 && j < _width - 1; j++)
		{
			if (field.at(i).at(j) == _empty && i != 0)
			{
				field.at(i).at(j) = _aroundShip;
			}
		}
	}
	if (cp2._x != -1)
	{
		for (int32_t i = cp2._y - 1; i < cp2._y + 2 && i < _height; i++)
		{
			for (int32_t j = cp2._x - 1; j < cp2._x + 2 && j < _width - 1; j++)
			{
				if (field.at(i).at(j) == _empty && i != 0)
				{
					field.at(i).at(j) = _aroundShip;
				}
			}
		}
	}
}

void NavalForces::checkBerserkMode(NavalForces& enemy)
{
	if (!_berserkMode)
	{
		int32_t deadShipCounter = 0;
		std::for_each(_flotilla.begin(), _flotilla.end(), [this, &deadShipCounter](Ship& ship)
			{
				if (ship._isDead)
				{
					++deadShipCounter;
				}
			});

		if (deadShipCounter == _flotilla.size() - 1)
		{
			_berserkMode = true;
			mtx.lock();
			_gameLogs.push_back(_teamName + " turned berserk mode on! beware!\n");
			mtx.unlock();
			int32_t wasShot = 0;
			for (int i = 1; i < 4; i++)
			{
				if (_gameOver)
				{
					break;
				}
				mtx1.lock();
				_gameLogs.push_back(std::to_string(i) + " missile has been shot!\n");
				mtx1.unlock();
				if (shoot(enemy))
				{
					++wasShot;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
			mtx2.lock();
			_gameLogs.push_back("Berserk mode is off");
			mtx2.unlock();
			if (wasShot > 0)
			{
				if (!_gameOver)
				{
					mtx2.lock();
					_gameLogs.push_back(_teamName + " makes last shot\n");
					mtx2.unlock();
					shoot(enemy);
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				}
			}
			_gameLogs.push_back("\nReturn to another player....\n");

		}
	}
}

bool NavalForces::shoot(NavalForces& enemy)
{
	CoordinatePair wound = findWoundedShip();
	if (wound._x != -1)
	{
		CoordinatePair secondWound(-1, -1);
		if (wound._y + 1 < _height && checkToShoot(CoordinatePair(wound._x, wound._y + 1)))
		{
			secondWound._x = wound._x;
			secondWound._y = wound._y + 1;
		}
		else if (wound._y - 1 > 0 && checkToShoot(CoordinatePair(wound._x, wound._y - 1)))
		{
			secondWound._x = wound._x;
			secondWound._y = wound._y - 1;
		}
		else if (wound._x + 1 < _width - 1 && checkToShoot(CoordinatePair(wound._x + 1, wound._y)))
		{
			secondWound._x = wound._x + 1;
			secondWound._y = wound._y;
		}
		else if (wound._x - 1 > 0 && checkToShoot(CoordinatePair(wound._x - 1, wound._y)))
		{
			secondWound._x = wound._x - 1;
			secondWound._y = wound._y;
		}

		_enemyField.at(secondWound._y).at(secondWound._x) = enemy.receiveMissile(secondWound);
		
		if (_enemyField.at(secondWound._y).at(secondWound._x) != _sea)
		{
			if (_enemyField.at(secondWound._y).at(secondWound._x) == _wreckedShip)
			{
				_enemyField.at(wound._y).at(wound._x) = _wreckedShip;
				fillCellsAroundShip(_enemyField, wound, secondWound);
				_enemyFlotilla.push_back(Ship(SHIPTYPE::DOUBLE_DECK, wound, secondWound, true));
			}
			printDeck();

			if (!_gameOver)
			{
				mtx1.lock();
				enemy.checkBerserkMode(*this);
				mtx1.unlock();
			}
			return true;
		}
		else
		{
			printDeck();
		}
	}
	else
	{
		std::uniform_int_distribution<int32_t> distCoord(1, 5);

		while (!_gameOver)
		{
			CoordinatePair cp(distCoord(mt), distCoord(mt));
			if (checkToShoot(cp))
			{
				_enemyField.at(cp._y).at(cp._x) = enemy.receiveMissile(cp);
				if (_enemyField.at(cp._y).at(cp._x) != _sea)
				{
					if (_enemyField.at(cp._y).at(cp._x) == _wreckedShip)
					{
						fillCellsAroundShip(_enemyField, cp);
						_enemyFlotilla.push_back(Ship(SHIPTYPE::SINGLE_DECK, cp, CoordinatePair(-1, -1), true));
					}
					printDeck();

					mtx2.lock();
					enemy.checkBerserkMode(*this);
					mtx2.unlock();

					return true;
				}
				else
				{
					printDeck();
				}
				break;
			}
		}
	}
	return false;
}

bool NavalForces::reshootRevivedShips(NavalForces& enemy, const CoordinatePair& cp1, const CoordinatePair& cp2)
{
	_enemyField.at(cp1._y).at(cp1._x) = enemy.receiveMissile(cp1);
	if (_enemyField.at(cp1._y).at(cp1._x) == '!')
	{
		std::find_if(_enemyFlotilla.begin(), _enemyFlotilla.end(), [&cp1, &cp2, this](Ship& ship)
			{
				if (ship._cp1 == cp1)
				{
					_enemyField.at(cp1._y).at(cp1._x) = _wreckedShip;
					ship._isDead = true;
					if (cp2._x != -1)
					{
						_enemyField.at(cp2._y).at(cp2._x) = _wreckedShip;
					}
					return true;
				}
				return false;
			}
		);
		return false;
	}

	printDeck(); 
	if (cp2._x != -1)
	{
		mtx.lock();
		_gameLogs.push_back(_teamName + " has shot a missile and hit enemy`s ship!\n");
		mtx.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		_enemyField.at(cp2._y).at(cp2._x) = enemy.receiveMissile(cp2);

		printDeck();
	}
	enemy.checkBerserkMode(*this);
	return true;
}

const CoordinatePair NavalForces::findWoundedShip() const
{
	int32_t iteratorIndex = 0;
	for (auto it : _enemyField)
	{
		int32_t wound = it.find(_woundedShip);
		if (wound != std::string::npos)
		{
			return CoordinatePair(wound, iteratorIndex);
		}
		else
			++iteratorIndex;
	}
	return CoordinatePair(-1, -1);
}