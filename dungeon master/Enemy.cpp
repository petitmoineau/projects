#include "Enemy.h"

Enemy::Enemy(int32_t level)
	: _level(level)
{
	_health = std::make_unique<Parameter>("health", _level);
	_attack = std::make_unique<Parameter>("attack", _level);
	_protection = std::make_unique<Parameter>("protection", _level);

	_level == 1 ? _symbol = 'e' : _level == 2 ? _symbol = 'Y' : _symbol = '$';
	_currentSword = std::make_unique<Item>("dubinka", _level);
	_attack->boostParam(_currentSword->getAbility());

	if (_level > 2)
	{
		_currentShield = std::make_unique<Item>("shit-kartonka", _level);
		_protection->boostParam(_currentShield->getAbility());
	}
}

void Enemy::attackPlayer(Player& player)
{
	static std::mt19937 mt(std::random_device{}());
	std::uniform_real_distribution<double> dist(0.5, 1.0);

	int32_t _currentAttack = max(_attack->getParamCapacity() + dist(mt) * _attack->getboostBar() - player.getProtection().getParamBar() - dist(mt) * player.getProtection().getboostBar(), 1);
	if (_currentSword != NULL)
	{
		_currentSword->reduceStrength();
		if (_currentSword->getStrength().getParamBar() == 0)
		{
			_currentSword.reset(NULL);
		}
	}

	std::cout << "\nEnemy attacks.";
	if (_currentSword != NULL) std::cout << "\nEnemy`s dubinka strength: " << _currentSword->getStrength().getParamBar();

	player.beingAttacked(*this, _currentAttack);
}

const char& Enemy::getEnemySymbol() const
{
	return this->_symbol;
}

void Enemy::beingAttacked(Player& player, const int32_t& attack)
{
	int32_t _protectionAfterAttack = _protection->reduceParam(attack);
	if (_currentShield != NULL)
	{
		_currentShield->reduceStrength();
		if (_currentShield->getStrength().getParamBar() == 0)
		{
			_currentShield.reset(NULL);
		}
	}
	
	if (_protectionAfterAttack < 0)
	{
		_health->reduceParam(std::abs(_protectionAfterAttack));
		_protection->zeroParam();
	}

	if (_health->getParamBar() <= 0)
	{
		return;
	}

	std::cout << "\nEnemy is attacked by player with " << attack << " points";
	std::cout << "\nEnemy`s health: " << _health->getParamBar();
	if (_currentShield != NULL) std::cout << "\nEnemy`s shit-kartonka strength: " << _currentShield->getStrength().getParamBar();

	attackPlayer(player);
}

const Parameter& Enemy::getProtection() const
{
	return *_protection.get();
}

const Parameter& Enemy::getHealth() const
{
	return *_health.get();
}