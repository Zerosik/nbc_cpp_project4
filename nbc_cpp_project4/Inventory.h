//Inventory.h
#pragma once
#include <map>

class Inventory {
private:
	const int maxStack = 10;
	std::map<int, int> stock;
public:
	Inventory(){}
	int getItemCount(int itemId) const;
	bool addItem(int itemId, int count);
	bool consumeItem(int itemID, int count);
	bool hasEnoughItem(int itemID, int count) const;
	const std::map<int, int>& getStock() const;
};