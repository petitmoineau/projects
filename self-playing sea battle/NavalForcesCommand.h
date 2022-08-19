#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include "Ship.h"

extern bool _gameOver;

class NavalForces
{
public:
	static std::mt19937 mt;
	static std::vector<std::string> _gameLogs;

	NavalForces(const std::string& teamName);

	void informAboutRevival(const std::string& teamName);
	void printDeck();
	static void printLogs();
	void yourTurn(NavalForces& enemy);
	char receiveMissile(const CoordinatePair& cp);
	const std::string& getTeamName() const;

private:
	void readDatabase();
	void initShipPlacement();
	void placeDoubleDeckOnField(const CoordinatePair& cp1, const CoordinatePair& cp2);
	const CoordinatePair findWoundedShip() const;	

	bool checkToShoot(const CoordinatePair& cp) const;

	std::vector<Ship>::iterator searchThroughFlotilla(bool reviveAction, const CoordinatePair& cp = CoordinatePair(-1, -1));
	void checkIfGameOver() const;
	void fillCellsAroundShip(std::vector<std::string>& field, const CoordinatePair& cp1, const CoordinatePair& cp2 = CoordinatePair(-1, -1));
	void checkBerserkMode(NavalForces& enemy);
	bool shoot(NavalForces& enemy);
	bool reshootRevivedShips(NavalForces& enemy, const CoordinatePair& cp1, const CoordinatePair& cp2 = CoordinatePair(-1, -1));
	const CoordinatePair checkToPlace(const CoordinatePair& cp) const;
	
	std::vector<std::string> _field;
	std::vector<std::string> _enemyField;
	
	std::vector<Ship> _flotilla;
	std::vector<Ship> _enemyFlotilla;

	bool _enemyShipRevived = false;

	std::string _teamName;
	const char _sea = '~';
	const char _ship = '@';
	const char _woundedShip = 'a';
	const char _wreckedShip = '^';
	const char _aroundShip = 'x';
	const char _empty = '_';

	static bool _berserkMode;
	std::ifstream databaseFile;
	int32_t _width;
	int32_t _height;

	std::mutex mtx;
	std::mutex mtx1;
	std::mutex mtx2;
	std::mutex mtx3;
	std::mutex reviveMutex;
};