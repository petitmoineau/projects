#include "Item.h"
#include <string>

Item::Item(const std::string& itemName, const int32_t& level)
	: _itemName(itemName), _itemID(++_freeID), _level(level)
{
	if (_itemName == "dubinka")
	{
		_symbol = '!';
		_ability = std::make_unique<Parameter>("damage", level);
		_strength = std::make_unique<Parameter>("strength", level);
	}
	else if (_itemName == "shit-kartonka")
	{
		_symbol = '0';
		_ability = std::make_unique<Parameter>("protection", level);
		_strength = std::make_unique<Parameter>("strength", level);
	}
	else if (_itemName == "butylka-piva")
	{
		_symbol = '8';
		_ability = std::make_unique<Parameter>("health");
	}
}

Item::Item(const Item& anotherItem)
{
	_itemID = anotherItem.getID();
	_level = anotherItem.getLevel();
	_symbol = anotherItem.getItemSymbol();
	_ability = std::make_unique<Parameter>(anotherItem.getAbility().getParamName(), anotherItem.getAbility().getParamLevel());
	_itemName = anotherItem.getName();
}

Item& Item::operator=(const Item& item)
{
	_itemID = item.getID();
	_level = item.getLevel();
	_symbol = item.getItemSymbol();
	_ability = std::make_unique<Parameter>(item.getAbility().getParamName(), item.getAbility().getParamLevel());
	_itemName = item.getName();
	return *this;
}

const std::string& Item::getName() const { return this->_itemName; }

const int32_t& Item::getLevel() const { return this->_level; }

const char& Item::getItemSymbol() const
{
	return this->_symbol;
}

const Parameter& Item::getAbility() const
{
	return *_ability.get();
}

const Parameter& Item::getStrength() const
{
	return *_strength.get();
}

void Item::reduceStrength()
{
	_strength->reduceParam(1);
}

const int32_t& Item::getID() const
{
	return this->_itemID;
}

bool operator==(const Item& item1, const Item& item2)
{
	return !item1.getName().compare(item2.getName());
}