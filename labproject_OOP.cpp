#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <windows.h>
#include <sstream>
#include <map>
#include<vector>
using namespace std;

/* ------------------------------
      COLOR FUNCTIONS
   ------------------------------*/
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void resetColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void clearScreen() {
    system("cls"); 
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

/* ------------------------------
   Base User Class
   ------------------------------*/
class User {
protected:
    string username, password, role;

public:
    User() {}
    User(string u, string p, string r): username(u), password(p), role(r) {}

    string getRole() { return role; }
    string getUsername() { return username; }

    virtual void displayPortalInfo() {
        setColor(14);
        cout << "\nLogged in as: " << username << " (" << role << ")\n";
        resetColor();
    }

    virtual ~User() {}

    static User login() {
        string u, p;

        setColor(11);
        cout << "\n=== LOGIN ===\n";
        resetColor();

        setColor(10);
        cout << "Username: ";
        resetColor();
        cin >> u;

        setColor(10);
        cout << "Password: ";
        resetColor();
        cin >> p;

        ifstream fin("users.txt");
        string fileU, fileP, fileR;

        while (fin >> fileU >> fileP >> fileR) {
            if (fileU == u && fileP == p) {
                setColor(10);
                cout << "\nLogin Successful (" << fileR << ")\n";
                resetColor();
                return User(fileU, fileP, fileR);
            }
        }

        setColor(12);
        cout << "Invalid credentials!\n";
        resetColor();
        return User("", "", "");
    }
};

/* ------------------------------
   Admin Class
   ------------------------------*/
class Admin : public User {
public:
    Admin() : User() {}
    Admin(string u, string p) : User(u, p, "admin") {}

    void displayPortalInfo() override {
        setColor(13);
        cout << "\n+------------------------------------+\n";
        cout << "|           ADMIN PORTAL            |\n";
        cout << "|  User: " << left << setw(28) << username << "|\n";
        cout << "+------------------------------------+\n";
        resetColor();
    }
};

/* ------------------------------
   Staff Class
   ------------------------------*/
class Staff : public User {
public:
    Staff() : User() {}
    Staff(string u, string p) : User(u, p, "staff") {}

    void displayPortalInfo() override {
        setColor(11);
        cout << "\n+------------------------------------+\n";
        cout << "|           STAFF PORTAL            |\n";
        cout << "|  User: " << left << setw(28) << username << "|\n";
        cout << "+------------------------------------+\n";
        resetColor();
    }
};

/* ------------------------------
        FOOD ITEM CLASS
   ------------------------------*/
class FoodItem {
public:
    int id;
    string name;
    int price;

    FoodItem() {}
    FoodItem(int i, string n, int p): id(i), name(n), price(p) {}
};

/* ------------------------------
     MENU MANAGER
   ------------------------------*/
class MenuManager {
public:
    void addItem() {
        clearScreen();
        
        setColor(11);
        cout << "\n+------------------------------------+\n";
        cout << "|           ADD MENU ITEM           |\n";
        cout << "+------------------------------------+\n";
        resetColor();
        
        ofstream fout("menu.txt", ios::app);
        int id, price;
        string name, category;

        setColor(10);
        cout << "\nAdding New Menu Item...\n";
        resetColor();

        setColor(11);
        cout << "Enter ID: ";
        resetColor();
        cin >> id;

        setColor(11);
        cout << "Enter Category (Starters, Main Course, Desserts, Specialities, Beverages, Add Ons): ";
        resetColor();
        cin.ignore();
        getline(cin, category);

        setColor(11);
        cout << "Enter Name: ";
        resetColor();
        getline(cin, name);

        setColor(11);
        cout << "Enter Price: ";
        resetColor();
        cin >> price;

        fout << id << "," << category << "," << name << "," << price << endl;
        fout.close();

        setColor(10);
        cout << "\n+------------------------------------+\n";
        cout << "|      ITEM ADDED SUCCESSFULLY!     |\n";
        cout << "+------------------------------------+\n";
        resetColor();
        pauseScreen();
    }

    void updateItem() {
        clearScreen();
        
        setColor(14);
        cout << "\n+------------------------------------+\n";
        cout << "|          UPDATE MENU ITEM         |\n";
        cout << "+------------------------------------+\n";
        resetColor();
        
        int id;

        setColor(14);
        cout << "\nEnter ID to update: ";
        resetColor();
        cin >> id;

        ifstream fin("menu.txt");
        ofstream fout("temp.txt");

        string line;
        bool found = false;

        while (getline(fin, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string idStr, category, name, priceStr;

            getline(ss, idStr, ',');
            getline(ss, category, ',');
            getline(ss, name, ',');
            getline(ss, priceStr);

            int fid = stoi(idStr);
            int price = stoi(priceStr);

            if (fid == id) {
                found = true;

                setColor(11);
                cout << "Current Category: " << category << endl;
                cout << "Current Name: " << name << endl;
                cout << "Current Price: " << price << endl;
                resetColor();

                cout << "\nEnter new details:\n";
                
                setColor(11);
                cout << "New Category: ";
                resetColor();
                cin.ignore();
                getline(cin, category);

                setColor(11);
                cout << "New Name: ";
                resetColor();
                getline(cin, name);

                setColor(11);
                cout << "New Price: ";
                resetColor();
                cin >> price;
            }

            fout << fid << "," << category << "," << name << "," << price << endl;
        }

        fin.close();
        fout.close();

        remove("menu.txt");
        rename("temp.txt", "menu.txt");

        if (found) {
            setColor(10);
            cout << "\n+------------------------------------+\n";
            cout << "|          ITEM UPDATED!            |\n";
            cout << "+------------------------------------+\n";
        } else {
            setColor(12);
            cout << "\n+------------------------------------+\n";
            cout << "|          ITEM NOT FOUND!          |\n";
            cout << "+------------------------------------+\n";
        }
        resetColor();
        pauseScreen();
    }

    void removeItem() {
        clearScreen();
        
        setColor(12);
        cout << "\n+------------------------------------+\n";
        cout << "|          REMOVE MENU ITEM         |\n";
        cout << "+------------------------------------+\n";
        resetColor();
        
        int id;
        setColor(12);
        cout << "\nEnter ID to remove: ";
        resetColor();
        cin >> id;

        ifstream fin("menu.txt");
        ofstream fout("temp.txt");

        string line;
        bool found = false;

        while (getline(fin, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string idStr, category, name, priceStr;

            getline(ss, idStr, ',');
            getline(ss, category, ',');
            getline(ss, name, ',');
            getline(ss, priceStr);

            int fid = stoi(idStr);

            if (fid != id) {
                fout << line << endl;
            } else {
                found = true;
                setColor(12);
                cout << "Removing: " << name << " (ID: " << id << ")\n";
                resetColor();
            }
        }

        fin.close();
        fout.close();

        remove("menu.txt");
        rename("temp.txt", "menu.txt");

        if (found) {
            setColor(10);
            cout << "\n+------------------------------------+\n";
            cout << "|          ITEM REMOVED!            |\n";
            cout << "+------------------------------------+\n";
        } else {
            setColor(12);
            cout << "\n+------------------------------------+\n";
            cout << "|          ITEM NOT FOUND!          |\n";
            cout << "+------------------------------------+\n";
        }
        resetColor();
        pauseScreen();
    }

    void viewMenu() {
        clearScreen();
        
        setColor(11);
        cout << "\n+------------------------------------+\n";
        cout << "|           VIEW FULL MENU          |\n";
        cout << "+------------------------------------+\n";
        resetColor();
        
        ifstream fin("menu.txt");
        if (!fin) {
            setColor(12);
            cout << "Menu file not found!\n";
            resetColor();
            pauseScreen();
            return;
        }

        map<string, vector<FoodItem>> categorizedMenu;

        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string idStr, category, name, priceStr;

            getline(ss, idStr, ',');
            getline(ss, category, ',');
            getline(ss, name, ',');
            getline(ss, priceStr);

            category.erase(0, category.find_first_not_of(" \t\r\n"));
            category.erase(category.find_last_not_of(" \t\r\n") + 1);
            name.erase(0, name.find_first_not_of(" \t\r\n"));
            name.erase(name.find_last_not_of(" \t\r\n") + 1);

            int id = stoi(idStr);
            int price = stoi(priceStr);

            categorizedMenu[category].push_back(FoodItem(id, name, price));
        }
        fin.close();

        setColor(14);
        cout << "\n==================== MENU ====================\n";
        resetColor();

        vector<string> categoryOrder = {"Starters", "Main Course", "Desserts", "Specialities", "Beverages", "Add Ons"};

        for (const string& category : categoryOrder) {
            if (categorizedMenu.find(category) != categorizedMenu.end()) {
                vector<FoodItem>& items = categorizedMenu[category];

                setColor(13);
                cout << "\n--- " << category << " ---\n";
                resetColor();

                setColor(11);
                cout << left << setw(6) << "ID" 
                     << setw(30) << "Item Name" 
                     << setw(10) << "Price (Rs)" << endl;
                resetColor();

                cout << "----------------------------------------------\n";

                for (auto& item : items) {
                    cout << left << setw(6) << item.id 
                         << setw(30) << item.name 
                         << setw(10) << item.price << endl;
                }
            }
        }

        setColor(14);
        cout << "\n==============================================\n";
        cout << "Total Categories: " << categorizedMenu.size() << endl;
        cout << "==============================================\n";
        resetColor();
        pauseScreen();
    }

    void searchItem() {
        clearScreen();
        
        setColor(13);
        cout << "\n+------------------------------------+\n";
        cout << "|           SEARCH MENU ITEM        |\n";
        cout << "+------------------------------------+\n";
        resetColor();
        
        string query;

        setColor(13);
        cout << "\nEnter part of food name to search: ";
        resetColor();

        cin.ignore();
        getline(cin, query);

        ifstream fin("menu.txt");
        string line;

        setColor(14);
        cout << "\n--- Search Results ---\n";
        resetColor();

        bool found = false;
        while (getline(fin, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string idStr, category, name, priceStr;

            getline(ss, idStr, ',');
            getline(ss, category, ',');
            getline(ss, name, ',');
            getline(ss, priceStr);

            if (name.find(query) != string::npos) {
                found = true;
                int id = stoi(idStr);
                int price = stoi(priceStr);
                setColor(10);
                cout << "[FOUND] ";
                resetColor();
                cout << "ID: " << id << " | " << name << " | Rs." << price << " | Category: " << category << endl;
            }
        }
        fin.close();
        
        if (!found) {
            setColor(12);
            cout << "No items found matching: '" << query << "'\n";
            resetColor();
        }
        
        pauseScreen();
    }
};

/* ------------------------------
        ORDER MANAGEMENT
   ------------------------------*/
class OrderSystem {
public:
    int placeOrder(string customerID) {
        clearScreen();
        
        setColor(10);
        cout << "\n+------------------------------------+\n";
        cout << "|            PLACE ORDER            |\n";
        cout << "|  Customer: " << left << setw(24) << customerID << "|\n";
        cout << "+------------------------------------+\n";
        resetColor();
        
        ifstream fin("menu.txt");
        string line;

        setColor(14);
        cout << "\n===================== MENU =====================\n";
        resetColor();
        
        cout << left << setw(6) << "ID" << " | "
             << setw(30) << "Item Name" << " | "
             << setw(10) << "Price (Rs)" << endl;
        cout << "------------------------------------------------\n";

        while (getline(fin, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string idStr, category, name, priceStr;

            getline(ss, idStr, ',');
            getline(ss, category, ',');
            getline(ss, name, ',');
            getline(ss, priceStr);

            int id = stoi(idStr);
            int price = stoi(priceStr);

            cout << left << setw(6) << id << " | "
                 << setw(30) << name << " | "
                 << setw(10) << price << endl;
        }
        fin.close();
        
        cout << "================================================\n";

        int choice, qty, total = 0;

        time_t now = time(0);
        stringstream orderIDss;
        orderIDss << now;
        string orderID = orderIDss.str();

        string custOrderFile = "current_customer_orders_" + customerID + ".txt";
        ofstream custOrderOut(custOrderFile);
        custOrderOut << "ORDERID:" << orderID << " USER:" << customerID << "\n";

        ofstream globalOrders("orders.txt", ios::app);
        globalOrders << "ORDERID:" << orderID << " USER:" << customerID << "\n";

        while (true) {
            setColor(11);
            cout << "\nEnter food ID (0 to finish): ";
            resetColor();
            cin >> choice;
            if (choice == 0) break;

            setColor(11);
            cout << "Enter quantity: ";
            resetColor();
            cin >> qty;

            ifstream menuFile("menu.txt");
            string menuLine;
            bool found = false;
            while (getline(menuFile, menuLine)) {
                if (menuLine.empty()) continue;

                stringstream ss(menuLine);
                string idStr, category, name, priceStr;
                
                getline(ss, idStr, ',');
                getline(ss, category, ',');
                getline(ss, name, ',');
                getline(ss, priceStr);

                int fid = stoi(idStr);
                int fprice = stoi(priceStr);

                if (fid == choice) {
                    int cost = qty * fprice;
                    total += cost;
                    string orderLine = name + " x" + to_string(qty) + " = " + to_string(cost);
                    custOrderOut << orderLine << "\n";
                    globalOrders << orderLine << "\n";
                    found = true;
                    
                    setColor(10);
                    cout << ">>> Added: " << name << " x" << qty << " = Rs." << cost << endl;
                    resetColor();
                    break;
                }
            }
            menuFile.close();

            if (!found) {
                setColor(12);
                cout << ">>> Invalid ID! Please try again.\n";
                resetColor();
            }
        }

        string paymentMethod;
        int discount = 0;
        int finalTotal = total;

        setColor(11);
        cout << "\n--- PAYMENT METHOD ---\n";
        resetColor();
        
        while (true) {
            cout << "Select payment method:\n";
            setColor(10);
            cout << "1. Cash\n";
            cout << "2. Card (10% discount)\n";
            resetColor();
            setColor(14);
            cout << "Enter choice (1 or 2): ";
            resetColor();
            
            int payChoice;
            cin >> payChoice;
            
            if (payChoice == 1) {
                paymentMethod = "Cash";
                break;
            } else if (payChoice == 2) {
                paymentMethod = "Card";
                discount = (total * 10) / 100;
                finalTotal = total - discount;
                
                setColor(10);
                cout << "\n*** Card payment selected! You get 10% discount! ***\n";
                resetColor();
                break;
            } else {
                setColor(12);
                cout << "Invalid choice! Please enter 1 or 2.\n";
                resetColor();
            }
        }

        custOrderOut << "SUBTOTAL = " << total << "\n";
        globalOrders << "SUBTOTAL = " << total << "\n";
        
        if (discount > 0) {
            custOrderOut << "DISCOUNT = " << discount << " (10% Card Discount)\n";
            globalOrders << "DISCOUNT = " << discount << " (10% Card Discount)\n";
        }
        
        custOrderOut << "TOTAL = " << finalTotal << "\n";
        custOrderOut << "PAYMENT_METHOD = " << paymentMethod << "\n\n";
        
        globalOrders << "TOTAL = " << finalTotal << "\n";
        globalOrders << "PAYMENT_METHOD = " << paymentMethod << "\n\n";

        custOrderOut.close();
        globalOrders.close();

        setColor(14);
        cout << "\n+------------------------------------+\n";
        cout << "|           BILL SUMMARY            |\n";
        cout << "+------------------------------------+\n";
        cout << "Subtotal:        Rs. " << total << "\n";
        if (discount > 0) {
            setColor(10);
            cout << "Discount (10%):  Rs. " << discount << "\n";
            setColor(14);
        }
        cout << "Final Total:     Rs. " << finalTotal << "\n";
        cout << "Payment Method:  " << paymentMethod << "\n";
        cout << "Order ID:        " << orderID << "\n";
        cout << "------------------------------------\n";
        resetColor();

        string paymentStatus;
        while (true) {
            setColor(11);
            cout << "\nIs payment done? (yes/no): ";
            resetColor();
            cin >> paymentStatus;
            if (paymentStatus == "yes" || paymentStatus == "no") break;
            cout << "Please enter 'yes' or 'no'.\n";
        }

        ofstream payOut("payments.txt", ios::app);
        payOut << orderID << " " << customerID << " " << paymentStatus << " " << paymentMethod << " " << finalTotal << "\n";
        payOut.close();

        if (paymentStatus == "yes") {
            setColor(10);
            cout << ">>> Payment recorded as PAID via " << paymentMethod << ".\n";
        } else {
            setColor(12);
            cout << ">>> Payment recorded as NOT PAID.\n";
        }
        resetColor();
        
        pauseScreen();
        return 0;
    }

    void viewOrders() {
        clearScreen();
        
        setColor(9);
        cout << "\n+------------------------------------+\n";
        cout << "|           VIEW ALL ORDERS         |\n";
        cout << "+------------------------------------+\n";
        resetColor();
        
        ifstream fin("orders.txt");

        if (!fin) {
            setColor(12);
            cout << "\nNo orders found!\n";
            resetColor();
            pauseScreen();
            return;
        }

        setColor(11);
        cout << "\n--- PREVIOUS ORDERS ---\n";
        resetColor();

        string line;
        bool printOrderHeader = false;
        int orderCount = 0;

        while (getline(fin, line)) {
            if (line.empty()) continue;

            if (line.find("ORDERID:") != string::npos) {
                orderCount++;
                setColor(14);
                cout << "\n" << line << "\n";
                resetColor();
                printOrderHeader = true;
            }
            else if (line.find("TOTAL =") != string::npos) {
                setColor(10);
                cout << line << "\n";
                resetColor();
                printOrderHeader = false;
            }
            else {
                if (printOrderHeader) {
                    cout << line << "\n";
                }
            }
        }

        if (orderCount == 0) {
            setColor(12);
            cout << "\nNo orders placed yet!\n";
            resetColor();
        } else {
            setColor(11);
            cout << "\nTotal Orders: " << orderCount << endl;
            resetColor();
        }

        fin.close();
        pauseScreen();
    }

    void viewPayments() {
        clearScreen();
        
        setColor(10);
        cout << "\n+------------------------------------+\n";
        cout << "|          PAYMENT STATUS           |\n";
        cout << "+------------------------------------+\n";
        resetColor();
        
        ifstream fin("payments.txt");
        if (!fin) {
            setColor(12);
            cout << "\nNo payment records found!\n";
            resetColor();
            pauseScreen();
            return;
        }

        setColor(14);
        cout << "\n===================== PAYMENT STATUS =====================\n";
        cout << left << setw(20) << "OrderID" 
             << setw(12) << "CustomerID" 
             << setw(10) << "Status"
             << setw(12) << "Method"
             << "Amount" << endl;
        cout << "----------------------------------------------------------\n";
        resetColor();

        string orderID, custID, status, method;
        int amount;
        int totalRecords = 0;
        int paidCount = 0;
        while (fin >> orderID >> custID >> status >> method >> amount) {
            if (status == "yes") {
                setColor(10);
                paidCount++;
            } else {
                setColor(12);
            }
            cout << left << setw(20) << orderID 
                 << setw(12) << custID 
                 << setw(10) << (status == "yes" ? "PAID" : "UNPAID")
                 << setw(12) << method
                 << "Rs. " << amount << endl;
            totalRecords++;
        }
        
        fin.close();
        setColor(14);
        cout << "----------------------------------------------------------\n";
        cout << "Total Records: " << totalRecords << " | Paid: " << paidCount << " | Unpaid: " << (totalRecords - paidCount) << endl;
        cout << "==========================================================\n";
        resetColor();
        pauseScreen();
    }
};

/* ------------------------------
        CUSTOMER CLASS
   ------------------------------*/
class Customer : public User {
private:
    string customerID;

public:
    Customer(string id) : User(id, "", "customer"), customerID(id) {}

    void displayPortalInfo() override {
        setColor(10);
        cout << "\n+------------------------------------+\n";
        cout << "|          CUSTOMER PORTAL          |\n";
        cout << "|  Customer ID: " << left << setw(22) << customerID << "|\n";
        cout << "+------------------------------------+\n";
        resetColor();
    }

    string getCustomerID() { return customerID; }

    void selectMenu() {
        clearScreen();
        setColor(11);
        cout << "\n+------------------------------------+\n";
        cout << "|            VIEW MENU              |\n";
        cout << "|  Customer: " << left << setw(24) << customerID << "|\n";
        cout << "+------------------------------------+\n";
        resetColor();
        
        MenuManager mm;
        mm.viewMenu();
    }

    void placeOrder() {
        OrderSystem os;
        os.placeOrder(customerID);
    }

    void viewLastOrder() {
        clearScreen();
        setColor(14);
        cout << "\n+------------------------------------+\n";
        cout << "|            LAST ORDER             |\n";
        cout << "|  Customer: " << left << setw(24) << customerID << "|\n";
        cout << "+------------------------------------+\n";
        resetColor();
        
        string custOrderFile = "current_customer_orders_" + customerID + ".txt";
        ifstream fin(custOrderFile);

        if (!fin) {
            setColor(12);
            cout << "\nNo previous order found!\n";
            resetColor();
            pauseScreen();
            return;
        }

        setColor(11);
        cout << "\n--- YOUR LAST ORDER ---\n";
        resetColor();

        string line;
        while (getline(fin, line)) {
            cout << line << endl;
        }
        fin.close();
        pauseScreen();
    }

    void viewReceipt() {
        clearScreen();
        setColor(14);
        cout << "\n+------------------------------------+\n";
        cout << "|            ORDER RECEIPT          |\n";
        cout << "|  Customer: " << left << setw(24) << customerID << "|\n";
        cout << "+------------------------------------+\n";
        resetColor();
        
        string custOrderFile = "current_customer_orders_" + customerID + ".txt";
        ifstream fin(custOrderFile);

        if (!fin) {
            setColor(12);
            cout << "\nNo receipt available! Place an order first!\n";
            resetColor();
            pauseScreen();
            return;
        }

        setColor(14);
        cout << "\n================================================\n";
        cout << "                 ZenZest\n";
        cout << "================================================\n";
        cout << "              ORDER RECEIPT\n";
        cout << "------------------------------------------------\n";
        resetColor();

        string line;
        int subtotal = 0;
        int discount = 0;
        int total = 0;
        string paymentMethod = "N/A";
        bool hasDiscount = false;

        while (getline(fin, line)) {
            if (line.find("ORDERID:") != string::npos) {
                cout << line << "\n";
                cout << "------------------------------------------------\n";
                setColor(11);
                cout << left << setw(28) << "Item" 
                     << setw(8) << "Qty" 
                     << "Amount" << endl;
                cout << "------------------------------------------------\n";
                resetColor();
            }
            else if (line.find("SUBTOTAL =") != string::npos) {
                subtotal = stoi(line.substr(11));
            }
            else if (line.find("DISCOUNT =") != string::npos) {
                hasDiscount = true;
                size_t pos = line.find("=");
                size_t endPos = line.find(" ", pos + 2);
                discount = stoi(line.substr(pos + 2, endPos - pos - 2));
            }
            else if (line.find("TOTAL =") != string::npos) {
                total = stoi(line.substr(8));
            }
            else if (line.find("PAYMENT_METHOD =") != string::npos) {
                paymentMethod = line.substr(17);
            }
            else if (line.find("x") != string::npos && line.find("=") != string::npos) {
                size_t posX = line.find("x");
                size_t posEq = line.find("=");

                string item = line.substr(0, posX - 1);
                string qty = line.substr(posX + 1, posEq - posX - 2);
                string amount = line.substr(posEq + 2);

                cout << left << setw(28) << item
                     << setw(8) << qty
                     << "Rs. " << amount << endl;
            }
        }

        fin.close();

        setColor(14);
        cout << "------------------------------------------------\n";
        cout << left << setw(36) << "SUBTOTAL:" << "Rs. " << subtotal << endl;
        
        if (hasDiscount) {
            setColor(10);
            cout << left << setw(36) << "DISCOUNT (10% Card):" << "- Rs. " << discount << endl;
            setColor(14);
        }
        
        cout << "================================================\n";
        cout << left << setw(36) << "GRAND TOTAL:" << "Rs. " << total << endl;
        cout << "================================================\n";
        cout << left << setw(36) << "Payment Method:" << paymentMethod << endl;
        
        if (hasDiscount) {
            setColor(10);
            cout << "\n*** You saved Rs. " << discount << " with card payment! ***\n";
            setColor(14);
        }
        
        cout << "================================================\n";
        cout << "        THANK YOU FOR DINING WITH US!\n";
        cout << "================================================\n\n";
        resetColor();
        pauseScreen();
    }

    void viewPayments() {
        clearScreen();
        setColor(11);
        cout << "\n+------------------------------------+\n";
        cout << "|          PAYMENT HISTORY          |\n";
        cout << "|  Customer: " << left << setw(24) << customerID << "|\n";
        cout << "+------------------------------------+\n";
        resetColor();
        
        ifstream fin("payments.txt");
        if (!fin) {
            setColor(12);
            cout << "\nNo payment records found!\n";
            resetColor();
            pauseScreen();
            return;
        }

        setColor(14);
        cout << "\n===================== PAYMENT STATUS =====================\n";
        cout << left << setw(20) << "OrderID" 
             << setw(12) << "CustomerID" 
             << setw(10) << "Status"
             << setw(12) << "Method"
             << "Amount" << endl;
        cout << "----------------------------------------------------------\n";
        resetColor();

        string orderID, custID, status, method;
        int amount;
        int userRecords = 0;
        while (fin >> orderID >> custID >> status >> method >> amount) {
            if (custID == customerID) { 
                userRecords++;
                if (status == "yes") {
                    setColor(10);
                } else {
                    setColor(12);
                }
                cout << left << setw(20) << orderID 
                     << setw(12) << custID 
                     << setw(10) << (status == "yes" ? "PAID" : "UNPAID")
                     << setw(12) << method
                     << "Rs. " << amount << endl;
            }
        }
        
        fin.close();
        
        if (userRecords == 0) {
            setColor(12);
            cout << "No payment records found for your account!\n";
            resetColor();
        }
        
        setColor(14);
        cout << "==========================================================\n";
        resetColor();
        pauseScreen();
    }

    void giveFeedback() {
        clearScreen();
        setColor(13);
        cout << "\n+------------------------------------+\n";
        cout << "|          FEEDBACK & RATING        |\n";
        cout << "|  Customer: " << left << setw(24) << customerID << "|\n";
        cout << "+------------------------------------+\n";
        resetColor();
        
        int rating;
        string feedback;

        setColor(11);
        cout << "\nWe value your feedback!\n";
        resetColor();

        while (true) {
            setColor(14);
            cout << "Rate your experience (1-5 stars): ";
            resetColor();
            cin >> rating;
            
            if (rating >= 1 && rating <= 5) {
                break;
            } else {
                setColor(12);
                cout << "Please enter a rating between 1 and 5!\n";
                resetColor();
            }
        }

        setColor(14);
        cout << "Would you like to leave a comment? (yes/no): ";
        resetColor();
        string choice;
        cin >> choice;

        if (choice == "yes" || choice == "Yes" || choice == "YES") {
            cin.ignore();
            setColor(11);
            cout << "Enter your feedback: ";
            resetColor();
            getline(cin, feedback);
        } else {
            feedback = "No comment";
        }

        ofstream fout("feedback.txt", ios::app);
        
        time_t now = time(0);
        string timestamp = ctime(&now);
        timestamp.pop_back(); 
        
        fout << "CustomerID: " << customerID << " | ";
        fout << "Rating: " << rating << " stars | ";
        fout << "Feedback: " << feedback << " | ";
        fout << "Date: " << timestamp << endl;
        
        fout.close();

        setColor(10);
        cout << "\n>>> Thank you for your feedback!\n";
        
        cout << "Your rating: ";
        for (int i = 0; i < rating; i++) {
            cout << "* ";
        }
        for (int i = rating; i < 5; i++) {
            cout << ". ";
        }
        cout << " (" << rating << "/5)\n";
        resetColor();
        pauseScreen();
    }
};

/* ------------------------------
      PORTALS FOR ROLES
   ------------------------------*/
void adminPortal() {
    MenuManager mm;
    OrderSystem os;

    while (true) {
        clearScreen();
        
        setColor(13);
        cout << "\n+------------------------------------+\n";
        cout << "|            ADMIN PORTAL           |\n";
        cout << "+------------------------------------+\n";
        resetColor();

        setColor(11);
        cout << "1. Add Menu Item\n";
        cout << "2. Update Menu Item\n";
        cout << "3. Remove Menu Item\n";
        cout << "4. View Menu\n";
        cout << "5. Search Menu\n";
        cout << "6. View Payment Status\n";
        cout << "7. View All Orders\n";
        cout << "8. View Customer Feedback\n";
        cout << "9. Logout\n";
        resetColor();
        
        setColor(14);
        cout << "\nChoose: ";
        resetColor();

        int c;
        cin >> c;

        switch (c) {
            case 1: mm.addItem(); break;
            case 2: mm.updateItem(); break;
            case 3: mm.removeItem(); break;
            case 4: mm.viewMenu(); break;
            case 5: mm.searchItem(); break;
            case 6: os.viewPayments(); break;
            case 7: os.viewOrders(); break;
            case 8: {
                clearScreen();
                setColor(14);
                cout << "\n+------------------------------------+\n";
                cout << "|         CUSTOMER FEEDBACK         |\n";
                cout << "+------------------------------------+\n";
                resetColor();
                
                ifstream fin("feedback.txt");
                if (!fin) {
                    setColor(12);
                    cout << "\nNo feedback found!\n";
                    resetColor();
                    pauseScreen();
                    break;
                }

                setColor(11);
                cout << "\n========== CUSTOMER FEEDBACK ==========\n";
                resetColor();

                string line;
                int count = 1;
                while (getline(fin, line)) {
                    if (count % 2 == 0) {
                        setColor(7);
                    } else {
                        setColor(8);
                    }
                    cout << count << ". " << line << endl;
                    count++;
                }
                fin.close();

                setColor(11);
                cout << "\n=======================================\n";
                cout << "Total Feedback Entries: " << (count - 1) << endl;
                cout << "=======================================\n";
                resetColor();
                pauseScreen();
                break;
            }
            case 9: 
                setColor(10);
                cout << "\nLogging out from Admin Portal...\n";
                resetColor();
                return;
            default:
                setColor(12);
                cout << "Invalid choice!\n";
                resetColor();
                pauseScreen();
        }
    }
}

void staffPortal() {
    MenuManager mm;
    OrderSystem os;
    string staffUsername;

    clearScreen();
    setColor(11);
    cout << "\n+------------------------------------+\n";
    cout << "|            STAFF PORTAL              |\n";
    cout << "+------------------------------------+\n";
    resetColor();
    
    cout << "Enter your staff username: ";
    cin >> staffUsername;

    while (true) {
        clearScreen();
        
        setColor(11);
        cout << "\n+------------------------------------+\n";
        cout << "|            STAFF PORTAL           |\n";
        cout << "|  User: " << left << setw(28) << staffUsername << "|\n";
        cout << "+------------------------------------+\n";
        resetColor();

        setColor(10);
        cout << "1. View Menu\n";
        cout << "2. Search Menu\n";
        cout << "3. Take Customer Order\n";
        cout << "4. View All Orders\n";
        cout << "5. View Payment Status\n";
        cout << "6. Logout\n";
        resetColor();
        
        setColor(14);
        cout << "\nChoose: ";
        resetColor();

        int c;
        cin >> c;

        switch (c) {
            case 1: 
                clearScreen();
                setColor(11);
                cout << "\n+------------------------------------+\n";
                cout << "|            VIEW MENU               |\n";
                cout << "|  Staff: " << left << setw(28) << staffUsername << "  |\n";
                cout << "+------------------------------------+\n";
                resetColor();
                mm.viewMenu(); 
                break;
                
            case 2: 
                clearScreen();
                setColor(13);
                cout << "\n+------------------------------------+\n";
                cout << "|            SEARCH MENU           |\n";
                cout << "|  Staff: " << left << setw(28) << staffUsername << "   |\n";
                cout << "+------------------------------------+\n";
                resetColor();
                mm.searchItem(); 
                break;
                
            case 3: {
                clearScreen();
                setColor(10);
                cout << "\n+------------------------------------+\n";
                cout << "|         TAKE CUSTOMER ORDER        |\n";
                cout << "|  Staff: " << left << setw(28) << staffUsername << "|\n";
                cout << "+------------------------------------+\n";
                resetColor();
                
                string customerID;
                setColor(11);
                cout << "Enter Customer ID: ";
                resetColor();
                cin >> customerID;
                
                setColor(10);
                cout << "Taking order for customer: " << customerID << endl;
                cout << "Staff assisting: " << staffUsername << endl;
                resetColor();
                
                os.placeOrder(customerID);
                break;
            }
            case 4: 
                clearScreen();
                setColor(9);
                cout << "\n+------------------------------------+\n";
                cout << "|           VIEW ALL ORDERS            |\n";
                cout << "|  Staff: " << left << setw(28) << staffUsername << "  |\n";
                cout << "+------------------------------------+\n";
                resetColor();
                os.viewOrders(); 
                break;
                
            case 5: 
                clearScreen();
                setColor(10);
                cout << "\n+------------------------------------+\n";
                cout << "|          PAYMENT STATUS              |\n";
                cout << "|  Staff: " << left << setw(28) << staffUsername << "  |\n";
                cout << "+------------------------------------+\n";
                resetColor();
                os.viewPayments(); 
                break;
                
            case 6: 
                setColor(10);
                cout << "\nLogging out from Staff Portal...\n";
                resetColor();
                return;

            default:
                setColor(12);
                cout << "Invalid choice!\n";
                resetColor();
                pauseScreen();
        }
    }
}

void customerPortal(string customerID) {
    Customer c(customerID);

    while (true) {
        clearScreen();
        c.displayPortalInfo();

        setColor(11);
        cout << "\n1. View Menu\n";
        cout << "2. Place Order\n";
        cout << "3. View Last Order\n";
        cout << "4. View Receipt\n";
        cout << "5. View Payment History\n";
        cout << "6. Give Feedback & Rating\n";
        cout << "7. Logout\n";
        resetColor();
        
        setColor(14);
        cout << "\nChoose: ";
        resetColor();

        int ch;
        cin >> ch;

        switch (ch) {
            case 1: c.selectMenu(); break;
            case 2: c.placeOrder(); break;
            case 3: c.viewLastOrder(); break;
            case 4: c.viewReceipt(); break;
            case 5: c.viewPayments(); break;
            case 6: c.giveFeedback(); break;
            case 7: 
                setColor(10);
                cout << "\nThank you for visiting ZenZest, " << customerID << "!\n";
                resetColor();
                return;

            default:
                setColor(12);
                cout << "Invalid choice!\n";
                resetColor();
                pauseScreen();
        }
    }
}

/* ------------------------------
    INITIALIZE FILES
   ------------------------------*/
void initializeMenu() {
    ofstream fout("menu.txt");
    
    if (!fout) {
        setColor(12);
        cout << "Error: Could not create menu.txt file!\n";
        resetColor();
        return;
    }
    
    // Starters
    fout << "1,Starters,Spring Rolls,250" << endl;
    fout << "2,Starters,Chicken Wings,350" << endl;
    fout << "3,Starters,Garlic Bread,200" << endl;
    fout << "4,Starters,French Fries,180" << endl;
    fout << "5,Starters,Cheese Nuggets,280" << endl;
    
    // Main Course
    fout << "10,Main Course,Chicken Biryani,450" << endl;
    fout << "11,Main Course,Mutton Karahi,650" << endl;
    fout << "12,Main Course,Grilled Fish,550" << endl;
    fout << "13,Main Course,Beef Steak,700" << endl;
    fout << "14,Main Course,Vegetable Pasta,380" << endl;
    fout << "15,Main Course,Chicken Tikka Masala,480" << endl;
    
    // Desserts
    fout << "20,Desserts,Chocolate Brownie,250" << endl;
    fout << "21,Desserts,Ice Cream Sundae,200" << endl;
    fout << "22,Desserts,Gulab Jamun,150" << endl;
    fout << "23,Desserts,Tiramisu,320" << endl;
    fout << "24,Desserts,Fruit Custard,180" << endl;
    
    // Specialities
    fout << "30,Specialities,Tandoori Platter,850" << endl;
    fout << "31,Specialities,Seafood Mix Grill,950" << endl;
    fout << "32,Specialities,Lahori Chargha,750" << endl;
    fout << "33,Specialities,BBQ Ribs,800" << endl;
    
    // Beverages
    fout << "40,Beverages,Fresh Lime Soda,120" << endl;
    fout << "41,Beverages,Mango Juice,150" << endl;
    fout << "42,Beverages,Cola,100" << endl;
    fout << "43,Beverages,Coffee,180" << endl;
    fout << "44,Beverages,Green Tea,130" << endl;
    fout << "45,Beverages,Mint Margarita,160" << endl;
    
    // Add Ons
    fout << "50,Add Ons,Extra Cheese,50" << endl;
    fout << "51,Add Ons,Raita,80" << endl;
    fout << "52,Add Ons,Salad,100" << endl;
    fout << "53,Add Ons,Naan,40" << endl;
    fout << "54,Add Ons,Rice Bowl,120" << endl;
    
    fout.close();
}

void initializeUsers() {
    ifstream checkFile("users.txt");
    if (checkFile.good()) {
        checkFile.close();
        return; 
    }
    checkFile.close();

    ofstream fout("users.txt");
    
    fout << "admin admin123 admin\n";
    fout << "staff staff123 staff\n";
    
    fout.close();
    
    setColor(10);
    cout << "Users initialized!\n";
    cout << "Default Admin: username=admin, password=admin123\n";
    cout << "Default Staff: username=staff, password=staff123\n";
    resetColor();
}

/* ------------------------------
          MAIN MENU
   ------------------------------*/
int main() {
    initializeMenu();
    initializeUsers();
    
    while (true) {
        clearScreen();
        setColor(11);
        cout << "\n+------------------------------------+\n";
        cout << "|         WELCOME TO ZENZEST         |\n";
        cout << "|        Restaurant Management       |\n";
        cout << "+------------------------------------+\n";
        resetColor();

        setColor(14);
        cout << "\nLogin as:\n";
        cout << "1. Admin\n";
        cout << "2. Staff\n";
        cout << "3. Customer\n";
        cout << "4. Exit\n";
        resetColor();
        
        setColor(11);
        cout << "\nEnter choice: ";
        resetColor();

        int choice;
        cin >> choice;

        if (choice == 4) {
            clearScreen();
            setColor(10);
            cout << "\n==============================================\n";
            cout << "   Thank you for visiting ZenZest!\n";
            cout << "              Goodbye!\n";
            cout << "==============================================\n\n";
            resetColor();
            break;
        }

        if (choice == 1) {
            User admin = User::login();
            if (admin.getRole() == "admin") {
                adminPortal();
            } else {
                setColor(12);
                cout << "Access denied! Admin privileges required.\n";
                resetColor();
                pauseScreen();
            }
        }
        else if (choice == 2) {
            User staff = User::login();
            if (staff.getRole() == "staff") {
                staffPortal();
            } else {
                setColor(12);
                cout << "Access denied! Staff privileges required.\n";
                resetColor();
                pauseScreen();
            }
        }
        else if (choice == 3) {
            clearScreen();
            setColor(11);
            cout << "\n+------------------------------------+\n";
            cout << "|          CUSTOMER LOGIN            |\n";
            cout << "+------------------------------------+\n";
            resetColor();
            
            string custID;
            setColor(14);
            cout << "Enter your Customer ID: ";
            resetColor();
            cin >> custID;
            
            if (custID.empty()) {
                setColor(12);
                cout << "Customer ID cannot be empty!\n";
                resetColor();
                pauseScreen();
            } else {
                setColor(10);
                cout << "Welcome, " << custID << "!\n";
                resetColor();
                pauseScreen();
                customerPortal(custID);
            }
        }
        else {
            setColor(12);
            cout << "Invalid choice!\n";
            resetColor();
            pauseScreen();
        }
    }

    return 0;
}

