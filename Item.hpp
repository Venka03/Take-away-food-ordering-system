#include <iostream>
#include <vector>
#include <cmath>
#pragma once // to avoid error redefinition of 'Item' etc

using namespace std;

string stringRound(float number, int precision){
	// save in string rounded 'number' to the 'precision' significant digit after comma
	if (precision < 1) throw "Wrong precision parameter, precision should be bigger than 0";
	string str_number = "";
	// round is used since without it number could be smaller by 1 due to the way numbers are stored in memory
	int n = pow(10, precision);
	int k = round(number * n);
    for (int i=0; i < precision; i++)
        str_number.insert(str_number.begin(), '0' + (k / (int) pow(10, i)) % 10);
	str_number = to_string( (int) k / n) + '.' + str_number;
	return str_number;
}

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
		string str = name + ": £" + stringPrice(price) + ", "  + to_string(calories) + " cal";
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
	static string stringPrice(float price){
		return stringRound(price, 2);
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
			str += ", " + stringRound(abv, 1) + "% abv)";
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
		// by given number, return item from the items vector
		if (i > numberOfEntries() || i <= 0)
			throw "Number of item is out of the range";
		return items[i-1];
	}
	int numberOfEntries(){ return items.size();}
};