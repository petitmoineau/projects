#pragma once

#include "Level.h"
#include "Ant.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

class GameEngine
{
public:
	GameEngine(const std::string& map_filepath);

	enum class result_t :int8_t { ants_died_out = -1, game_still_on = 0, ants_won = 1 };
	enum class m_game_symbols : char { wall = 'O', spawn_place = 'S', anthill = 'M', ant = '8', food = 'f', empty = ' ' };
	result_t game_result() const;

	void on_year_pass();

	void load_level();
	void render();

private:
	const Level m_level;
	std::vector<Ant> m_ants;
	static std::uniform_int_distribution<uint32_t> random_ant_direction;
	static std::uniform_int_distribution<uint32_t> random_foods_amount;
	static std::uniform_int_distribution<uint32_t> random_nutrition;
	std::vector<Position> m_foods; 
	
	static std::vector<std::string> m_level_rendering;

	uint32_t m_total_ant_food = 0;
	uint32_t m_moves_till_food_spawn = 0;

	static constexpr uint32_t initial_ant_num = 10;
	static constexpr uint32_t num_ants_to_win = 50;
	static constexpr uint32_t spawn_food_amount = 60;
	static constexpr uint32_t food_to_spawn_ant = 5;
	Position random_spawn_pos();
	void spawn_ants();

	static std::uniform_int_distribution<uint32_t> random_moves_till_food_spawn;
	void spawn_food_tick();

	Direction choose_direction(Ant ant);
	void ants_move_tick();
	void ants_age_tick();
	void ants_population_tick();
};
