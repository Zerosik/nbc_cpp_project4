#include "CraftingManager.h"

void CraftingManager::InitializeRecipes()
{
	//craftRecipes.push_back(CraftRecipe{ 101, { {1, 1},{11, 1},{12, 1}} });
	//craftRecipes.push_back(CraftRecipe{ 102, { {1, 1},{12, 1},{15, 1}} });
	//craftRecipes.push_back(CraftRecipe{ 105, { {1, 1},{2, 1},{15, 1},{16, 1} }

}

const int CraftingManager::CustomCraft(const std::map<int, int>& items)
{
	int resultid = -1;
	for (CraftRecipe rcp : craftRecipes) {
		if (rcp.compareIngredient(items)) {
			return rcp.getItemId();
		}
	}

	return resultid;
}

const int CraftingManager::Craft(int ItemID, Inventory& inventory)
{
	int resultid = -1;
	for (CraftRecipe rcp : craftRecipes) {
		if (rcp.getItemId()==ItemID) {
			bool can_make = true;
			//재고 충분한지 확인
			for (const auto& [id, count] : rcp.getIngredients()) {
				if (inventory.enoughItem(id, count) == false){
					can_make = false;
					break;
				}
			}
			if (can_make) {
				resultid = rcp.getItemId();
				//재고 소모
				for (const auto& [id, count] : rcp.getIngredients()) {
					inventory.consumeItem(id, count);
				}
			}
			break;
		}
	}
	return resultid;
}

const bool CraftingManager::addCraftRecipe(const std::map<int, int>& items, const int ResultItemId)
{
	CraftRecipe newRecipe{ ResultItemId, items};
	craftRecipes.push_back(newRecipe);
	return true;
}
const CraftRecipe* CraftingManager::getRecipeById(int id)
{
	for (const auto& rcp : craftRecipes) {
		if (rcp.getItemId() == id)
			return &rcp;
	}
	return nullptr;
}

//아래처럼하면 댕글링포인터 발생함
//const CraftRecipe& CraftingManager::getRecipeById(int id)
//{
//	for (CraftRecipe rcp : craftRecipes) {
//		if (rcp.getItemId() == id)
//			return rcp;
//	}
//	return CraftRecipe();
//}

const std::vector<int> CraftingManager::getItemUsingIngredient(int ingredientId) const
{
	std::vector<int> items;
	for (const auto& rcp : craftRecipes) {
		const auto& recipeIngredients = rcp.getIngredients();
		if (recipeIngredients.contains(ingredientId)) {
			items.push_back(rcp.getItemId());
		}
	}
	return items;
}

const std::vector<int> CraftingManager::getCraftableItems(Inventory& inventory) const
{
	std::vector<int> craftables;
	for (auto& recipe : craftRecipes) {
		bool isCraftable = true;
		auto ingredient = recipe.getIngredients();
		for (auto& [k,v] : ingredient) {
			if (!inventory.enoughItem(k, v))
				break;
		}
		if(isCraftable)
			craftables.push_back(recipe.getItemId());
	}
	return craftables;
}

const std::vector<CraftRecipe>& CraftingManager::getAllRecipes()
{
	return craftRecipes;
}

//void CraftingManager::showAllRecipes()
//{
//}
