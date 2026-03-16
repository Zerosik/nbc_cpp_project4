//Item.h
#pragma once
#include <string>
class Item {
	int id;
	std::string name;
	std::string description;
public:
	Item() :id(-1), name("NO DATA"), description("NO DATA") {}
	Item(int _id, const std::string& _name, const std::string& _desc) :
		id(_id), name(_name), description(_desc) {}
	const int getId() const { return id; }
	const std::string& getName() const { return name; };
	const std::string& getDescription() const { return description; };
};