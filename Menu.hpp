#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Item.hpp"
#pragma once

class Menu: public ItemList {
private:
    int numberOfAppetiser;
    int numberOfMainCourse;
    int numberOfBeverage;
	bool sorted = false;
	void setSorted(bool sorted){this->sorted = sorted;}
public:
	Menu(): ItemList(){
		numberOfAppetiser = 0;
		numberOfMainCourse = 0;
		numberOfBeverage = 0;
	}
	Menu(vector<Item*> item) : ItemList(item){}
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
			// add items to menu in the way that first appears appetisers, then main course and beverages in the end
			switch (line[0]){
				case 'a':
					// added to the beginning of the menu
                    items.insert(items.begin(), new Appetiser(separated[1], separated[3], separated[2], separated[4], separated[5]));
                    numberOfAppetiser++;
					break;
				case 'm':
					// added after the last appetiser
                    items.insert(items.begin()+numberOfAppetiser, new MainCourse(separated[1], separated[3], separated[2]));
                    numberOfMainCourse++;
					break;
				case 'b':
					// added to the end of menu
					items.push_back(new Beverage(separated[1], separated[3], separated[2], separated[6], separated[7]));
					break;
				default:
					throw "Unexpected input in given csv file";
			}
		}
        numberOfBeverage = numberOfEntries() - numberOfAppetiser - numberOfMainCourse;
	}
	string toString() {
		string menu = "\t\tMenu:\n";
		if (sorted){
			menu += "---------Sorted by the price---------\n";
			for (int i=0; i<numberOfEntries(); i++)
				menu += "(" + to_string(i+1) + ") " + items[i]->toString() + "\n";
		}
		else {
			menu += "-------------Appetisers--------------\n";
			for (int i=0; i<numberOfAppetiser; i++)
				menu += "(" + to_string(i+1) + ") " + items[i]->toString() + "\n";
			menu += "-------------Main Course-------------\n";
			for (int i=numberOfAppetiser; i<numberOfAppetiser+numberOfMainCourse; i++)
				menu += "(" + to_string(i+1) + ") " + items[i]->toString() + "\n";
			menu += "-------------Beverage----------------\n";
			for (int i=numberOfAppetiser+numberOfMainCourse; i<numberOfEntries(); i++)
				menu += "(" + to_string(i+1) + ") " + items[i]->toString() + "\n";
		}
		return menu;
	}
	void sortMenu(bool increasing) {
		if (increasing) sort(items.begin(), items.end(), Item::isSmaller);
		else sort(items.begin(), items.end(), Item::isGreater);
	}
	Menu sortedMenu(bool increasing){
		Menu sorted (*this);
		sorted.sortMenu(increasing);
		sorted.setSorted(true);
		return sorted;
	}
};
