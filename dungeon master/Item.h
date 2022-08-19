#pragma once
#include <iostream>
#include "Parameter.h"

class Item
{
public:

	Item(const std::string& itemName, const int32_t& level = 1);

	Item(const Item& anotherItem);

	~Item() { return; }

	Item& operator=(const Item& item);

	const std::string& getName() const;

	const int32_t& getLevel() const;

	const char& getItemSymbol() const;

	const Parameter& getAbility() const;

	const Parameter& getStrength() const;

	void reduceStrength();

	const int32_t& getID() const;

protected:
	static int32_t _freeID;

private:

	int32_t _itemID;
	int32_t _level;
	char _symbol;
	std::string _itemName;
	std::unique_ptr<Parameter> _ability;
	std::unique_ptr<Parameter> _strength;
};

bool operator==(const Item& item1, const Item& item2);