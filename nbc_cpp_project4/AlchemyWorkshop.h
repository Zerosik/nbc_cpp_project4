#pragma once
#include <optional>//비어있을때 std::nullopt 명시
//#include <iostream>
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
////템플릿 클래스 제한에 사용
//#include <type_traits>
//#include "ItemDatabase.h"
//#include "CraftingManager.h"
//
//
//
////연금술 공방
//class AlchemyWorkshop
//{
//private:
//	Inventory inventory;
//	CraftingManager craftManager;
//	int bottle;
//	//AlchemyCraftstation craftStation;
//	int alchemicPoint;
//public:
//	AlchemyWorkshop();
//
//	void addIngredient(const std::string name, int count);
//
//	//알케믹포인트를 이용해 랜덤 레시피 해금
//	void findRandomRecipe(int ap);
//	//랜덤 재료 획득, 바로 ingredients에 추가
//	void findRandomIngredients();
//};


































//
//class ItemBase {
//public:
//	virtual const std::string getName() const = 0;
//	virtual const std::string getDescription() const = 0;
//	//정렬을 위한 비교연산.
//	bool operator<(const ItemBase& other) const {
//		return this->getName() < other.getName();
//	}
//	bool operator==(const ItemBase& other) const {
//		return this->getName() == other.getName();
//	}
//};
//
////확장을 위해 분리
////재료 정의
//class Ingredient : public ItemBase{
//private:
//	std::string name;
//	std::string description;
//public:
//	explicit Ingredient(const std::string& name, const std::string& desc) : name(name), description(desc) {}
//	const std::string getName() const override { return name; }
//	const std::string getDescription() const override { return description; }
//};
//
//
////포션 정의
//class Potion : public ItemBase {
//private:
//	std::string name;
//	std::string description;
//public:
//	explicit Potion(const std::string& n,const std::string& desc):name(n), description(desc) {}
//	const std::string getName() const override { return name; }
//	const std::string getDescription() const override { return description; }
//};
//
//template <typename T>
//class Inventory {
//	//이 에러는 빌드시(컴파일 타임)에 발생함.
//	static_assert(std::is_base_of<ItemBase, T>::value,
//		"T 는 반드시 Potion이나 Ingredient객체 사용 (Ingredient or Potion)");
//	//이후로는 그냥 T를 BaseItem처럼 사용
//private:
//	std::map<T, int> items;
//public:
//	//재료 추가
//	void add(const T& item, int count) {
//		items[item] += count;
//	}
//	//필요 재료가 충분한지 확인
//	bool hasEnough(const T& item, int count) {
//		//std::cout << item.getName() << std::endl;
//		if (items.find(item) == items.end()) {
//			return false;
//		}
//		return items[item] >= count;
//	}
//	//재료 소모, 부족하면 실패
//	bool consume(const T& item, int count) {
//		if (items.find(item) == items.end()) {
//			//std::cout << item.getName() << "의 재고가 없습니다" << std::endl;
//			return false;
//		}
//		if (items[item] < count) {
//			//std::cout << item.getName() << "의 재고가 " << count - items[item] << "개 부족합니다." << std::endl;
//			return false;
//		}
//		items[item] -= count;
//		return true;
//	}
//	void printAll() {
//		for (const auto& [key, value] : items) {
//			std::cout << key.getName() << ' ' << value << "개 보유중" << std::endl;
//		}
//	}
//	void clear() const { items.clear(); }
//	const std::map<T, int> getAllItems() const { return items; }
//
//};
//
////포션 레시피를 정의
//class PotionRecipe {
//private:
//	//레시피 결과
//	std::string resultPotion;
//	//재료명, 개수
//	std::map<Ingredient, int> requireIngredient;
//public:
//	bool operator<(const PotionRecipe& other) const {
//		return this->resultPotion < other.resultPotion;
//	}
//	//같은 레시피 확인을 위해 재료만 비교
//	bool operator==(const PotionRecipe& other) const {
//		return this->requireIngredient == other.requireIngredient;
//	}
//	PotionRecipe(const std::string& name,const std::map<Ingredient, int>& require);
//	//레시피 출력
//	std::string getPotionName() const { return resultPotion; }
//	std::map<Ingredient, int> getIngredient() const { return requireIngredient; }
//	//void printRecipe() const;
//};
//
//
////레시피 관리
//class RecipeBook {
//private:
//	//std::map<std::string, PotionRecipe> recipes;
//	std::map<PotionRecipe, bool> recipes;
//public:
//	//레시피 추가
//	void addRecipe(std::string name, std::map<Ingredient, int> ingredients);
//	//보유한 모든 레시피 출력
//	std::set<PotionRecipe> getAllRecipes(bool showUnknown = false) const;
//	//이름에 해당하는 레시피 반환
//	std::optional<PotionRecipe> searchRecipeByName(std::string name);
//	//입력한 재료를 사용하는 발견된 레시피들 반환
//	std::set<PotionRecipe> searchByIngredient(std::string ingredientName);
//	//입력한 재료를 사용하는 발견된 레시피 반환
//	std::optional<PotionRecipe> searchByRecipe(PotionRecipe recipe);
//
//
//#ifdef _DEBUG
//	void unlockAllRecipes();
//	void lockAllRecipes();
//#endif
//};
//
//struct craftResult {
//	bool success;
//	std::string message;
//	std::optional<Potion> result;
//};
//
//
//
////연금술 제작 관련
//class AlchemyCraftstation {
//private:
//	RecipeBook recipes;
//	void addDefaultRecipes();
//public:
//
//	//레시피를 아는경우 재고를 참조하여 제작시도
//	craftResult craftPotion(Inventory<Ingredient>& inven, const PotionRecipe& recipe, int& bottle);
//	//사용자 입력에 따른 포션 조합 시도, 존재하는 레피
//	craftResult craftUnknownPotion(std::map<std::string, int> ingredients, int& bottle);
//
//	void displayAllRecipes() const;
//};
//
//
//
////연금술 공방
//class AlchemyWorkshop
//{
//private:
//	Inventory<Ingredient> ingredientInventory;
//	Inventory<Potion> potionInventory;
//	int bottle;
//	AlchemyCraftstation craftStation;
//	int alchemicPoint;
//public:
//	AlchemyWorkshop();
//
//	void addIngredient(const std::string name, int count);
//
//	//알케믹포인트를 이용해 랜덤 레시피 해금
//	void findRandomRecipe(int ap);
//	//랜덤 재료 획득, 바로 ingredients에 추가
//	void findRandomIngredients();
//};
//
