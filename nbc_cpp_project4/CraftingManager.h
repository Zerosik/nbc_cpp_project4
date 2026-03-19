//CraftingManager.h
#pragma once
#include "ItemDatabase.h"
#include "CraftRecipe.h"
#include "Inventory.h"
#include <map>
#include <vector>

enum class CraftResultType {
	Success,//성공
	RecipeNotFound,//레시피없음
	RecipeAlreadyExist,
	NotEnoughIngredient,//재료부족
	InvalidItem,//아이템 정보 없음
	AlreadyReservedRecipe
};

struct CraftResult {
	CraftResultType resultType;
	int resultItemId;
};

class CraftingManager
{
private:
	std::vector<CraftRecipe> craftRecipes;
	ItemDatabase& itemDB;
	void InitializeRecipes();
public:
	CraftingManager():itemDB(ItemDatabase::getInstance()){
		InitializeRecipes();
	}
	CraftResult customCraft(const std::map<int, int>& items) const;
	CraftResult craft(int ItemID, Inventory& inventory);
	
	CraftResult addCraftRecipe(const std::map<int, int>& items, int ResultItemId);
	const CraftRecipe* findRecipeByResultItemId(const int id) const;
	//입력한 재료를 필요로 하는 아이템들 반환
	std::vector<int> findRecipesByIngredientId(int ingredientId) const;
	std::vector<int> findRecipesByInventory(const Inventory& inventory) const;
	const std::vector<CraftRecipe>& getAllRecipes() const;
};

