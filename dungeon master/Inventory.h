#pragma once
#include <algorithm>
#include <iostream>
#include "Item.h"
#include <vector>

class Inventory
{
public:
	Inventory();

	bool addItem(Item item);

	std::unique_ptr<Item> findItem(const std::string& itemName);

	bool dropOrUseItem(const std::string& itemName = "", const int32_t& indexOfInvCell = 0, const int32_t& level = 0);

	const void printInventory() const;

private:
	std::vector<Item> _storage;
	int32_t _capacity;
};
