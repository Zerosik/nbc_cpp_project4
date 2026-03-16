#include "Inventory.h"



const std::map<int, int>& Inventory::getStock() const
{
	return stock;
}

bool Inventory::addItem(int itemId, int count)
{
	if (stock[itemId] + count > maxStack) { return false; }
	else {
		stock[itemId] += count;
	}
	return true;
}

bool Inventory::consumeItem(int itemID, int count)
{
	if (stock.contains(itemID)) {
		if (stock[itemID] >= count) {
			stock[itemID] -= count;
		}
		//재고가 0이면 제거
		if (stock[itemID] == 0) { stock.erase(itemID); }
		return true;
	}
	else { return false; }
}

bool Inventory::enoughItem(int itemID, int count)
{
	if (stock.contains(itemID)) {
		return stock[itemID] >= count;
	}
	else { return false; }
}