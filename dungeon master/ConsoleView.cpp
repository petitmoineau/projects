#include "ConsoleView.h"

ConsoleView::ConsoleView()
{
	_player = std::make_unique<Player>();
	_field = std::make_unique<Field>(*_player.get());

	_field->insertInField(_player->playerPosX(), _player->playerPosY(), *_player.get());

	_enemyCounter = 0;
}

void ConsoleView::update()
{
	std::chrono::duration<double, std::milli> elapsed = std::chrono::high_resolution_clock::now() - _time;
	if ((int32_t)elapsed.count() % 60 == 0)
	{
		static std::mt19937 mt(std::random_device{}());
		std::uniform_int_distribution<int32_t> dist(1, 3);
		int32_t x = dist(mt);
		switch (x)
		{
		case 1:
			_field->addItemOnField(Item("shit-kartonka", _player->level()));
			break;
		case 2:
			_field->addItemOnField(Item("dubinka", _player->level()));
			break;
		case 3:
			_field->addItemOnField(Item("butylka-piva"));
			break;
		}
	}

	_field->addEnemyOnField(Enemy(_player->level()));

	_field->draw();

	_player->printStats();
}

void ConsoleView::showItems()
{
	(_player->showInventory()).printInventory();
}

const Player& ConsoleView::getPlayer() const
{
	return *_player.get();
}

Player& ConsoleView::getPlayer()
{
	return *_player.get();
}

void ConsoleView::movePlayer(const char& move)
{
	switch (move)
	{
	case 'w':
		_player->moveUp();
		break;
	case 'a':
		_player->moveLeft();
		break;
	case 's':
		_player->moveDown();
		break;
	case 'd':
		_player->moveRight();
		break;
	}

	_field->movePlayer(*_player.get());
}