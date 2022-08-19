#include "Engine.h"
#include <algorithm>
#ifdef _WIN32
#include <Windows.h>
#define console_clear system("CLS")
#else
#define console_clear system("clear")
#endif

namespace
{
	std::random_device rd;
	std::mt19937 random_engine(rd());

	void insert_symbol(const std::vector<Position>& insert_from, std::vector<std::string>& insert_to, const char& symbol)
	{
		std::for_each(insert_from.begin(), insert_from.end(), [&insert_to, &symbol](const Position& position)
			{
				insert_to.at(position.y).at(position.x) = symbol;
			}
		);
	}

	void ant_insert_delete(const std::vector<Ant>& insert_from, std::vector<std::string>& insert_to, const char& symbol, const Level& level)
	{
		if (symbol == char(GameEngine::m_game_symbols::ant))
		{
			std::for_each(insert_from.begin(), insert_from.end(), [&insert_to, &symbol](const Ant& ant)
				{
					insert_to.at(ant.pos().y).at(ant.pos().x) = symbol;
				}
			);
		}
		else
		{
			auto spawn_locations = level.get_ants_spawn_locations();
			std::for_each(insert_from.begin(), insert_from.end(), [&insert_to, &spawn_locations](Ant ant)
				{
					auto spawn_place_it = std::find_if(spawn_locations.begin(), spawn_locations.end(), [ant](Position spawn_pos) { return ant.pos() == spawn_pos; });
					if (spawn_place_it != spawn_locations.end())
						insert_to.at(ant.pos().y).at(ant.pos().x) = char(GameEngine::m_game_symbols::spawn_place);
					else
						insert_to.at(ant.pos().y).at(ant.pos().x) = char(GameEngine::m_game_symbols::empty);
				}
			);
		}
	}

	uint32_t oldest_ant_in_colony(const std::vector<Ant>& ant_colony)
	{
		uint32_t oldest = 0;
		std::for_each(ant_colony.begin(), ant_colony.end(), [&oldest](Ant ant) { oldest = max(oldest, ant.years_left()); });
		return oldest;
	}
}

std::uniform_int_distribution<uint32_t> GameEngine::random_foods_amount{ 5,10};
std::uniform_int_distribution<uint32_t> GameEngine::random_nutrition{ 2,4 };
std::uniform_int_distribution<uint32_t> GameEngine::random_moves_till_food_spawn{ 1,5 };
std::uniform_int_distribution<uint32_t> GameEngine::random_ant_direction{ 1,4 };

std::vector<std::string> GameEngine::m_level_rendering(50, std::string(190, ' '));


GameEngine::result_t GameEngine::game_result() const
{
	if (m_ants.size() >= num_ants_to_win)
		return result_t::ants_won;
	if (m_ants.size() <= 0)
		return result_t::ants_died_out;
	return result_t::game_still_on;
}

void GameEngine::on_year_pass()
{
	spawn_food_tick();

	ants_move_tick();
	ants_age_tick();
	ants_population_tick();
}

Position GameEngine::random_spawn_pos()
{
	const auto& spawn_pos = m_level.get_ants_spawn_locations();
	std::uniform_int_distribution random_spawn_index{ static_cast<size_t>(0), spawn_pos.size()-1 };
	return spawn_pos[random_spawn_index(random_engine)];
}

void GameEngine::spawn_ants()
{
	for (int i = 0; i < initial_ant_num; i++)
		m_ants.push_back(Ant{ random_spawn_pos(), Ant::random_age(random_engine)});
}

void GameEngine::spawn_food_tick()
{
	if (m_moves_till_food_spawn-- > 0)
		return;

	auto potential_food_spawns = m_level.get_available_food_spawns();
	std::vector<Position> trimmed;
	std::sample(potential_food_spawns.begin(), potential_food_spawns.end(), std::back_inserter(trimmed), spawn_food_amount, random_engine);
	for (auto pos : trimmed)
	{
		if (std::find(m_foods.begin(), m_foods.end(), pos) == m_foods.end())
			if (std::find_if(m_ants.begin(), m_ants.end(), [pos](Ant ant) { return ant.pos() == pos;}) == m_ants.end())
				m_foods.push_back(pos);
	}
	m_moves_till_food_spawn = random_moves_till_food_spawn(random_engine);
}

Direction GameEngine::choose_direction(Ant ant)
{
	std::vector<Direction> possible_dirs;
	if (m_level.is_move_available(ant, Direction::up()))
		possible_dirs.push_back( Direction::up());
	if (m_level.is_move_available(ant, Direction::down()))
		possible_dirs.push_back(Direction::down());
	if (m_level.is_move_available(ant, Direction::left()))
		possible_dirs.push_back(Direction::left());
	if (m_level.is_move_available(ant, Direction::right()))
		possible_dirs.push_back(Direction::right());
	std::shuffle(possible_dirs.begin(), possible_dirs.end(), random_engine);
	return possible_dirs[0];
}

void GameEngine::ants_move_tick()
{
	for (auto& ant : m_ants)
	{
		Direction chosen = choose_direction(ant);
		ant.move(chosen);

		auto iter = std::find(m_foods.begin(), m_foods.end(), ant.pos());
		if (iter != m_foods.end())
		{
			m_foods.erase(iter);
			m_total_ant_food += random_nutrition(random_engine);
		}
	}
}

void GameEngine::ants_age_tick()
{
	std::for_each(m_ants.begin(), m_ants.end(), [](Ant& ant) { ant.on_year_lived(); });
}

void GameEngine::ants_population_tick()
{
	for (int i = m_ants.size() - 1; i >= 0; i--)
	{
		if (!m_ants.at(i).still_alive())
			m_ants.erase(m_ants.begin() + i);
	}
	while (m_total_ant_food > food_to_spawn_ant)
	{
		m_ants.push_back(Ant( random_spawn_pos(), Ant::random_age(random_engine)));
		m_total_ant_food -= food_to_spawn_ant;
	}
}

GameEngine::GameEngine(const std::string& map_filepath)
	: m_level(Level(map_filepath))
{
	load_level();
	spawn_ants();
}

void GameEngine::load_level()
{
	insert_symbol(m_level.get_walls(), m_level_rendering, char(m_game_symbols::wall));
	insert_symbol(m_level.get_anthill_positions(), m_level_rendering, char(m_game_symbols::anthill));
	insert_symbol(m_level.get_ants_spawn_locations(), m_level_rendering, char(m_game_symbols::spawn_place));
}

void GameEngine::render() 
{
	ant_insert_delete(m_ants, m_level_rendering, char(m_game_symbols::ant), m_level);
	insert_symbol(m_foods, m_level_rendering, char(m_game_symbols::food));
	console_clear;
	std::for_each(m_level_rendering.begin(), m_level_rendering.end(), [](std::string_view line)
		{
			std::cout << line << "\n";
		}
	);

	std::cout << "There are " << m_ants.size() << " stupid ants on the field\n";
	std::cout << "Food supply: " << m_total_ant_food << " foodies\n";
	std::cout << "The colony will die in " << oldest_ant_in_colony(m_ants) << " years\n";

	insert_symbol(m_foods, m_level_rendering, char(m_game_symbols::empty));
	ant_insert_delete(m_ants, m_level_rendering, char(m_game_symbols::empty), m_level);
}
