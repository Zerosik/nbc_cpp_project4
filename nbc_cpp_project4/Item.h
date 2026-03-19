//Item.h
#pragma once
#include <string>
enum class ItemType {
	Material,
	Potion,
	Misc,
	None
};
class Item {
	int id;
	std::string name;
	std::string description;
	ItemType itemType; 
public:
	Item() :id(-1), name("NO DATA"), description("NO DATA"), itemType(ItemType::None) {}
	Item(int _id, const std::string& _name, const std::string& _desc, ItemType type) :
		id(_id), name(_name), description(_desc), itemType(type) {}

	int getId() const { return id; }
	const std::string& getName() const { return name; }
	const std::string& getDescription() const { return description; }
	ItemType getItemType() const { return itemType; }
};