#include "ItemDatabase.h"

//static멤버변수의 초기화
ItemDatabase* ItemDatabase::instance = nullptr;


void ItemDatabase::InitializeItemDatabase()
{
	////재료는 100부터
	////포션은 200부터
	//itemDatabase.insert({ 1,Item{1,"정화수","불순물 없는 깨끗한 물. 연금의 기초"} });
	//itemDatabase.insert({ 2,Item{2,"마력수","은은한 마력이 담긴 물. 재료의 마력을 녹여낸다."} });
	//itemDatabase.insert({ 11,Item{11,"반짝이풀","빛이 잘 들지않는 동굴에서 자라는 풀. 약간의 빛을 받으면 반짝인다."} });
	//itemDatabase.insert({ 12,Item{12,"해맞이꽃","햇빛이 잘 드는 평야에서 피는 꽃. 해가 지면 숨어버린다."} });
	//itemDatabase.insert({ 13,Item{13,"사망초","근처 식물의 양분을 빨아먹는 불길한 보라색 풀."} });
	//itemDatabase.insert({ 14,Item{14,"화염꽃","뜨거운 마력을 지녀 불처럼 타오르는 꽃. 햇빛이 강할때 더 타오른다"} });
	//itemDatabase.insert({ 15,Item{15,"달맞이꽃","구름없는 밝은 밤에만 피는 꽃, 늘어진 꽃봉오리에서는 은은한 빛이 난다.."} });
	//itemDatabase.insert({ 16,Item{16,"물잎초","물가에서 자라는 푸른 초, 가끔 비가 길어질땐 다른곳에서도 나타난다."} });
	//itemDatabase.insert({ 17,Item{17,"오한가시풀","항상 추운곳에서 자라는 가시처럼 생긴 식물, 차가운 파란 가시에 찔리지 않도록 조심해야한다."} });

	//itemDatabase.insert({ 101, Item{101, "초급 회복 물약","기초적인 회복 물약. 상처가 마법처럼 낫지는 않는다."} });
	//itemDatabase.insert({ 102, Item{102, "중급 회복 물약","꽤 비싼 회복 물약. 작은 상처는 없엇던것처럼 낫는다"} });
	//itemDatabase.insert({ 103, Item{103, "고급 회복 물약","명품 회복 물약. 어지간한 상처는 마법처럼 사라진다!"} });
	//itemDatabase.insert({ 104, Item{104, "엘릭서","전설에 나올만한 회복약. 잘린 부위도 재생시키는 명약인만큼 매우 귀하다!"} });

	//itemDatabase.insert({ 105, Item{105, "초급 마력 물약","닿은곳에 마력을 주입하는 기초적인 물약, 식물영양제로도 쓸만하다"} });
	//itemDatabase.insert({ 106, Item{106, "중급 회복 물약","평급 마법사의 하루치 마력을 단번에!"} });
}

std::map<int, Item> ItemDatabase::getAllItemInfo()
{
	return itemDatabase;
}

int ItemDatabase::tryAddCustomItem(const std::string& name, const std::string& desc) {
	int idx = 0;
	int exist = isItemExistByName(name);
	//신규 아이템 추가
	if (isItemExistByName(name) == -1) {
		while (true) {
			idx++;
			//아이템 ID가 사용중이지 않으면 해당 아이디 사용.
			if (not itemDatabase.contains(idx)) {
				itemDatabase.emplace(idx, Item{ idx, name, desc });
				return idx;
			}
		}
	}
	else {
		return isItemExistByName(name);
	}
}

const Item* ItemDatabase::getItemById(const int id)
{
	if (itemDatabase.contains(id)) {
		return &itemDatabase[id];
	}
	return nullptr;
}



const bool ItemDatabase::isItemExistById(int id)
{
	return itemDatabase.contains(id);;
}

const int ItemDatabase::isItemExistByName(const std::string& name)
{
	for (auto& [k, v] : itemDatabase) {
		if (v.getName() == name)
			return k;
	}
	return -1;
}


