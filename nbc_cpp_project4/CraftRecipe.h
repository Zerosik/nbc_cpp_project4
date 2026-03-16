//CraftRecipe.h
#pragma once
#include <map>

class CraftRecipe {
	//결과 id
	int resultItemId;
	//id, 수량
	std::map<int, int> ingredients;
public:
	CraftRecipe() : resultItemId(-1), ingredients({}) {}
	CraftRecipe(int _result, std::map<int, int> _ids) :resultItemId(_result), ingredients(_ids) {}

	bool compareIngredient (std::map<int, int> comp) const {
		return ingredients == comp;
	}

	const int getItemId() const { return resultItemId; }
	const std::map<int, int>& getIngredients() const { return ingredients; }
};