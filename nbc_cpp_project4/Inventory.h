//Inventory.h
#pragma once
#include <map>
#include "ItemDatabase.h"
class Inventory {
private:
	int maxStack = 999;
	std::map<int, int> stock;
public:
	Inventory(){}
	bool addItem(int itemId, int count);
	bool consumeItem(int itemID, int count);
	bool enoughItem(int itemID, int count);
	const std::map<int, int>& getStock() const;
};