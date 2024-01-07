#include "Menu.hpp"
#include "Order.hpp"
#include "Item.hpp"

#include <iostream>
#include <vector>
#include <cstring>
#include <set>

using namespace std;
void printInstructions(){
    cout << "menu - display the menu" << endl;
    cout << "add [INDEXES] - add an item to the order by numeric index in the menu (starting at 1)" << endl;
    cout << "remove [INDEXES] - remove item from order by numeric index in the order (starting at 1)" << endl;
    cout << "checkout - display the items in the order, the price, and discount savings" << endl;
    cout << "help - display a help menu for the user with the available options" << endl;
    cout << "exit - terminate the program gracefully" << endl;
}
int main()
{
    string userCommand;
    vector <string> parameters;
    
    Menu menu = Menu("menu.csv");  // Create a menu object from a CSV file

    Order order = Order();  // Create an order object
    // some temporary variables used further in execution
    int tmp;
    string answer;
    string command;

	printInstructions();
    while (userCommand != "exit")
    {
        userCommand.clear();
        parameters.clear();
        command.clear();
        cout << "Your command: ";
        getline(cin, userCommand);
        char* cstr = new char[userCommand.length() + 1]; 
        strcpy(cstr, userCommand.c_str());
        tmp = 0;

        char* token;
        token = strtok(cstr, " ");
        // add word(separated by blank space) written by user to the vector of strings
        while (token != NULL)
        {
            parameters.push_back(token);
            token = strtok(NULL, " ");
        }
        if (!parameters.empty()) command = parameters[0];
        
        if (command.compare("menu") == 0) {
            cout << menu.toString();
        }
        else if (command.compare("add") == 0)
        {
			int n = parameters.size();
			for (int i=1; i<n; i++){
                // for each number given by user add menu entry with corresponding number to the user's order
                tmp = stoi(parameters[i]);
                if (tmp > menu.numberOfEntries() || tmp < 1)
                    cout << "There is no item in order with number " << tmp << endl;
				else {
                    Item* choice = menu.getItem(tmp);
				    order.add(choice);
                }
			}
        }
        else if (command.compare("remove") == 0)
        {
            // for each number given by user remove from order the entry with corresponding number
            if (order.isEmpty())
                cout << "You have not ordered anything. Add items from menu to your order" << endl;
            else {
                int n = parameters.size();
                // decreasing order is used to avoid problems with item number
                // calling remove 1 2, after removing item 1, item that before had number 2, now will be first, 
                // thus removing item with number 2 will actually remove entry with original number 3
                // however, removing from the back avoids this problem
                // additionally it is faster with time complexity O(n*log(n))
                set<int, greater<int>> removeIDs;
                for (int i=1; i<n; i++)
                    removeIDs.insert(stoi(parameters[i]));
                for (int id: removeIDs){
                    if (id > order.numberOfEntries() || id < 1)
                        cout << "There is not item in your order with number " << id << endl;
                    else order.remove(id);
                }
            }
        }
        else if (command.compare("checkout") == 0)
        {
            answer.clear();
			if (order.isEmpty())
				cout << "Your order is empty, add some items" << endl;
			else {
				cout << order.toString() << endl;
				while (!(answer == "y" || answer == "n")){
					cout << "Do you want to place your order?" << endl;
					cout << "Type 'y' to confirm, or 'n' to go back and modify it. ";
					cin >> answer;
				}
				if (answer == "y") {
                    order.printReceipt(); // save user's order to file
                    cout << "Receipt has been saved in file" << endl;
                    userCommand = "exit"; // since order is placed, the program should end as nothing else to be done
                }
                cin.ignore();
			}
        }
        else if (command.compare("help") == 0){ printInstructions(); }
        else cout << "There is no such command" << endl;
    }
    cout << "Thank you for using our service. Goodbye" << endl;
    cout << "Press enter to quit...";
    getchar();

}
