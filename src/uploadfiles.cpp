#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>

using namespace std;

const string FILE_NAME = "inventory.txt";

struct Product {
    int id;
    string name;
    int quantity;
    double price;
};

// Function Prototypes
void clearScreen();
void waitForUser();
void displayMainMenu();
void addProduct();
void updateProduct();
void deleteProduct();
void searchProduct();
void viewInventory();
void processCustomerOrder();
void loadInventory(vector<Product>& inventory);
void saveInventory(const vector<Product>& inventory);
int getValidatedInteger(string prompt);
double getValidatedDouble(string prompt);
string getValidatedString(string prompt);
int findProductIndex(const vector<Product>& inventory, int id);

int main() {
    int choice;
    do {
        clearScreen();
        displayMainMenu();
        choice = getValidatedInteger("   Select an option (1-7): ");
        
        clearScreen(); // Clear screen to show the selected feature's display
        switch (choice) {
            case 1: addProduct(); break;
            case 2: updateProduct(); break;
            case 3: deleteProduct(); break;
            case 4: searchProduct(); break;
            case 5: viewInventory(); break;
            case 6: processCustomerOrder(); break;
            case 7: 
                cout << "\n==================================================\n";
                cout << "   Thank you for using RetailManage. Goodbye!\n"; 
                cout << "==================================================\n";
                break;
            default: 
                cout << "\n[!] Invalid choice! Please select between 1 and 7.\n";
                waitForUser();
        }
        if (choice != 7) {
            waitForUser();
        }
    } while (choice != 7);

    return 0;
}

// Clears the console window for a clean "App Display" transition
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Pauses the screen so the user can look at the data display before returning to the menu
void waitForUser() {
    cout << "\nPress Enter to return to the Main Menu...";
    cin.get();
}

void displayMainMenu() {
    cout << "==================================================\n";
    cout << "        PUP-SR RETAIL INVENTORY SYSTEM            \n";
    cout << "==================================================\n";
    cout << "   [1] Add New Product\n";
    cout << "   [2] Update Product Information\n";
    cout << "   [3] Delete Product Record\n";
    cout << "   [4] Search Product by ID\n";
    cout << "   [5] View All Products & Stock Levels\n";
    cout << "   [6] Process Customer Order\n";
    cout << "   [7] Exit Application\n";
    cout << "==================================================\n";
}

void addProduct() {
    vector<Product> inventory;
    loadInventory(inventory);

    cout << "==================================================\n";
    cout << "               ADD NEW PRODUCT                    \n";
    cout << "==================================================\n";
    int id = getValidatedInteger("Enter Unique Product ID: ");

    if (findProductIndex(inventory, id) != -1) {
        cout << "\n[ERROR] Product ID already exists in the system!\n";
        return;
    }

    Product newProduct;
    newProduct.id = id;
    newProduct.name = getValidatedString("Enter Product Name     : ");
    newProduct.quantity = getValidatedInteger("Enter Initial Stock    : ");
    newProduct.price = getValidatedDouble("Enter Unit Price (PHP) : ");

    inventory.push_back(newProduct);
    saveInventory(inventory);
    cout << "\n[SUCCESS] Product successfully saved to inventory database!\n";
}

void updateProduct() {
    vector<Product> inventory;
    loadInventory(inventory);

    cout << "==================================================\n";
    cout << "             UPDATE PRODUCT DETAILS               \n";
    cout << "==================================================\n";
    int id = getValidatedInteger("Enter Product ID to modify: ");
    int index = findProductIndex(inventory, id);

    if (index == -1) {
        cout << "\n[ERROR] Product ID not found.\n";
        return;
    }

    cout << "\n--- Current Record ---\n";
    cout << "Name: " << inventory[index].name << " | Stock: " << inventory[index].quantity << " | Price: P" << inventory[index].price << "\n\n";

    inventory[index].name = getValidatedString("Enter New Product Name : ");
    inventory[index].quantity = getValidatedInteger("Enter New Stock Level  : ");
    inventory[index].price = getValidatedDouble("Enter New Price (PHP)  : ");

    saveInventory(inventory);
    cout << "\n[SUCCESS] Database record updated successfully!\n";
}

void deleteProduct() {
    vector<Product> inventory;
    loadInventory(inventory);

    cout << "==================================================\n";
    cout << "             DELETE PRODUCT RECORD                \n";
    cout << "==================================================\n";
    int id = getValidatedInteger("Enter Product ID to remove: ");
    int index = findProductIndex(inventory, id);

    if (index == -1) {
        cout << "\n[ERROR] Product ID not found.\n";
        return;
    }

    char confirm;
    cout << "\nAre you sure you want to PERMANENTLY delete '" << inventory[index].name << "'? (y/n): ";
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (confirm == 'y' || confirm == 'Y') {
        inventory.erase(inventory.begin() + index);
        saveInventory(inventory);
        cout << "\n[SUCCESS] Product removed from database.\n";
    } else {
        cout << "\n[CANCELLED] Deletion aborted.\n";
    }
}

void searchProduct() {
    vector<Product> inventory;
    loadInventory(inventory);

    cout << "==================================================\n";
    cout << "               SEARCH PRODUCT                     \n";
    cout << "==================================================\n";
    int id = getValidatedInteger("Enter Product ID: ");
    int index = findProductIndex(inventory, id);

    if (index == -1) {
        cout << "\n[!] No matching product found in inventory.\n";
        return;
    }

    cout << "\n--- Product Information Display ---\n";
    cout << " -> Product ID   : " << inventory[index].id << "\n";
    cout << " -> Description  : " << inventory[index].name << "\n";
    cout << " -> Stock Status : " << inventory[index].quantity << " units available\n";
    cout << " -> Retail Price : PHP " << fixed << setprecision(2) << inventory[index].price << "\n";
}

void viewInventory() {
    vector<Product> inventory;
    loadInventory(inventory);

    cout << "=================================================================\n";
    cout << "                    CURRENT INVENTORY DISPLAY                    \n";
    cout << "=================================================================\n";
    
    if (inventory.empty()) {
        cout << "         [!] Database is empty. No products logged yet.\n";
        cout << "=================================================================\n";
        return;
    }

    // Grid Display
    cout << left << setw(10) << "ID" 
         << setw(28) << "Product Name" 
         << setw(14) << "Stock Level" 
         << setw(12) << "Price" << "\n";
    cout << string(65, '-') << "\n";

    for (const auto& prod : inventory) {
        string stockDisplay = (prod.quantity == 0) ? "OUT OF STOCK" : to_string(prod.quantity);
        cout << left << setw(10) << prod.id 
             << setw(28) << prod.name 
             << setw(14) << stockDisplay 
             << "PHP " << fixed << setprecision(2) << prod.price << "\n";
    }
    cout << "=================================================================\n";
}

void processCustomerOrder() {
    vector<Product> inventory;
    loadInventory(inventory);

    cout << "==================================================\n";
    cout << "             CUSTOMER ORDER ENTRY                 \n";
    cout << "==================================================\n";
    int id = getValidatedInteger("Enter Product ID to order: ");
    int index = findProductIndex(inventory, id);

    if (index == -1) {
        cout << "\n[ERROR] Product does not exist.\n";
        return;
    }

    if (inventory[index].quantity <= 0) {
        cout << "\n[OUT OF STOCK] Cannot process order for this item.\n";
        return;
    }

    cout << "\nProduct Found: " << inventory[index].name << "\n";
    cout << "Price per Unit: PHP " << inventory[index].price << "\n";
    cout << "Available Stock: " << inventory[index].quantity << "\n\n";

    int orderQty = getValidatedInteger("Enter Quantity to Purchase: ");

    if (orderQty <= 0) {
        cout << "\n[ERROR] Purchase quantity must be at least 1.\n";
        return;
    }

    if (orderQty > inventory[index].quantity) {
        cout << "\n[DENIED] Insufficient stock! Order cannot exceed available units.\n";
        return;
    }

    // Calculations
    double totalCost = orderQty * inventory[index].price;
    inventory[index].quantity -= orderQty; 

    saveInventory(inventory);

    // Displaying the Transaction Summary/Receipt Window
    clearScreen();
    cout << "========================================\n";
    cout << "          OFFICIAL TRANSACTION RECEIPT  \n";
    cout << "========================================\n";
    cout << " Item Ordered : " << inventory[index].name << "\n";
    cout << " Quantity     : " << orderQty << "\n";
    cout << " Unit Price   : PHP " << fixed << setprecision(2) << inventory[index].price << "\n";
    cout << "----------------------------------------\n";
    cout << " TOTAL AMOUNT : PHP " << totalCost << "\n";
    cout << "========================================\n";
    cout << " [SUCCESS] Stock deducted & checkout logged.\n";
}

// --- File Handling Functions ---
void loadInventory(vector<Product>& inventory) {
    inventory.clear();
    ifstream file(FILE_NAME);
    if (!file.is_open()) return; 

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, name, qtyStr, priceStr;

        if (getline(ss, idStr, ',') &&
            getline(ss, name, ',') &&
            getline(ss, qtyStr, ',') &&
            getline(ss, priceStr, ',')) {
            
            Product prod;
            prod.id = stoi(idStr);
            prod.name = name;
            prod.quantity = stoi(qtyStr);
            prod.price = stod(priceStr);
            inventory.push_back(prod);
        }
    }
    file.close();
}

void saveInventory(const vector<Product>& inventory) {
    ofstream file(FILE_NAME, ios::trunc); 
    if (!file.is_open()) return;

    for (const auto& prod : inventory) {
        file << prod.id << "," << prod.name << "," << prod.quantity << "," << prod.price << "\n";
    }
    file.close();
}

// --- Inputs Validation Helpers ---
int getValidatedInteger(string prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "    [!] Invalid Input. Please enter a positive whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double getValidatedDouble(string prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0.0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "    [!] Invalid Input. Please enter a valid currency amount.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string getValidatedString(string prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        if (!value.empty()) return value;
        cout << "    [!] Entry cannot be empty.\n";
    }
}

int findProductIndex(const vector<Product>& inventory, int id) {
    for (size_t i = 0; i < inventory.size(); i++) {
        if (inventory[i].id == id) return i;
    }
    return -1; 
}
