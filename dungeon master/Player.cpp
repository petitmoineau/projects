#include "Player.h"

Player::Player(int32_t level)
	: _level(level), _playerSymbol('@')
{
	_health = std::make_unique<Parameter>("health", _level);
	_attack = std::make_unique<Parameter>("attack", _level);
	_protection = std::make_unique<Parameter>("protection", _level);
	_experience = 0;
}

void Player::setInitPosition(const int32_t& playerPosX, const int32_t& playerPosY)
{
	_playerPosX = playerPosX;
	_playerPosY = playerPosY;
}

void Player::moveUp()
{
	_playerDirection = DIRECTION::UP;
	--_playerPosY;
}
void Player::moveDown()
{
	_playerDirection = DIRECTION::DOWN;
	++_playerPosY;
}
void Player::moveLeft()
{
	_playerDirection = DIRECTION::LEFT;
	--_playerPosX;
}
void Player::moveRight()
{
	_playerDirection = DIRECTION::RIGHT;
	++_playerPosX;
}

const char& Player::playerSymbol() const
{
	return this->_playerSymbol;
}

const int32_t& Player::playerPosX() const
{
	return this->_playerPosX;
}

const int32_t& Player::playerPosY() const
{
	return this->_playerPosY;
}

const DIRECTION& Player::direction() const
{
	return this->_playerDirection;
}

const int32_t& Player::level() const
{
	return this->_level;
}

const int32_t& Player::experience() const
{
	return this->_experience;
}

void Player::levelUp()
{
	++_level;
	_health->levelUp();
	_protection->levelUp();
	_attack->levelUp();
}

bool Player::pickUpItem(std::unique_ptr<Item>& item)
{
	if (!_inventory.addItem(*item.get()))
	{
		return false;
	}
	else if (item->getName() == "shit-kartonka")
	{
		if (_currentShield == nullptr)
		{
			_currentShield = std::move(item);
		}
		else if (item->getAbility().getParamCapacity() > _currentShield->getAbility().getParamCapacity())
		{
			_currentShield.reset(item.release());
		}
		_protection->boostParam(_currentShield->getAbility());
	}
	else if (item->getName() == "dubinka")
	{
		if (_currentSword == nullptr)
		{
			_currentSword = std::move(item);
		}
		else if (item->getAbility().getParamCapacity() > _currentSword->getAbility().getParamCapacity())
		{
			_currentSword.reset(item.release());
		}
		_attack->boostParam(_currentSword->getAbility());
	}

	return true;
}

void Player::beingAttacked(Enemy& enemy, const int32_t& attack)
{
	int32_t _protectionAfterAttack = _protection->reduceParam(attack);
	if (_currentShield != NULL)
	{
		_currentShield->reduceStrength();
		if (_currentShield->getStrength().getParamBar() == 0)
		{
			_inventory.dropOrUseItem("shit-kartonka", _currentShield->getLevel());
			_currentShield.reset(_inventory.findItem("shit-kartonka").release());
		}
	}
	
	if (_protectionAfterAttack < 0)
	{
		_health->reduceParam(std::abs(_protectionAfterAttack));
		_protection->zeroParam();
	}
	if (_health->getParamBar() <= 0)
	{
		if (!usePotion())
		{
			return;
		}
		std::cout << "\nPotion used\n";
	}

	std::cout << "\nPlayer is attacked by enemy with " << attack << " points";
	std::cout << "\nPlayer`s health: " << _health->getParamBar();
	if (_currentShield != NULL) std::cout << "\nPlayer`s shit-kartonka strength: " << _currentShield->getStrength().getParamBar();

	attackEnemy(enemy);
}

bool Player::usePotion()
{
	if (_inventory.dropOrUseItem("butylka-piva"))
	{
		_health->restoreParam();
		return true;
	}
	return false;
}

void Player::attackEnemy(Enemy& enemy)
{
	static std::mt19937 mt(std::random_device{}());
	std::uniform_real_distribution<double> dist(0.5, 1.0);

	int32_t currentAttack = max(_attack->getParamCapacity() + dist(mt) * _attack->getboostBar() - enemy.getProtection().getParamBar() - dist(mt) * enemy.getProtection().getboostBar(), 1);
	if (_currentSword != NULL)
	{
		_currentSword->reduceStrength();
		if (_currentSword->getStrength().getParamBar() == 0)
		{
			_inventory.dropOrUseItem("dubinka", _currentSword->getLevel());
			_currentSword.reset(_inventory.findItem("dubinka").release());
		}
	}

	std::cout << "\nPlayer attacks.";
	if (_currentSword != NULL) std::cout << "\nPlayer`s dubinka strength: " << _currentSword->getStrength().getParamBar();

	enemy.beingAttacked(*this, currentAttack);
}

Inventory& Player::getInventory()
{
	return _inventory;
}

const Inventory& Player::showInventory() const
{
	return _inventory;
}

const Parameter& Player::getProtection() const
{
	return *_protection.get();
}

const Parameter& Player::getHealth() const
{
	return *_health.get();
}

void Player::addBittenEnemy(const int32_t& level)
{
	switch (level)
	{
	case 1:
		_enemiesBitten.insert(std::make_pair("bandyuk-shesterka", level));
		break;
	case 2:
		_enemiesBitten.insert(std::make_pair("mestnyi reshala", level));
		break;
	case 3:
		_enemiesBitten.insert(std::make_pair("glavar` OPG", level));
		break;
	}
}

void Player::addExperience()
{
	_experience += _level * 50;
	if (_experience % 200 == 0)
	{
		levelUp();
	}
}

void Player::printStats() const
{
	std::cout << "\nPlayer`s stats:\n";
	std::cout << "Health: " << _health->getParamBar() << "\n";
	std::cout << "Protection: " << _protection->getParamBar() + _protection->getboostBar() << "\n";
	std::cout << "Attack: " << _attack->getParamBar() + _attack->getboostBar() << "\n";
	std::cout << "Level: " << level() << "\n";
	std::cout << "Babki: " << experience() << "\n";
}

void Player::printBittenEnemies() const
{
	std::cout << "\nEnemies bitten by player:\n";
	std::for_each(_enemiesBitten.begin(),
		_enemiesBitten.end(),
		[](const std::pair<std::string, int32_t>& enemies) {
			std::cout << enemies.first << ", level: " << enemies.second << "\n";
		});
}
