#include "Menu.hpp"
#include "Order.hpp"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


int main(){
	Menu menu ("/Users/venka/Desktop/Take-away food ordering system/menu.csv");
	cout << menu.toString();
	
}