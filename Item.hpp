#include <iostream>
#include <vector>
#pragma once // to avoid error redefinition of 'Item' etc

using namespace std;

class Item {
protected:
    string name;
    int calories;
    float price;
public:
	Item (string name, int calories, float price){
		this->name = name;
		this->calories = calories;
		this->price = price;
	}
	Item (string name, string calories, string price){
		this->name = name;
		this->calories = stoi(calories);
		this->price = stof(price);
	}
	virtual string toString(){
		string str_price = to_string(price);
		int i = str_price.find('.');
		if (i == -1) str_price += ".00";
		else str_price = str_price.substr(0, i + 2 + 1); // + 2 for precision
		string str = name + ": £" + str_price + ", "  + to_string(calories) + " cal";
		return str;
	}
	string getName(){
		return name;
	}
	int getCalories(){
		return calories;
	}
	float getPrice(){
		return price;
	}
	virtual bool isTwoForOne() {return false;} // 2-4-1 is not applied to all products except the once with y in corresponding 2-4-1 column
};

class Appetiser: public Item {
private:
    bool shareable;
    bool twoForOne;
public:
	Appetiser(string name, int calories, float price, bool shareable, bool twoForOne): Item(name, calories, price){
		this->shareable = shareable;
		this->twoForOne = twoForOne;
	}
	Appetiser(string name, string calories, string price, string shareable, string twoForOne): Item(name, calories, price){
		this->shareable = (shareable == "y") ? true : false;
		this->twoForOne = (twoForOne == "y") ? true : false;
	}
	string toString() override{
		string str = Item::toString();
		if (shareable) str += " (shareable)";
		if (twoForOne) str += " (2-4-1)";
		return str;
	}
	bool isShareable(){ return shareable;}
	bool isTwoForOne() override {return twoForOne;}
};

class MainCourse: public Item {
public:
	MainCourse(string name, int calories, float price) : Item(name, calories, price){}
	MainCourse(string name, string calories, string price) : Item(name, calories, price){}
};

class Beverage: public Item {
private:
    int volume;
    float abv;
public:
	Beverage(string name, int calories, float price, int volume, float abv):Item(name, calories, price){
		this->volume = volume;
		this->abv = abv;
	}
	Beverage(string name, string calories, string price, string volume, string abv):Item(name, calories, price){
		this->volume = stoi(volume);
		this->abv = stof(abv);
	}
	string toString() override{
		string str = Item::toString();
		str += "(" + to_string(volume) + "ml";
		if (isAlcoholic()) {
			string str_abv = to_string(abv);
			int i = str_abv.find('.');
			if (i == -1) str_abv += ".0";
			else str_abv = str_abv.substr(0, i + 1 + 1); // + 1 for precision
			str += ", " + str_abv + "% abv)";
		}
		else str += ")";
		return str;
	}
	bool isAlcoholic(){
		return abv != 0;
	}
};

class ItemList {
protected:
	vector<Item*> items;
public:
	ItemList(){
		this->items = {};
	}
	virtual string toString() = 0;
	Item* getItem(int i){
		if (i > items.size() || i <= 0)
			throw "Number of item is out of the range";
		return items[i-1];
	}
};