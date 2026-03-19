//Item.h
#pragma once
#include <string>
#include "ItemType.h"

class Item {
	int id;
	std::string name;
	std::string description;
	ItemType itemType;
	int maxStack;
public:
	Item() :id(-1), name("NO DATA"), description("NO DATA"), itemType(ItemType::None), maxStack(-1){}
	Item(int _id, const std::string& _name, const std::string& _desc, ItemType type, int _maxStack) :
		id(_id), name(_name), description(_desc), itemType(type), maxStack(_maxStack) {}

	int getId() const { return id; }
	const std::string& getName() const { return name; }
	const std::string& getDescription() const { return description; }
	const int getMaxStack() const { return maxStack; }
	ItemType getItemType() const { return itemType; }
};