#pragma once
#include <random>
#include <cstdint>

struct Position
{
	Position(int32_t _x, int32_t _y)
		:x(_x), y(_y)
	{}

	int32_t x;
	int32_t y;
	inline Position& operator+=(Position pos) { x += pos.x; y += pos.y; return *this; }
	auto operator<=>(const Position& pos)const& = default;
};

inline Position operator+(Position pos1, Position pos2) { return pos1 += pos2; }

struct Direction : public Position
{
	Direction(int32_t x, int32_t y) : Position(x, y) {}
	inline static Direction up() { return Direction{ 0, -1 }; };
	inline static Direction down() { return Direction{ 0, 1 }; };
	inline static Direction left() { return Direction{ -1, 0 }; };
	inline static Direction right() { return Direction{ 1, 0 }; };
};


class Ant
{
public:
	Ant(Position pos, uint32_t lifespan) :m_pos(pos), m_years_left_to_live(lifespan) {}

	inline void on_year_lived() { m_years_left_to_live--; }
	inline bool still_alive() const { return m_years_left_to_live > 0; }

	inline Position pos() const { return m_pos; }
	inline uint32_t years_left() const { return m_years_left_to_live; }
	inline void move(Direction dir) { m_pos += dir; }

	static std::uniform_int_distribution<uint32_t> random_age;

private:
	Position m_pos;
	uint32_t m_years_left_to_live;
};