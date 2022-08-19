#include "Inventory.h"

Inventory::Inventory()
	: _capacity(10)
{}

bool Inventory::addItem(Item item)
{
	if (_storage.size() < _capacity)
	{
		_storage.push_back(item);
		return true;
	}
	return false;
}

std::unique_ptr<Item> Inventory::findItem(const std::string& itemName)
{
	int32_t maxItemLevelInInv(-1);
	for (auto it : _storage)
	{
		if (it.getName() == itemName && maxItemLevelInInv < it.getLevel())
		{
			maxItemLevelInInv = it.getLevel();
		}
	}
	if (maxItemLevelInInv == -1) 
		return std::make_unique<Item>(nullptr);
	return std::make_unique<Item>(itemName, maxItemLevelInInv);
}


bool Inventory::dropOrUseItem(const std::string& itemName, const int32_t& indexOfInvCell, const int32_t& level)
{
	if (itemName == "" && indexOfInvCell != 0)
	{
		if (_storage.size() >= indexOfInvCell)
		{
			_storage.erase(_storage.begin() + indexOfInvCell - 1);
			return true;
		}
	}


	auto it = std::find_if(_storage.begin(), _storage.end(), 
		[&itemName, &level](const Item& itemToCompare) 
		{ 
			if (level != 0)
			{
				return (itemToCompare.getName() == itemName && itemToCompare.getLevel() == level);
			}
			return itemToCompare.getName() == itemName;
		});

	if (it != _storage.end())
	{
		_storage.erase(it);
		return true;
	}
	return false;
}

const void Inventory::printInventory() const
{
	std::cout << "\nInventory:\n";
	int32_t indexOfInvCell(1);
	for (auto it : _storage)
	{
		std::cout << indexOfInvCell << " " << it.getName() << "\n";
		++indexOfInvCell;
	}
}