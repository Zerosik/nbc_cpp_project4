//#include "AlchemyWorkshop.h"
#include "ItemDatabase.h"
#include "CraftingManager.h"
#include <iostream>

using namespace std;

void drawLine() {
	std::cout << "────────────────────────────────────────" << std::endl;
}
void printRecipe(ItemDatabase* database, const CraftRecipe& cr) {
	cout << database->getItemById(cr.getItemId())->getName() << " 제작법" << endl;
	for (auto [i, c] : cr.getIngredients()) {
		cout << database->getItemById(i)->getName() << c << "개\t";
	}
	cout << endl;

}
void printAllRecipe(ItemDatabase* database, CraftingManager cManager) {
	auto& recipes = cManager.getAllRecipes();
	if (not recipes.size()) {
		cout << "저장된 레시피가 없습니다" << endl;
		return;
	}
	for (CraftRecipe r : recipes) {
		printRecipe(database, r);
	}
}

void printItem(ItemDatabase* database, int itemId) {
	cout << database->getItemById(itemId)->getName() << endl;
}
void printAllItemInventory(ItemDatabase* database, Inventory& inventory) {
	std::cout << "보유중인 아이템 목록" << std::endl;
	auto& items = inventory.getStock();
	int col = 0;
	for (auto [key, val] : items) {
		col++;
		const Item* i = database->getItemById(key);
		cout << i->getName() << " " << val << "개\t" ;
		//if(i.getDescription() != "")
		//	cout << i.getDescription() << endl << endl;
		if (col%3==0) {
			cout << endl;
		}
	}
	cout << endl;
}
void printAllItemDatabase(ItemDatabase* database) {
	cout << "ID\t이름\t설명" << endl;
	for (auto [key, val] : database->getAllItemInfo()) {
		cout <<val.getId() << "\t" + val.getName() + "\t" + val.getDescription() << endl;
	}
}
int findItemId(ItemDatabase* database, std::string s) {
	int itemId = -1;
	try {
		//id가 입력된 경우
		int inputId = stoi(s);
		if (database->isItemExistById(inputId))
			itemId = inputId;
	}
	catch (std::invalid_argument ia) {
		//이름이 입력된경우
		int result = database->isItemExistByName(s);
		if (result != -1) {
			itemId = result;
		}
	}
	return itemId;
}

int main()
{
	ItemDatabase* itemDB = ItemDatabase::getInstance();
	Inventory inventory{};
	CraftingManager craftManager{};
	int Bottle = 3;
	while (true) {
		drawLine();
		std::cout << "⚗️ 연금술 공방 관리 시스템" << std::endl;
		std::cout << "1. 레시피 추가" << std::endl;
		std::cout << "2. 아이템 조합(수동)" << std::endl;
		std::cout << "3. 아이템 조합(자동)" << std::endl;
		std::cout << "4. 아이템 인벤토리에 추가" << std::endl;
		std::cout << "5. 레시피 이름으로 검색" << std::endl;
		std::cout << "6. 재료로 레시피 검색" << std::endl;
		std::cout << "7. 소지중인 아이템 출력" << std::endl;
		std::cout << "8. 모든 레시피 출력" << std::endl;
		std::cout << "9. 모든 아이템 출력" << std::endl;
		std::cout << "0. 종료" << std::endl;
		std::cout << "선택: ";

		int choice;
		std::cin >> choice;


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

				ingredients_input.emplace(ingredient,  count);
			}

			// 입력받은 재료가 하나 이상 있을 때만 레시피 추가
			if (!ingredients_input.empty()) {
				std::map<int, int> newRecipe;
				for (auto& [k, v] : ingredients_input) {
					int newItmeId = itemDB->tryAddCustomItem(k, "");
					newRecipe.emplace(newItmeId, v);
					inventory.addItem(newItmeId, 3);
				}
				int PotionId = itemDB->tryAddCustomItem(potionName, "");
				//inventory.addItem(PotionId, 0);
				craftManager.addCraftRecipe(newRecipe, PotionId);

			}
			else {
				std::cout << ">> 재료가 입력되지 않아 레시피 추가를 취소합니다." << std::endl;
			}

		}
		else if (choice == 2) {
			printAllItemInventory(itemDB, inventory);
			std::cout << "제작에 투입할 재료 이름과 개수를 입력하세요. (입력 완료 시 '끝' 입력)" << std::endl;
			std::cin.ignore(10000, '\n');
			std::cout.clear();
			std::map<int, int> ingredients_input;
			std::string ingredient;
			int count;

			while (true) {
				std::cout << "재료 입력(이름 또는 ID): ";
				std::getline(std::cin, ingredient);

				if (ingredient == "끝") {
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
					if (itemDB->isItemExistById(inputId))
						itemId = inputId;
				}
				catch (std::invalid_argument ia) {
					//이름이 입력된경우
					int result = itemDB->isItemExistByName(ingredient);
					if (result == -1) {
						cout << "존재하지 않는 아이템입니다." << endl;
						break;
					}
					else {
						itemId = result;
					}
				}
				if (itemId != -1) {
					if (not inventory.enoughItem(itemId, count)) {
						std::cout << "투입할 재료가 부족합니다!" << std::endl;
						ingredients_input.clear();
						break;
					}else
						ingredients_input.emplace(itemId, count);
				}
			
				
			}
			if (!ingredients_input.empty()) {
				int result = craftManager.CustomCraft(ingredients_input);
				for (auto [k, v] : ingredients_input) {
					inventory.consumeItem(k, v);
				}
				if (result == -1)
					std::cout << "제작에 실패했습니다. 재료는 소모됩니다." << std::endl;
				else {
					auto item = itemDB->getItemById(result);
					std::cout << item->getName() << "제작에 성공했습니다!" << std::endl;
				}
			}
			else {
				std::cout << "작업이 취소됩니다." << std::endl;
			}


		}
		else if (choice == 3) {
			printAllRecipe(itemDB, craftManager);
			std::string name;
			std::cout << "제작할 아이템 입력 : ";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, name);

			int targetId = itemDB->isItemExistByName(name);
			if (targetId != -1) {
				std::cout << name << "제작에 성공했습니다" << std::endl;

				int resultid = craftManager.Craft(targetId, inventory);
				inventory.addItem(resultid, 1);
			}
			else {
				std::cout << "제작법을 찾지 못했습니다." << std::endl;

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
			try {
				//숫자변환가능한경우
				int id = stoi(name);
				if (itemDB->isItemExistById(id)) {
					inventory.addItem(id, count);
					std::cout << "인벤토리에 아이템이 추가되었습니다." << std::endl;
				}
				else {
					std::cout << "추가할 아이템이 존재하지 않습니다." << std::endl;
				}
			}
			catch (std::invalid_argument e) {
				int result = itemDB->isItemExistByName(name);
				if (result!=-1) {
					inventory.addItem(result, count);
					std::cout << "인벤토리에 아이템이 추가되었습니다." << std::endl;
				}
				else {
					std::cout << "추가할 아이템이 존재하지 않습니다." << std::endl;
				}
			}


		}
		else if (choice == 5) {
			std::string name;
			std::cout << "제작법을 알고싶은 아이템 입력 : ";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, name);

			int result = findItemId(itemDB, name);
			if (result != -1) {
				const CraftRecipe* cr = craftManager.getRecipeById(result);
				if (cr != nullptr) {
					printRecipe(itemDB, *cr);
				}
			}
		}
		else if (choice == 6) {
			printAllItemInventory(itemDB, inventory);
			std::string name;
			std::cout << "어디에 쓰이는지 알고싶은 재료 이름 입력 : ";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, name);

			int result = findItemId(itemDB, name);
			if (result != -1) {
				const vector<int> recipes = craftManager.getItemUsingIngredient(result);
				for (int i : recipes) {
					const CraftRecipe* cr = craftManager.getRecipeById(i);
					if (cr != nullptr) {
						printRecipe(itemDB, *cr);
					}
				}
			}
		}
		else if (choice == 7) {
			//myWorkshop.displayAllRecipes();
			printAllItemInventory(itemDB, inventory);
		}
		else if (choice == 8) {
			//myWorkshop.displayAllRecipes();
			printAllRecipe(itemDB, craftManager);
		}
		else if (choice == 9) {
			//myWorkshop.displayAllRecipes();
			printAllItemDatabase(itemDB);
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
