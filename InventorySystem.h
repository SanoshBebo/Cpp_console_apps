// InventorySystem.h

#ifndef INVENTORY_SYSTEM_H
#define INVENTORY_SYSTEM_H

#include <vector>
#include <map>
#include <string>
#include <regex>
#include <limits>
#include "Item.h"

class InventorySystem {
private:
    vector<Item> items;
    map<string, vector<Item>> itemsByCategory;

    // Private member functions
    bool fileExists(const string &filename);
    bool containsOnlyAlphabets(const string &str);
    bool containsOnlyDigits(const string &str);
    string validateInformation(const string &msg, const string &dataType, const string &val);
    void createCSVFile(const string &filename);
    void readItemsFromFile();
    void writeItemsToFile();
    string ValidEmail();
    string tolowercase(string &input);

public:
    InventorySystem(); // Constructor
    void displayAvailableItems();
    void addNewItem();
    void editItem();
    void deleteItem();
    void paymentProcess(int price);
    void purchaseItem();
    bool isValidDate(const string date);
    bool isDateWithin30Days(const string &dateToCheck);
    void getInvoice();
    void adminAction();
    void operatorAction();
    bool getAnswerFromUser(const string &msg);
};

#endif // INVENTORY_SYSTEM_H
