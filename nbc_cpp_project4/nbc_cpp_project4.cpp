#include "ItemDatabase.h"
#include "ItemType.h"
#include "CraftingManager.h"
#include <iostream>

using namespace std;

std::string ItemTypeToString(ItemType c) {
	switch (c) {
	case ItemType::Misc:   return "기타";
	case ItemType::Potion:   return "물약";
	case ItemType::Material:   return "재료";
	default:           return "Unknown";
	}
}

void drawLine() {
	std::cout << "────────────────────────────────────────" << std::endl;
}
void printRecipe(ItemDatabase* database, const CraftRecipe& cr, bool printMaterial = true) {
	cout << database->getItemById(cr.getResultItemId())->getName();
	if (!printMaterial)
		cout << "\t" << endl;
	if (printMaterial) {
		cout << " 제작법───────────────────────────" << endl;
		for (auto [i, c] : cr.getIngredients()) {
			cout << database->getItemById(i)->getName() << c << "개\t";
		}
		cout << endl;
	}

}
void printAllRecipe(ItemDatabase& database, CraftingManager craftManager, bool printMaterial = true) {
	auto& recipes = craftManager.getAllRecipes();
	if (not recipes.size()) {
		cout << "저장된 레시피가 없습니다" << endl;
		return;
	}
	for (CraftRecipe r : recipes) {
		printRecipe(&database, r, printMaterial);
	}
}

void printAllItemInventory(ItemDatabase& database, Inventory& inventory, ItemType type = ItemType::None) {
	std::cout << "보유중인 아이템 목록" << std::endl;
	auto& items = inventory.getStock();
	int col = 0;
	for (auto [key, val] : items) {
		const Item* i = database.getItemById(key);
		if (type == ItemType::None or i->getItemType() == type) {
			cout << i->getName() << " " << val << "개\t";
			col++;
			if (col % 3 == 0) {
				cout << endl;
			}
		}
	}
	if (col % 3 != 0)
		cout << endl;
}

void printAllItemDatabase(ItemDatabase& database, ItemType type = ItemType::None) {
	cout << "ID\t이름\t타입\t설명" << endl;
	for (auto& [key, val] : database.getAllItems()) {
		ItemType it = database.getItemTypeById(key);
		if (type == ItemType::None or it == type)
			cout <<val.getId() << "\t" + val.getName() + "\t" + ItemTypeToString(val.getItemType()) + "\t" + val.getDescription() << endl;
	}
}
int findItemId(ItemDatabase& database, std::string s) {
	int itemId = -1;
	try {
		//id가 입력된 경우
		int inputId = stoi(s);
		if (database.isItemExistById(inputId))
			itemId = inputId;
	}
	catch (std::invalid_argument ia) {
		//이름이 입력된경우
		QueryResult result = database.getItemIdByName(s);
		if (result.resultType == QueryResultType::Success) {
			itemId = result.resultItemId;
		}
	}
	return itemId;
}

int main()
{
	ItemDatabase& itemDB = ItemDatabase::getInstance();
	Inventory inventory{};
	CraftingManager craftManager{};
	itemDB.tryAddCustomItem("빈 병", "물약을 담을 빈 병", ItemType::Material, 3);
	inventory.addItem(0, 3);
	while (true) {
		drawLine();
		std::cout << "⚗️ 연금술 공방 관리 시스템" << std::endl;
		std::cout << "1. 레시피 추가" << std::endl;
		std::cout << "2. 아이템 조합" << std::endl;
		std::cout << "3. 공병 회수" << std::endl;
		std::cout << "4. 인벤토리에 아이템 추가" << std::endl;
		std::cout << "5. 포션 이름으로 레시피 검색" << std::endl;
		std::cout << "6. 재료 이름으로 레시피 검색" << std::endl;
		std::cout << "7. 물약 판매" << std::endl;
		std::cout << "8. 소지중인 아이템 확인" << std::endl;
		std::cout << "9. 도감 확인" << std::endl;
		std::cout << "0. 종료" << std::endl;
		std::cout << "선택: ";

		int choice;
		std::cin >> choice;

		drawLine();
		if (std::cin.fail()) {
			std::cout << "잘못된 입력입니다. 숫자를 입력해주세요." << std::endl;
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			continue;
		}

		if (choice == 1) {
			std::string potionName;
			std::cout << "물약 이름: ";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, potionName);

			QueryResult searchResult = itemDB.getItemIdByName(potionName);
			if (searchResult.resultType == QueryResultType::Success) {
				std::cout << "이미 존재하는 포션입니다." << std::endl;
				continue;
			}

			// 여러 재료를 입력받기 위한 로직
			std::map<std::string, int> ingredients_input;
			std::string ingredient;
			int count;
			std::cout << "필요한 재료들을 입력하세요. (입력 완료 시 '끝' 입력)" << std::endl;

			while (true) {
				std::cout << "재료 입력: ";
				std::getline(std::cin, ingredient);

				// 사용자가 '끝'을 입력하면 재료 입력 종료
				if (ingredient == "끝") {
					break;
				}

				std::cout << "개수 입력: ";
				std::cin >>count;
				std::cin.ignore(10000, '\n');

				if (std::cin.fail() or count <= 0) {
					std::cout << "잘못된 입력입니다. 1 이상 숫자를 입력해주세요." << std::endl;
					std::cin.clear();
					std::cin.ignore(10000, '\n');
					continue;
				}
				ingredients_input.emplace(ingredient,  count);
			}

			// 입력받은 재료가 하나 이상 있을 때만 레시피 추가
			if (!ingredients_input.empty()) {
				std::map<int, int> newRecipe;
				for (auto& [k, v] : ingredients_input) {
					QueryResult addResult = itemDB.tryAddCustomItem(k, "", ItemType::Material);
					if (addResult.resultType == QueryResultType::Success
						or addResult.resultType == QueryResultType::AlreadyExist) {
						newRecipe.emplace(addResult.resultItemId, v);
					}
				}
				QueryResult addPotionResult = itemDB.tryAddCustomItem(potionName, "", ItemType::Potion);
				//기본 재고 3 추가.
				newRecipe.emplace(0, 1);
				CraftResult result = craftManager.addCraftRecipe(newRecipe, addPotionResult.resultItemId);
				if (result.resultType == CraftResultType::Success) {
					std::cout << potionName << " 제작법이 추가되었습니다. 기본 재고가 3개 추가됩니다." << std::endl;
					inventory.addItem(addPotionResult.resultItemId, 3);
				}
				else if( result.resultType == CraftResultType::RecipeAlreadyExist ) {
					std::cout << "재료 조합이 이미 존재하는 조합법입니다, 생성을 중단합니다." << std::endl;
					itemDB.deleteItem(addPotionResult.resultItemId);
				}

			}
			else {
				std::cout << ">> 재료가 입력되지 않아 레시피 추가를 취소합니다." << std::endl;
			}

		}
		else if (choice == 2) {
			auto& recipes = craftManager.getAllRecipes();
			if (not recipes.size()) {
				cout << "저장된 레시피가 없습니다" << endl;
				continue;
			}
			if (inventory.getItemCount(0) == 0) {
				std::cout << "물약 제조에 사용할 빈 병이 없습니다. 회수해주세요." << std::endl;
				choice = 0;
			}
			else {
				std::cout << "제작 방법을 선택하세요. 수동 : 1 / 자동 :2 / 외 취소" << std::endl;
				std::cin >> choice;
			}
			if (choice == 1) {
				printAllItemInventory(itemDB, inventory);
				std::cout << "제작에 투입할 재료 이름과 개수를 입력하세요. (입력 완료 시 '끝', 취소시 '취소')" << std::endl;
				std::cout << "참고 : 물약 조제에는 빈 병이 반드시 필요합니다!" << std::endl;
				std::cin.ignore(10000, '\n');
				std::cout.clear();
				std::map<int, int> ingredients_input;
				std::string ingredient;
				int count;
				bool cancelFlag = false;
				while (true) {
					std::cout << "재료 입력(이름 또는 ID): ";
					std::getline(std::cin, ingredient);

					if (ingredient == "끝") {
						break;
					}
					if (ingredient == "취소") {
						cancelFlag = true;
						break;
					}

					std::cout << "개수 입력: ";
					std::cin >> count;
					std::cin.ignore(10000, '\n');

					// 사용자가 '끝'을 입력하면 재료 입력 종료

					int itemId = -1;
					try {
						//id가 입력된 경우
						int inputId = stoi(ingredient);
						if (itemDB.isItemExistById(inputId))
							itemId = inputId;
					}
					catch (std::invalid_argument ia) {
						//이름이 입력된경우
						QueryResult result = itemDB.getItemIdByName(ingredient);
						if (result.resultType != QueryResultType::Success) {
							cout << "존재하지 않는 아이템입니다." << endl;
							break;
						}
						else if (result.resultType != QueryResultType::AlreadyExist) {
							itemId = result.resultItemId;
						}
					}
					if (itemId != -1) {
						StoreResult result = inventory.hasEnoughItem(itemId, count);
						if (result.resultType != StoreResultType::Success) {
							std::cout << "투입할 재료가 부족합니다!" << std::endl;
							ingredients_input.clear();
							break;
						}
						else
							ingredients_input.emplace(itemId, count);
					}


				}
				if (!ingredients_input.empty() or cancelFlag == false) {
					CraftResult craftResult = craftManager.customCraft(ingredients_input);
					auto item = itemDB.getItemById(craftResult.resultItemId);
					if (inventory.canStoreItem(craftResult.resultItemId, 1) == false) {
						std::cout << item->getName() << "을(를) 저장할 공간이 없습니다. 취소합니다." << std::endl;
					}
					if (craftResult.resultType != CraftResultType::Success)
						std::cout << "적합한 레시피가 없습니다, 제작에 실패했습니다." << std::endl;
					else {
						std::cout << item->getName() << "제작에 성공했습니다!" << std::endl;
						for (auto [k, v] : ingredients_input) {
							inventory.consumeItem(k, v);
						}
						inventory.addItem(item->getId(), 1);
					}
				}
				else {
					std::cout << "작업이 취소됩니다." << std::endl;
				}
			}
			else if (choice == 2) {
				printAllRecipe(itemDB, craftManager, false);
				std::string name;
				std::cout << "제작할 아이템 입력 : ";
				std::cin.ignore(10000, '\n');
				std::getline(std::cin, name);

				QueryResult result = itemDB.getItemIdByName(name);
				if (result.resultType == QueryResultType::Success) {
					if(inventory.canStoreItem(result.resultItemId, 1) == false) {
						std::cout << name << "을(를) 저장할 공간이 없습니다. 재고 판매후 진행해주세요." << std::endl;
						continue;
					}
					CraftResult craftResult = craftManager.craft(result.resultItemId, inventory);

					if (craftResult.resultType == CraftResultType::Success) {
						std::cout << name << "제작에 성공했습니다" << std::endl;
					}
					//다양한 실패문구.
					else if (craftResult.resultType == CraftResultType::NotEnoughIngredient) {
						std::cout << name << "을(를) 제작할 재료가 부족합니다." << std::endl;

					}
				}
				else {
					std::cout << "제작법을 찾지 못했습니다." << std::endl;

				}
			}
			else {

			}
			
		}
		else if (choice == 3) {
			int bottleCount = inventory.getItemCount(0);
			int recall = 3 - bottleCount;
			if (recall != 0) {
				std::cout << 3 - bottleCount << "개의 빈 병을 회수합니다." << std::endl;
				inventory.addItem(0, recall);
			}
			else {
				std::cout << "회수할 병이 없습니다" << std::endl;
			}


		}
		else if (choice == 4) {
			std::cout << "추가할 아이템의 이름이나 아이디, 갯수를 입력하세요." << std::endl;
			std::string name;
			int count;
			std::cout << "추가할 아이템 입력 : ";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, name);

			std::cout << "개수 입력: ";
			std::cin >> count;
			std::cin.ignore(10000, '\n');
			int id;
			try {
				//숫자변환가능한경우
				int sid = stoi(name);
				if (itemDB.isItemExistById(sid)) {
					id = sid;
				}
				else {
					std::cout << "아이템을 찾을 수 없습니다." << std::endl;
				}
			}
			catch (std::invalid_argument e) {
				QueryResult result = itemDB.getItemIdByName(name);
				if (result.resultType == QueryResultType::Success) {
					id = result.resultItemId;
				}
				else {
					std::cout << "아이템을 찾을 수 없습니다." << std::endl;
				}
			}
			StoreResult addResult = inventory.addItem(id, count);
			if (addResult.resultType == StoreResultType::Success)
				std::cout << "인벤토리에 아이템이 추가되었습니다." << std::endl;
			else if (addResult.resultType == StoreResultType::CannotStackItems)
				std::cout << "해당 아이템을 더 가질 수 없습니다.(최대 재료 100, 포션 3개)" << std::endl;
			else if (addResult.resultType == StoreResultType::AtLeastOneRequired)
				std::cout << "최소 1개 입력해야합니다" << std::endl;


		}
		else if (choice == 5) {
			printAllItemDatabase(itemDB, ItemType::Potion);
			std::string name;
			std::cout << "제작법을 알고싶은 아이템 입력 : ";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, name);

			int result = findItemId(itemDB, name);
			if (result != -1) {
				const CraftRecipe* cr = craftManager.findRecipeByResultItemId(result);
				if (cr != nullptr) {
					printRecipe(&itemDB, *cr);
				}
			}
		}
		else if (choice == 6) {
			printAllItemInventory(itemDB, inventory, ItemType::Material);
			std::string name;
			std::cout << "어디에 쓰이는지 알고싶은 재료 이름 입력(정확히 입력하세요) : ";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, name);

			int result = findItemId(itemDB, name);
			if (result != -1) {
				const vector<int> recipes = craftManager.findRecipesByIngredientId(result);
				for (int i : recipes) {
					const CraftRecipe* cr = craftManager.findRecipeByResultItemId(i);
					if (cr != nullptr) {
						printRecipe(&itemDB, *cr);
					}
				}
			}
		}
		else if (choice == 7) {
			auto& potions = inventory.searchItemsByType(ItemType::Potion);
			if (potions.size() == 0) {
				std::cout << "판매할 포션이 없습니다." << std::endl;
				continue;
			}
			printAllItemInventory(itemDB, inventory, ItemType::Potion);
			std::string name;
			std::cout << "판매할 포션의 이름 입력 : ";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, name);

			int resultId = findItemId(itemDB, name);
			if (resultId != -1) {
				StoreResult result = inventory.hasEnoughItem(resultId, 1);
				if (result.resultType == StoreResultType::Success) {
					inventory.consumeItem(resultId, 1);
					std::cout << name << "을(를) 판매하였습니다." << std::endl;

				}
				else {
					std::cout << "재고가 부족합니다." << std::endl;

				}
			}

		}
		else if (choice == 8) {
			std::cout << "확인 방법을 선택하세요. 전체 : 1 / 검색 : 2 / 외 취소" << std::endl;
			std::cin >> choice;
			if (choice == 1) {
				printAllItemInventory(itemDB, inventory);
			}
			else if (choice == 2) {
				std::string search;
				std::cout << "검색할 재료 이름 입력(일부분도 가능): ";
				std::cin.ignore(10000, '\n');
				std::getline(std::cin, search);
				map<int, int> searchResult = inventory.searchItemsByName(search);
				std::cout << "검색 결과: " << endl;
				for (auto& [k, v] : searchResult) {
					std::string name = itemDB.getItemById(k)->getName();
					std::cout << name << " " << v << "개" << std::endl;
				}
				
			}
		}
		else if (choice == 9) {
			std::cout << "아이템 확인 : 1, 레시피 확인 : 2" << std::endl;
			std::cin >> choice;
			if (choice == 1) {
				printAllItemDatabase(itemDB);
			}
			else if (choice == 2) {
				printAllRecipe(itemDB, craftManager, true);
			}
			else {}
			//myWorkshop.displayAllRecipes();
		}
		else if (choice == 0) {
			std::cout << "공방 문을 닫습니다..." << std::endl;
			break;

		}
		else {
			std::cout << "잘못된 선택입니다. 다시 시도하세요." << std::endl;
		}
	}
	return 0;
}
