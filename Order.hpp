#include <iostream>
#include <fstream>
#include <vector>
#include "Item.hpp"
#pragma once

using namespace std;
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
		cout << choice->getName() << " was added to order" << endl;
	}
	void remove(int i){
		if (i > items.size() || i <= 0) throw "Number of item is out of the range"; 
		string name = (*(items.begin()+i-1))->getName();
		items.erase(items.begin()+i-1);
		cout << name << " was deleted from the order" << endl;
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
		string order = "===== Checkout =====\n";
        for (int i=0; i<numberOfEntries(); i++)
            order += "(" + to_string(i+1) + ") " + items[i]->toString() + "\n";
		calculateTotal();
		float saved = 0;
		for (int i=0; i<two4one/2; i++)
			saved += prices2_4_1[i];
		if (saved != 0)
			order += "2-4-1 discount applied! Savings: £" + to_string(saved) + "\n";
		order += "Total: £" + to_string(total);
		return order;
	}
	bool isEmpty(){ return items.empty(); }
};