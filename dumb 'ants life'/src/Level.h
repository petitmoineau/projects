#pragma once
#include "Ant.h"
#include <string>
#include <vector>
#include <cstdint>


class Level
{
public:

	Level(const std::string& filepath);

	bool is_move_available(const Ant& a, const Direction& dir) const;
	const std::vector<Position>& get_ants_spawn_locations() const;
	const std::vector<Position>& get_walls() const;
	const std::vector<Position>& get_anthill_positions() const;

	const std::vector<Position> get_available_food_spawns() const;

	void load(const std::string& filepath);
private:
	std::vector<Position> m_anthill_positions;
	std::vector<Position> m_spawn_locations;
	std::vector<Position> m_walls;
};