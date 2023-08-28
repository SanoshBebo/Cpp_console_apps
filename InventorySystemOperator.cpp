// InventorySystem.cpp

#include "InventorySystem.h" // Include the operator-specific class definition
#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>
#include <ctime>
#include <string>

using namespace std;


bool InventorySystem::isDateWithin30Days(const string &dateToCheck)
{
    time_t currentTime = time(nullptr);
    tm *localTime = localtime(&currentTime);

    int currentYear = localTime->tm_year + 1900;
    int currentMonth = localTime->tm_mon + 1;
    int currentDay = localTime->tm_mday;

    int targetYear, targetMonth, targetDay;
    sscanf(dateToCheck.c_str(), "%d/%d/%d", &targetDay, &targetMonth, &targetYear);

    // Convert the target date and the current date to days since year 0
    int targetDays = targetDay + 30 * targetMonth + 365 * targetYear;
    int currentDays = currentDay + 30 * currentMonth + 365 * currentYear;

    if (currentDays - targetDays <= 30)
    {
        return true; // Date is within 30 days
    }
    else
    {
        return false; // Date is more than 30 days in the future
    }
}

void InventorySystem::displayAvailableItems()
{
    if (items.empty())
    {
        cout << "No items available." << endl;
        return;
    }
    itemsByCategory.clear();
    // Group and sort items by category
    for (const Item &item : items)
    {
        itemsByCategory[item.category].push_back(item);
    }
    // Sort categories
    vector<string> sortedCategories;
    for (const auto &entry : itemsByCategory)
    {
        sortedCategories.push_back(entry.first);
    }
    sort(sortedCategories.begin(), sortedCategories.end());
    // Display items by category
    cout << "Available Items:" << endl;
    for (const string &category : sortedCategories)
    {
        const vector<Item> &categoryItems = itemsByCategory[category];
        cout << "Category: " << category << endl;
        for (const Item &item : categoryItems)
        {
            cout << "ID: " << item.id << " | Name: " << item.name << " | Description: " << item.description << " | Category: " << item.category
                 << " | Quantity: " << item.quantity << " | Rate: " << item.rate << endl;
        }
    }
}
string InventorySystem::ValidEmail()
{
    string UPI;
    cout << "Enter UPI ID: ";
    getline(cin, UPI);
    const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    while (!regex_match(UPI, pattern))
    {
        cin.clear();
        cout << "Invalid UPI ID, Try again: " << endl;
        getline(cin, UPI);
    }
    return UPI;
}
string InventorySystem::tolowercase(string &input)
{
    for (int i = 0; i < input.length(); i++)
    {
        input[i] = tolower(input[i]);
    }
    return input;
}
bool InventorySystem::getAnswerFromUser(const string &msg)
{
    string option;
    cout << msg << " yes/no : ";
    cin >> option;
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    tolowercase(option);
    while (option != "yes" && option != "no")
    {
        cout << "invalid input, try again" << endl;
        cout << msg;
        cin >> option;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }
    if (option == "yes")
        return 1;
    else
        return 0;
}
void InventorySystem::paymentProcess(int price)
{
    string paymentSelected;
    int pin, pinChecker = 4205;
    vector<string> payment{"gpay", "card", "cash"};
    for (string option : payment)
    {
        cout << option << endl;
    }
    paymentSelected = validateInformation("Select payment method: ", "string", "");
    tolowercase(paymentSelected);
    if (paymentSelected == "cash")
    {
        cout << "Pay " << price << " on delivery"<<endl;
    }
    else if (paymentSelected == "card")
    {
        string CardNo = validateInformation("Enter 16 digit card no: ", "number", "card");
        pin:
        string pinStr = validateInformation("Enter 4 digit pin: ", "number", "pin");
        pin = stoi(pinStr);

        if (pin == pinChecker)
        {
                cout << "transaction Successful" << endl;
    
        }
        else
        {
            cout << "Invalid Pin"<<endl;
            if (getAnswerFromUser("Try Again? "))
            {
                goto pin;
            }else{
                return paymentProcess(price);
            }
        }
    }
    else if (paymentSelected == "gpay")
    {
        string UpiId = ValidEmail();
        gpay:
        string pinStr = validateInformation("Enter 4 digit pin: ", "number", "pin");
        pin = stoi(pinStr);
        if (pin == pinChecker)
        {
            cout<<"transaction successful";
        }
        else
        {
            cout << "Invalid Pin"<<endl;
            if (getAnswerFromUser("Try Again? "))
            {
                goto gpay;
            }else{
                return paymentProcess(price);
            }
        }
    }
    else
    {
        cout << "Invalid payment method, select from list of options..."<<endl;
        return paymentProcess(price);
    }
}

void InventorySystem::purchaseItem()
{
    int idToPurchase;
    displayAvailableItems();
    idToPurchase = stoi(validateInformation("Enter ID of item to purchase: ", "number", ""));
    bool found = false;
    for (Item &item : items)
    {
        if (item.id == idToPurchase)
        {
            found = true;
            int purchaseQuantity;
            purchaseQuantity = stoi(validateInformation("Enter quantity to purchase: ", "number", ""));
            if (purchaseQuantity <= item.quantity && item.quantity != 0)
            {
                double totalPrice = item.rate * purchaseQuantity;
                const double discountThreshold = 10000.0;
                string couponCode = "";
                if (totalPrice > discountThreshold)
                {
                    cout << "Congratulations! You have received a coupon of 10% off on purchases above " << discountThreshold << "." << endl;

                    if (getAnswerFromUser("Do you want to use the coupon? "))
                    {
                        double discountAmount = totalPrice * 0.1;
                        totalPrice -= discountAmount;
                        couponCode = "10%OFF";
                    }
                }
                ofstream invoicesFile("invoices.csv", ios::app);
                if (!invoicesFile)
                {
                    cout << "Error opening invoices.csv" << endl;
                    return;
                }
                time_t currentTime = time(nullptr);
                tm *localTime = localtime(&currentTime);
                int currentYear = localTime->tm_year + 1900;
                int currentMonth = localTime->tm_mon + 1;
                int currentDay = localTime->tm_mday;
                invoicesFile << currentDay << "/" << currentMonth << "/" << currentYear << ","
                             << item.id << "," << item.name << "," << item.description << "," << item.category << "," << purchaseQuantity << "," << totalPrice << endl;
                invoicesFile.close();
                cout << "Total Price: " << totalPrice << " rs" << endl;
                paymentProcess(totalPrice);
                cout << "Items purchased!" << endl;
                if (!couponCode.empty())
                {
                    cout << "Coupon Code: " << couponCode << " applied." << endl;
                }
                // Update item quantity in stock.csv
                item.quantity -= purchaseQuantity;
                writeItemsToFile();
                cout << "Item quantity updated successfully!" << endl;
            }
            else
            {
                cout << "Purchase quantity not available. Only " << item.quantity << " left." << endl;
            }
            break; // No need to continue searching for the item
        }
    }
    if (!found)
    {
        cout << "Item does not exist." << endl;
    }
}
bool InventorySystem::isValidDate(const string date)
{
    regex datePattern("^(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/(\\d{4})$");
    return regex_match(date, datePattern);
}

void InventorySystem::getInvoice()
{
    string dateToCheck;
    cout << "Enter date to get invoice (dd/mm/yyyy): ";

    while (true)
    {
        cin >> dateToCheck;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        if (!isValidDate(dateToCheck))
        {
            cout << "Invalid date format. Please enter in dd/mm/yyyy format: " ;
        }
        else if (isDateWithin30Days(dateToCheck))
        {
            cout << "**************************************" << endl;
            cout << "*       Invoices within 30 days      *" << endl;
            cout << "**************************************" << endl;

            ifstream invoicesFile("invoices.csv");
            if (!invoicesFile)
            {
                cout << "No invoices available." << endl;
                return;
            }
            cout << "Invoices within 30 days of " << dateToCheck << ":" << endl;
            string line;
            while (getline(invoicesFile, line))
            {
                stringstream ss(line);
                string date, id, name, description, quantity, totalPrice, category;
                getline(ss, date, ',');
                getline(ss, id, ',');
                getline(ss, name, ',');
                getline(ss, description, ',');
                getline(ss, category, ',');
                getline(ss, quantity, ',');
                getline(ss, totalPrice, ',');
                // if (isDateWithin30Days(date))
                // {
                //     cout << "Date: " << date << " | ID: " << id << " | Name: " << name << "| Description: " << description << "| category: " << category
                //          << " | Quantity: " << quantity << " | Total Price: " << totalPrice << " rs" << endl;
                //}
                if (isDateWithin30Days(date))
                {
                    cout << "**************************************" << endl;
                    cout << "* Date:        " << date << endl;
                    cout << "* ID:          " << id << endl;
                    cout << "* Name:        " << name << endl;
                    cout << "* Description: " << description << endl;
                    cout << "* Category:    " << category << endl;
                    cout << "* Quantity:    " << quantity << endl;
                    cout << "* Total Price: " << totalPrice << " rs" << endl;
                }
            }
            invoicesFile.close();
            cout << "**************************************" << endl;
            break; // Exit the loop if the date is valid and within 30 days
        }

        else
        {
            cout << "Date is not within 30 days from the current date. Enter a valid date: ";
        }
    }
}

void InventorySystem::operatorAction()
{
    cout << "Operator action" << endl;
    while (true)
    {
        cout << "Choose an action:" << endl;
        cout << "1. Display Available Items" << endl;
        cout << "2. Purchase Item" << endl;
        cout << "3. Get Invoice" << endl;
        cout << "4. Logout" << endl;
        cout << "5. Exit" << endl;
        int choice;
        choice = stoi(validateInformation("Enter you choice: ", "number", ""));
        switch (choice)
        {
        case 1:
            displayAvailableItems();
            break;
        case 2:
            purchaseItem();
            break;
        case 3:
            getInvoice();
            break;
        case 4:
            cout << "Logged out." << endl;
            return; // Logout
        case 5:
            cout << "Thanks for using the application!" << endl;
            exit(0); // Exit
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}
