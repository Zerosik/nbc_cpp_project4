//Inventory.h
#pragma once
#include <map>
#include "ItemType.h"
#include "ItemDatabase.h"

enum class StoreResultType {
	Success,//성공
	CannotStackItems,
	InventoryIsFull,
	AtLeastOneRequired,
	ItemNotFound,
	ItemNotEnough
};

struct StoreResult {
	StoreResultType resultType;
};

class Inventory {
private:
	ItemDatabase& database;
	std::map<int, int> stock;
public:
	Inventory() : 
		database(ItemDatabase::getInstance()){}
	int getItemCount(int itemId) const;
	bool canStoreItem(int itemId, int count);
	StoreResult addItem(int itemId, int count);
	StoreResult consumeItem(int itemID, int count);
	StoreResult hasEnoughItem(int itemID, int count) const;
	const std::map<int, int> searchItemsByName(std::string name) const;
	const std::map<int, int> searchItemsByType(ItemType type) const;
	const std::map<int, int>& getStock() const;
};