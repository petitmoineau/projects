#include "Level.h"
#include <fstream>

namespace
{
    void set_positions(const char& symbol, const std::string& from, std::vector<Position>& to, const int32_t& line_index)
    {
        int32_t char_pos;
        char_pos = from.find(symbol, 0);
        while (char_pos != std::string::npos)
        {
            to.push_back(Position(char_pos, line_index));
            char_pos = from.find(symbol, char_pos + 1);
        }
    }

    bool check_pos(const std::vector<Position>& map_pos, const Position& ants_pos)
    {
        return std::all_of(map_pos.begin(), map_pos.end(), [&ants_pos](const Position& pos) { return pos != ants_pos; });
    }
}

Level::Level(const std::string& filepath)
{
	load(filepath);
}

bool Level::is_move_available(const Ant& a, const Direction& dir) const
{
    return check_pos(m_anthill_positions, a.pos() + dir) 
        && check_pos(m_walls, a.pos() + dir) 
        && check_pos(m_spawn_locations, a.pos() + dir);
}

const std::vector<Position>& Level::get_ants_spawn_locations() const
{
    return m_spawn_locations;
}

const std::vector<Position>& Level::get_walls() const
{
    return m_walls;
}

const std::vector<Position>& Level::get_anthill_positions() const
{
    return m_anthill_positions;
}

const std::vector<Position> Level::get_available_food_spawns() const
{
    std::vector<Position> food_spawns;
    for (int32_t i = 0; i < 50; i++)
    {
        for (int32_t j = 0; j < 190; j++)
        {
            if(check_pos(m_anthill_positions, Position(j, i))
                && check_pos(m_walls, Position(j, i))
                && check_pos(m_spawn_locations, Position(j, i)))
            {
                food_spawns.push_back(Position(j, i));
            }
        }
    }
    return food_spawns;
}

void Level::load(const std::string& filepath)
{
    std::fstream map_file(filepath, std::ios::in);
    std::string str;
    std::string out_str;
    for (int32_t i = 0; map_file >> str; i++)
    {
        out_str.clear();
        for (int32_t j = 0; j < str.size(); j++)
        {
            if (str[j] != ',')
            {
                if (str[j + 1] != ',')
                {
                    
                }
                out_str.push_back(str[j++]);
            }
            else
            {
                out_str.push_back(' ');
            }
        }
        set_positions('M', out_str, m_anthill_positions, i);
        set_positions('O', out_str, m_walls, i);
        set_positions('S', out_str, m_spawn_locations, i);
    }
    map_file.close();
}
