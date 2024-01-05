#include <iostream>
#include <fstream>
#include <vector>
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
};

class Menu:ItemList {
public:
	Menu(string link){
		ifstream menu; 
		string line;
		// Open an existing file 
		menu.open(link);
		if (!menu) cout << "File at given address does not exist" << endl;
		string str;
		vector<string> separated;
		while (getline(menu, line)) {
			separated.clear();
			int i = line.find(','); // find first appearance of separator ','
			int next_after_last = 0;
			while (i != -1){
				separated.push_back(line.substr(next_after_last, i-next_after_last)); // add substring between two appearances of separators
				next_after_last = i + 1;
				i = line.find(',', next_after_last); // find next appearance of separator
			}
			separated.push_back(line.substr(next_after_last, line.length()-next_after_last)); // add last substring
			switch (line[0]){
				case 'a':
					items.push_back(new Appetiser(separated[1], separated[3], separated[2], separated[4], separated[5]));
					break;
				case 'm':
					items.push_back(new MainCourse(separated[1], separated[3], separated[2]));
					break;
				case 'b':
					items.push_back(new Beverage(separated[1], separated[3], separated[2], separated[6], separated[7]));
					break;
				default:
					throw "Unexpected input in given csv file";
			}
		}
	}
	string toString() {
		string menu = "\t\tMenu:\n";
		//cout << "----------Appetisers----------\n";
		for (auto& item: items){
			cout << item->toString() << endl;
		}
		for (int i=0; i<items.size(); i++){
			//cout << '(' << i+1 << ") " << items[i]->getName() << ": £" << items[i]->getPrice() << ", " << items[i]->getCalories() << endl;
			cout << '(' << i+1 << ") ";
			cout << items[i]->toString() << endl;
		}
	}
};

class Order:ItemList {
private:
	float total;
public:
	float calculateTotal(){}
	void printReceipt(){}
	string toString(){}


};

int main(){
	Menu menu ("/Users/venka/Desktop/Adam/menu.csv");
	menu.toString();
	
}