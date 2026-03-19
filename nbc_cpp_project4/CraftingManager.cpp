#include "CraftingManager.h"

void CraftingManager::InitializeRecipes()
{
	// 여기에 초기 데이터 예시
	//craftRecipes.push_back(CraftRecipe{ 1, {{1,1},{2,1}} });
}

CraftResult CraftingManager::customCraft(const std::map<int, int>& items) const
{
	for (const auto& rcp : craftRecipes) {
		if (rcp.matchesIngredient(items)) {
			return { CraftResultType::Success, rcp.getResultItemId() };
		}
	}
	return { CraftResultType::RecipeNotFound, -1 };
}

CraftResult CraftingManager::craft(int itemId, Inventory& inventory)
{
	//CraftResult craftResult{ CraftResultType::RecipeNotFound, -1 };
	if (itemDB.isItemExistById(itemId) == false) {
		return { CraftResultType::InvalidItem, -1 };
	}

	const CraftRecipe* recipe = findRecipeByResultItemId(itemId);
	if (recipe == nullptr) {
		return { CraftResultType::RecipeNotFound, -1 };
	}

	//재고 충분한지 확인
	const std::map<int,int>& requiredIngredients = recipe->getIngredients();
	for (const auto& [id, count] : requiredIngredients) {
		if (inventory.hasEnoughItem(id, count) == false) {
			return { CraftResultType::NotEnoughIngredient, -1 };
		}
	}

	for (const auto& [id, count] : requiredIngredients) {
		bool consumed =inventory.consumeItem(id, count);
		if (consumed == false) {
			//앞에서 체크했는데 소모가 안되면???
			//원자성을 유지할 방법 생각
		}
	}
	//아이템 소모후 인벤토리에 추가, 성공 반환.
	inventory.addItem(recipe->getResultItemId(), 1);
	return { CraftResultType::Success, recipe->getResultItemId() };
}

bool CraftingManager::addCraftRecipe(const std::map<int, int>& items, const int resultItemId)
{
	if (findRecipeByResultItemId(resultItemId) == nullptr) {
		CraftRecipe newRecipe{ resultItemId, items };
		craftRecipes.push_back(newRecipe);
		return true;
	}
	return false;
}

const CraftRecipe* CraftingManager::findRecipeByResultItemId(const int id) const
{
	for (const auto& rcp : craftRecipes) {
		if (rcp.getResultItemId() == id)
			return &rcp;
	}
	return nullptr;
}

//아래처럼하면 댕글링포인터 발생함 
//const CraftRecipe& CraftingManager::getRecipeById(int id)
//{
//	for (CraftRecipe rcp : craftRecipes) {<- rcp는 복사본임
//		if (rcp.getItemId() == id)
//			return rcp;<-즉 이건 지역변수의 반환임.
//	}
//	return CraftRecipe();<-마찬가지로 지역변수 임시객체
//}
// 지역변수를 값참조로 반환을 하면 함수를 벗어나면서 그즉시 파괴됨.
// 이거 보일때마다 복기하기

std::vector<int> CraftingManager::findRecipesByIngredientId(int ingredientId) const
{
	std::vector<int> resultItems;
	for (const auto& rcp : craftRecipes) {
		const auto& recipeIngredients = rcp.getIngredients();
		if (recipeIngredients.contains(ingredientId)) {
			resultItems.push_back(rcp.getResultItemId());
		}
	}
	return resultItems;
}

std::vector<int> CraftingManager::findRecipesByInventory(const Inventory& inventory) const
{
	std::vector<int> craftables;
	for (const auto& recipe : craftRecipes) {
		bool isCraftable = true;
		const auto& ingredient = recipe.getIngredients();
		for (const auto& [k, v] : ingredient) {
			if (inventory.hasEnoughItem(k, v) == false) {
				isCraftable = false;
				break;
			}
		}

		if(isCraftable)
			craftables.push_back(recipe.getResultItemId());
	}
	return craftables;
}

const std::vector<CraftRecipe>& CraftingManager::getAllRecipes() const
{
	return craftRecipes;
}