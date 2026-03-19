#include "ItemDatabase.h"

void ItemDatabase::initializeItemDatabase()
{
	//초기 아이템 추가 예시
	//itemDatabase.insert({ 100, Item{100, "초급 회복 물약","기초적인 회복 물약."} });
}

const std::map<int, Item>& ItemDatabase::getAllItems() const
{
	return itemDatabase;
}

QueryResult ItemDatabase::tryAddCustomItem(const std::string& name, const std::string& desc, ItemType type) {
	//아이템 존재하는지 먼저 체크
	for (const auto& [k, v] : itemDatabase) {
		if (v.getName() == name) {
			return { QueryResultType::AlreadyExist , k };
		}
	}

	int startIdx, endIdx;
	switch (type) {
	case(ItemType::Material):
		startIdx = 0, endIdx = 100;
		break;
	case(ItemType::Potion):
		startIdx = 100, endIdx = 200;
		break;
	default:
		return { QueryResultType::InvalidItemType, -1 };
	}

	//신규 아이템 추가
	for(int idx = startIdx ; idx < endIdx ; idx++){
		//아이템 ID가 사용중이지 않으면 해당 아이디 사용.
		if (itemDatabase.contains(idx) == false) {
			itemDatabase.emplace(idx, Item{ idx, name, desc, type });
			return { QueryResultType::Success, idx };
		}
	}

	return { QueryResultType::NoAvailableId , -1 };
}

bool ItemDatabase::isItemExistById(int id) const
{
	return itemDatabase.contains(id);
}

const Item* ItemDatabase::getItemById(int id) const
{
	auto it = itemDatabase.find(id);
	if (it != itemDatabase.end()) {
		return &it->second;
	}
	return nullptr;
}

ItemType ItemDatabase::getItemTypeById(int id) const
{
	auto it = itemDatabase.find(id);
	if (it != itemDatabase.end()) {
		return it->second.getItemType();
	}
	else
		return ItemType::None;
}

QueryResult ItemDatabase::getItemIdByName(const std::string& name) const
{
	for (const auto& [k, v] : itemDatabase) {
		if (v.getName() == name) {
			return { QueryResultType::Success , k };
		}
	}
	return { QueryResultType::ItemNotFound , -1 };
}