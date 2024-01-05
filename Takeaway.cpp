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

class Menu: public ItemList {
private:
    int numberOfAppetiser;
    int numberOfMainCourse;
    int numberOfBeverage;
public:
	Menu(string link){
		ifstream menu; 
		string line;
		// Open an existing file 
		menu.open(link);
		if (!menu) cout << "File at given address does not exist" << endl;
		string str;
		vector<string> separated;
        numberOfAppetiser = 0;
        numberOfMainCourse = 0;
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
					//items.push_back(new Appetiser(separated[1], separated[3], separated[2], separated[4], separated[5]));
                    items.insert(items.begin(), new Appetiser(separated[1], separated[3], separated[2], separated[4], separated[5]));
                    numberOfAppetiser++;
					break;
				case 'm':
					//items.push_back(new MainCourse(separated[1], separated[3], separated[2]));
                    items.insert(items.begin()+numberOfAppetiser, new MainCourse(separated[1], separated[3], separated[2]));
                    numberOfMainCourse++;
					break;
				case 'b':
					items.push_back(new Beverage(separated[1], separated[3], separated[2], separated[6], separated[7]));
					break;
				default:
					throw "Unexpected input in given csv file";
			}
		}
        numberOfBeverage = items.size() - numberOfAppetiser - numberOfMainCourse;
	}
	string toString() {
		string menu = "\t\tMenu:\n";
        menu += "-------------Appetisers--------------\n";
        for (int i=0; i<numberOfAppetiser; i++)
            menu += "(" + to_string(i+1) + ") " + items[i]->toString() + "\n";
        menu += "-------------Main Course-------------\n";
        for (int i=numberOfAppetiser; i<numberOfAppetiser+numberOfMainCourse; i++)
            menu += "(" + to_string(i+1) + ") " + items[i]->toString() + "\n";
        menu += "-------------Beverage----------------\n";
        for (int i=numberOfAppetiser+numberOfMainCourse; i<items.size(); i++)
            menu += "(" + to_string(i+1) + ") " + items[i]->toString() + "\n";
        return menu;
        /*
		for (int i=0; i<items.size(); i++){
			//cout << '(' << i+1 << ") " << items[i]->getName() << ": £" << items[i]->getPrice() << ", " << items[i]->getCalories() << endl;
			cout << '(' << i+1 << ") ";
			cout << items[i]->toString() << endl;
		}
        */
	}
};

class Order: public ItemList {
private:
	float total;
	vector<float> prices2_4_1;
	int two4one;
public:
	Order(){
		total = 0;
	};
	void add(Item* choice){
		items.push_back(choice);
	}
	void remove(Item* choice){
		vector<Item*>::iterator i = find(items.begin(), items.end(), choice);
		items.erase(i);
	}
	void remove(int i){
		if (i > items.size() || i <= 0) throw "Number of item is out of the range"; 
		items.erase(items.begin()+i-1);
	}
	void calculateTotal(){
		two4one = 0; // counter of items with 2-4-1 offer
		total = 0;
		prices2_4_1.clear();
		for (Item* item: items){
			total += item->getPrice();
			if (item->isTwoForOne()){
				two4one++;
				prices2_4_1.push_back(item->getPrice());
			}
		}
		// rearrange array so first floor(two4one/2) elements are the smallest
		// floor(two4one/2) since we apply for each second 2-4-1 item the discount
		partial_sort(prices2_4_1.begin(), prices2_4_1.begin() + two4one/2, prices2_4_1.end());
		for (int i=0; i<two4one/2; i++)
			total -= prices2_4_1[i];
	}
	void printReceipt(){
		string address = "receipt.txt";
		ofstream receipt;
		receipt.open(address);
		receipt << toString() << endl; 
		receipt.close();
	}
	string toString(){
		string order = "";
        for (int i=0; i<items.size(); i++)
            order += "(" + to_string(i+1) + ") " + items[i]->toString() + "\n";
		calculateTotal();
		int saved = 0;
		for (int i=0; i<two4one/2; i++)
			saved += prices2_4_1[i];
		if (saved != 0)
			order += "2-4-1 discount applied! Savings: £" + to_string(saved) + "\n";
		order += "Total: £" + to_string(total);
		return order;
	}
};

int main(){
	Menu menu ("/Users/venka/Desktop/Take-away food ordering system/menu.csv");
	cout << menu.toString();
	Item* item = menu.getItem(3);
}