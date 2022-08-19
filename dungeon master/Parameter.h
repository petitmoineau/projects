#pragma once
#include <iostream>

class Parameter
{
public:

	Parameter(const std::string& paramName, const int32_t& level = 0);

	const int32_t& getParamCapacity() const;

	int32_t& reduceParam(const int32_t& attack);

	void boostParam(const Parameter& parameter);

	const int32_t& getboostBar() const;

	const int32_t& getParamBar() const;

	const std::string& getParamName() const;

	const int32_t& getParamLevel() const;

	void zeroParam();

	void zeroBoost();

	void restoreParam();

	void levelUp();

private:
	std::string _paramName;
	int32_t _paramBar;
	int32_t _boostBar;
	int32_t _paramCapacity;
	int32_t _level;
};