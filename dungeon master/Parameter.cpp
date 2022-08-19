#include "Parameter.h"

Parameter::Parameter(const std::string& paramName, const int32_t& level)
	: _paramName(paramName), _level(level)
{
	_paramCapacity = _paramBar = 5 * _level;
	_boostBar = 0;
}

const int32_t& Parameter::getParamCapacity() const
{
	return this->_paramCapacity;
}

int32_t& Parameter::reduceParam(const int32_t& attack)
{
	int32_t tempBoost = _boostBar;
	if (_boostBar > 0)
	{
		if ((_boostBar -= attack) < 0)
		{
			_paramBar += _boostBar;
			_boostBar = tempBoost;
		}
	}
	else
	{
		_paramBar -= attack;
	}
	return _paramBar;
}

void Parameter::boostParam(const Parameter& parameter)
{
	_boostBar = parameter.getParamBar();
}

const int32_t& Parameter::getboostBar() const
{
	return this->_boostBar;
}

const int32_t& Parameter::getParamBar() const
{
	return this->_paramBar;
}

const std::string& Parameter::getParamName() const
{
	return this->_paramName;
}

const int32_t& Parameter::getParamLevel() const
{
	return this->_level;
}

void Parameter::zeroParam()
{
	_paramBar = 0;
}

void Parameter::zeroBoost()
{
	_boostBar = 0;
}

void Parameter::restoreParam()
{
	_paramBar = _paramCapacity;
}

void Parameter::levelUp()
{
	++_level;
	_paramCapacity = _paramBar = 5 * _level;
}
