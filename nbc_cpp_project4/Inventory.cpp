#include "Inventory.h"

const std::map<int, int>& Inventory::getStock() const
{
	return stock;
}

int Inventory::getItemCount(int itemId) const
{
	auto it = stock.find(itemId);
	if (it != stock.end()) {
		return it->second;
	}
	return 0;
}

bool Inventory::addItem(int itemId, int count)
{
	if (count <= 0)
		return false;

	auto it = stock.find(itemId);
	if (it == stock.end()) {
		if (count > maxStack)
			return false;
		
		stock[itemId] = count;
	}
	else {
		if ((it->second + count) > maxStack)
			return false;
		else {
			it->second += count;
		}
	}
	return true;
}

bool Inventory::consumeItem(int itemId, int count)
{
	if (count <= 0)
		return false;

	if (!stock.contains(itemId)) {
		return false;
	}

	if (stock.at(itemId) < count) {
		return false;
	}

	stock[itemId] -= count;

	if (stock[itemId] == 0) {
		stock.erase(itemId);
	}

	return true;
}

bool Inventory::hasEnoughItem(int itemId, int count) const
{
	if (count <= 0)
		return false;

	auto it = stock.find(itemId);
	if (it == stock.end()){
		return false;
	}

	return it->second >= count;
}