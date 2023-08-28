// InventorySystem.cpp
#include "InventorySystem.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>
#include <ctime>
#include <sys/stat.h>
#include <string>

using namespace std;

InventorySystem::InventorySystem()
{
    readItemsFromFile();
}

bool InventorySystem::fileExists(const string &filename)
{
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}
bool InventorySystem::containsOnlyAlphabets(const string &str)
{
    regex pattern("^[[:alpha:]]+(\\s+[[:alpha:]]+)*\\s*$");
    return regex_match(str, pattern);
}

bool InventorySystem::containsOnlyDigits(const string &str)
{
    regex pattern("^[[:digit:]]*$");
    return regex_match(str, pattern);
}

string InventorySystem::validateInformation(const string &msg, const string &dataType, const string &val)
{
    string info;
    cout << msg;
    getline(cin, info);
    if (dataType == "string")
    {
        if (!containsOnlyAlphabets(info))
        {
            cin.clear();
            cout << "Error Invalid Input, Try Again!" << endl;
            return validateInformation(msg, dataType, val);
        }
    }
    else if (dataType == "number")
    {
        if (!containsOnlyDigits(info) || val == "pin" && info.length() != 4 || val == "card" && info.length() != 16)
        {
            cin.clear();
            cout << "Error Invalid Input, Try Again!" << endl;
            return validateInformation(msg, dataType, val);
        }
    }
    return info;
}

void InventorySystem::createCSVFile(const string &filename)
{
    ofstream file(filename);
    file.close();
}

void InventorySystem::readItemsFromFile()
{
    ifstream stockFile("stock.csv");
    if (!stockFile)
    {
        cout << "No items available." << endl;
        return;
    }
    items.clear();
    string line;
    while (getline(stockFile, line))
    {
        stringstream ss(line);
        int id, quantity;
        double rate;
        string name, description, category;
        char comma;
        ss >> id >> comma;
        getline(ss, name, ',');
        getline(ss, description, ',');
        getline(ss, category, ',');
        ss >> quantity >> comma;
        ss >> rate;
        Item newItem(id, name, description, category, quantity, rate);
        items.push_back(newItem);
    }
    stockFile.close();
}

void InventorySystem::writeItemsToFile()
{
    ofstream stockFile("stock.csv");
    for (const Item &item : items)
    {
        stockFile << item.toCSVString() << endl;
    }
    stockFile.close();
}
void InventorySystem::adminAction()
{
    cout << "Admin action" << endl;
    string stockFileName = "stock.csv";
    if (!fileExists(stockFileName))
    {
        createCSVFile(stockFileName);
        cout << "Stock.csv file created." << endl;
    }
    readItemsFromFile();
    bool lowStockAlertShown = false;
    while (true)
    {
        if (!lowStockAlertShown)
        {
            // Check for low stock products and display warning message
            for (const Item &item : items)
            {
                if (item.quantity < 50)
                {
                    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
                    cout << "Admin: Some products have low stock:\n";
                    cout << "Product: '" << item.name << "' | Current stock: " << item.quantity << endl;
                    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
                    lowStockAlertShown = true;
                }
            }
        }
        cout << "Choose an action:" << endl;
        cout << "1. Display Available Items" << endl;
        cout << "2. Add New Item" << endl;
        cout << "3. Edit Item" << endl;
        cout << "4. Delete Item" << endl;
        cout << "5. Logout" << endl;
        cout << "6. Exit" << endl;
        int choice;
        choice = stoi(validateInformation("Enter your choice: ", "number", ""));
        switch (choice)
        {
        case 1:
            displayAvailableItems();
            break;
        case 2:
            addNewItem();
            break;
        case 3:
            editItem();
            break;
        case 4:
            deleteItem();
            break;
        case 5:
            return; // Logout
        case 6:
            cout << "Thanks for using the application!" << endl;
            exit(0); // Exit
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

void InventorySystem::addNewItem()
{
    int id, quantity;
    string name, description, category, inp;
    double rate;
    int maxId = 0;
    for (const Item &item : items)
    {
        if (item.id > maxId)
        {
            maxId = item.id;
        }
    }
    id = maxId + 1;
    cout << "Enter Name: ";
    getline(cin, name);
    cout << "Enter Description: ";
    getline(cin, description);
    quantity = stoi(validateInformation("Enter Quantity: ", "number", ""));
    category = validateInformation("Enter Category: ", "string", "");
    rate = stod(validateInformation("Enter Rate: ", "number", ""));
    Item newItem(id, name, description, category, quantity, rate);
    items.push_back(newItem);
    writeItemsToFile();
    cout << "Item added to stock successfully!" << endl;
}

void InventorySystem::editItem()
{
        int idToEdit;
        int editOption;
    edit:
        displayAvailableItems();
        idToEdit = stoi(validateInformation("Enter ID of item to edit: ","number", ""));
        bool found = false;
        for (Item &item : items)
        {
            if (item.id == idToEdit)
            {
            sameId:
                editOption = stoi(validateInformation("Choose the option to edit:\n1.name\n2.description\n3.category\n4.quantity\n5.rate\n", "number", ""));

                cout << "Editing item with ID: " << idToEdit << endl;
                // Prompt user for updated item details
                string newName, newDescription, newCategory, inp;
                int newQuantity;
                double newRate;
                switch (editOption)
                {
                case (1):
                    cout << "Enter new Name: ";
                    getline(cin, newName);
                    item.name = newName;
                    writeItemsToFile();
                    cout << "Item updated successfully!" << endl;
                    found = true;
                    break;
                case 2:
                    cout << "Enter new Description: ";
                    getline(cin, newDescription);
                    item.description = newDescription;
                    writeItemsToFile();
                    cout << "Item updated successfully!" << endl;
                    found = true;
                    break;
                case 3:
                    newCategory = validateInformation("Enter new category: ", "string", "");
                    item.category = newCategory;
                    writeItemsToFile();
                    cout << "Item updated successfully!" << endl;
                    found = true;
                    break;
                case 4:
                    newQuantity = stoi(validateInformation("Enter new Quantity: ", "number", ""));
                    item.quantity = newQuantity;
                    writeItemsToFile();
                    cout << "Item updated successfully!" << endl;
                    found = true;
                    break;

                case 5:

                    newRate = stoi(validateInformation("Enter new Rate: ", "number", ""));
                    item.rate = newRate;
                    writeItemsToFile();
                    cout << "Item updated successfully!" << endl;
                    found = true;
                    break;

                default:
                    cout << "Invalid input. Enter valid number \n";
                    goto sameId;
                }

                if (getAnswerFromUser("do you want to continue editing other items? "))
                {
                    if (getAnswerFromUser("do you want to edit in the same id? "))
                    {
                        goto sameId;
                    }
                    else
                    {
                        goto edit;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        if (!found)
        {
            cout << "No item found with the given ID." << endl;
        }
    }

void InventorySystem::deleteItem()
{
    int idToDelete;
    displayAvailableItems();
    idToDelete = stoi(validateInformation("Enter ID of item to delete: ", "number", ""));
    auto itemIter = items.begin();
    bool found = false;
    while (itemIter != items.end())
    {
        if (itemIter->id == idToDelete)
        {
            itemIter = items.erase(itemIter);
            writeItemsToFile();
            cout << "Item deleted successfully!" << endl;
            found = true;
            break;
        }
        else
        {
            ++itemIter;
        }
    }
    if (!found)
    {
        cout << "No item found with the given ID." << endl;
    }
}
