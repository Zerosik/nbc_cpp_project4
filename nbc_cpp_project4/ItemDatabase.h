//ItemDatabase.h
#pragma once
#include <map>
#include "Item.h"

class ItemDatabase
{
private:
	std::map<int, Item> itemDatabase;
	static ItemDatabase* instance;
	void InitializeItemDatabase();
	ItemDatabase() { InitializeItemDatabase(); }
public:
	ItemDatabase(const ItemDatabase&) = delete;
	ItemDatabase& operator=(const ItemDatabase&) = delete;
	static ItemDatabase* getInstance() {
		if (instance == nullptr) {
			instance = new ItemDatabase();
		}
		return instance;
	}
	std::map<int, Item> getAllItemInfo();
	// 추가되거나 이미 존재하는 아이템의 id를 반환.
	int tryAddCustomItem(const std::string& name, const std::string& desc);
	const Item* getItemById(const int id);
	const bool isItemExistById(const int id);
	const int isItemExistByName(const std::string& name);
};


//Meyers' Singleton으로 변경할것.