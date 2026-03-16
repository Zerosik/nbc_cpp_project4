//CraftingManager.h
#pragma once
#include "ItemDatabase.h"
#include "CraftRecipe.h"
#include "Inventory.h"
#include <map>
#include <vector>

class CraftingManager
{
private:
	std::vector<CraftRecipe> craftRecipes;
	ItemDatabase* db;
	void InitializeRecipes();
public:
	CraftingManager(){
		db = ItemDatabase::getInstance();
		InitializeRecipes();
	}
	//아이템 직접 제작 시도, 실패시 -1
	const int CustomCraft(const std::map<int, int>& items);
	//특정 아이템 제작, 아이템id, 인벤토리 참조
	const int Craft(int ItemID, Inventory& inventory);

	const bool addCraftRecipe(const std::map<int, int>& items, const int ResultItemId);
	//특정 아이템 레시피
	const CraftRecipe* getRecipeById(int id);
	//레시피 중 특정 재료를 사용하는 아이템 리스트
	const std::vector<int> getItemUsingIngredient(const int ingredientId) const;
	//인벤토리참조하여 가능한 모든 제작 가능한 아이템 반환
	const std::vector<int> getCraftableItems(Inventory& inventory) const;
	//모든 레시피 출력
	const std::vector<CraftRecipe>& getAllRecipes();
};

