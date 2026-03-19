#include "Inventory.h"
#include "ItemType.h"
const std::map<int, int>& Inventory::getStock() const
{
	return stock;
}

int Inventory::getItemCount(int itemId) const
{
	int i;
	auto it = stock.find(itemId);
	if (it != stock.end()) {
		return it->second;
	}
	return 0;
}

bool Inventory::canStoreItem(int itemId, int count)
{
	if (count <= 0)
		return false;

	const Item* item = database.getItemById(itemId);
	if (item == nullptr)
		return false;

	auto it = stock.find(itemId);
	if (it == stock.end()) {
		if (count > item->getMaxStack())
			return false;
	}
	else {
		if ((it->second + count) > item->getMaxStack())
			return false;
	}
	return true;
}

StoreResult Inventory::addItem(int itemId, int count)
{
	if (count <= 0)
		return { StoreResultType::AtLeastOneRequired };

	const Item* item = database.getItemById(itemId);
	if (item == nullptr)
		return { StoreResultType::ItemNotFound };

	auto it = stock.find(itemId);
	if (it == stock.end()) {
		if (count > item->getMaxStack())
			return { StoreResultType::ItemNotFound };
		
		stock[itemId] = count;
	}
	else {
		if ((it->second + count) > item->getMaxStack())
			return { StoreResultType::CannotStackItems };
		else {
			it->second += count;
		}
	}
	return { StoreResultType::Success };
}

StoreResult Inventory::consumeItem(int itemId, int count)
{
	if (count <= 0)
		return { StoreResultType::AtLeastOneRequired};

	auto it = stock.find(itemId);
	if (it == stock.end()) {
		return { StoreResultType::ItemNotFound};
	}

	if (stock.at(itemId) < count) {
		return { StoreResultType::ItemNotEnough};
	}

	stock[itemId] -= count;
	if (stock[itemId] == 0) {
		stock.erase(itemId);
	}

	return { StoreResultType::Success};
}

StoreResult Inventory::hasEnoughItem(int itemId, int count) const
{
	if (count <= 0)
		return { StoreResultType::AtLeastOneRequired};

	auto it = stock.find(itemId);
	if (it == stock.end()){
		return { StoreResultType::ItemNotEnough };
	}
	if (it->second >= count)
		return { StoreResultType::Success };
	return { StoreResultType::ItemNotEnough};//it->second >= count;
}

const std::map<int, int> Inventory::searchItemsByName(std::string name) const
{
	std::map<int, int> result;
	for (auto& [k, v] : stock) {
		const Item* item = database.getItemById(k);
		if (item->getName().find(name) != std::string::npos) {
			result.emplace(k, v);
		}
	}
	return result;
}

const std::map<int, int> Inventory::searchItemsByType(ItemType type) const
{
	std::map<int, int> result;
	for (auto& [k, v] : stock) {
		const Item* item = database.getItemById(k);
		if (item->getItemType() == type) {
			result.emplace(k, v);
		}
	}
	return result;
}

