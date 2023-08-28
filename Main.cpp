#include "InventorySystem.h"
#include <iostream>
using namespace std;
int main() {
    InventorySystem system;
    while (true)
    {
        string userType;
        cout << "Enter user type (admin/operator/exit): ";
        getline(cin, userType);
        if (userType == "admin")
        {
            InventorySystem inventory;
            inventory.adminAction();
        }
        else if (userType == "operator")
        {
            InventorySystem inventory;
            inventory.operatorAction();
        }
        else if (userType == "exit")
        {
            cout << "Thanks for using the application!" << endl;
            exit(0);
        }
        else
        {
            cout << "Invalid user type." << endl;
        }
    }
    return 0;
}
