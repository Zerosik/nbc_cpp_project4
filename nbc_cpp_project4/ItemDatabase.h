//ItemDatabase.h
#pragma once
#include <map>
#include "Item.h"

enum class QueryResultType {
	Success,//성공
	InvalidItemType,//아이템 정보 없음
	AlreadyExist,
	ItemNotFound,
	NoAvailableId
};

struct QueryResult {
	QueryResultType resultType;
	int resultItemId;
};

class ItemDatabase
{
private:
	std::map<int, Item> itemDatabase;
	void initializeItemDatabase();
	ItemDatabase() { initializeItemDatabase(); }
	int getItemDefaultStack(ItemType type);
public:
	ItemDatabase(const ItemDatabase&) = delete;
	ItemDatabase& operator=(const ItemDatabase&) = delete;
	static ItemDatabase& getInstance() {
		static ItemDatabase instance;
		return instance;
	}
	~ItemDatabase() {}
	const std::map<int, Item>& getAllItems() const;
	QueryResult tryAddCustomItem(const std::string& name, const std::string& desc, ItemType type);
	QueryResult tryAddCustomItem(const std::string& name, const std::string& desc, ItemType type, int maxStack);
	bool isItemExistById(int id) const;
	const Item* getItemById(int id) const;
	ItemType getItemTypeById(int id) const;
	QueryResult getItemIdByName(const std::string& name) const;
	void deleteItem(int id);
};
