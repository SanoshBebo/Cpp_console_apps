// Item.h

#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <sstream>
 
using namespace std;

class Item {
public:
    int id;
    string name;
    string category;
    string description;
    int quantity;
    double rate;
    Item(int itemId, const string &itemName, const string &itemDescription,
         const string &itemCategory, int itemQuantity, double itemRate)
        : id(itemId), name(itemName), description(itemDescription),
          category(itemCategory), quantity(itemQuantity), rate(itemRate) {}

    string toCSVString() const {
        stringstream ss;
        ss << id << "," << name << "," << description << "," << category << ","
           << quantity << "," << rate;
        return ss.str();
    }
};

#endif // ITEM_H
