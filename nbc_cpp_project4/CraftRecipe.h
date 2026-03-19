//CraftRecipe.h
#pragma once
#include <map>

class CraftRecipe {
	int resultItemId;
	std::map<int, int> ingredients;
public:
	CraftRecipe() : resultItemId(-1), ingredients({}) {}
	CraftRecipe(int result,const std::map<int, int>& ids) :
		resultItemId(result), ingredients(ids) {}

	bool matchesIngredient (const std::map<int, int>& comp) const {
		return ingredients == comp;
	}

	int getResultItemId() const { return resultItemId; }
	const std::map<int, int>& getIngredients() const { return ingredients; }
};