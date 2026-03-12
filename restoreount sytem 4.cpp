#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <algorithm>

using namespace std;

// MenuItem class
class MenuItem {
private:
    int id;
    string name;
    double price;
    string category;

public:
    MenuItem(int id, string name, double price, string category)
        : id(id), name(name), price(price), category(category) {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    string getCategory() const { return category; }

    void display() const {
        cout << "ID: " << id << " | " << name << " | $" 
             << fixed << setprecision(2) << price << " | " << category << endl;
    }
};

// OrderItem class
class OrderItem {
private:
    MenuItem item;
    int quantity;

public:
    OrderItem(MenuItem item, int qty) : item(item), quantity(qty) {}

    MenuItem getItem() const { return item; }
    int getQuantity() const { return quantity; }
    double getTotal() const { return item.getPrice() * quantity; }

    void display() const {
        cout << "  " << item.getName() << " x" << quantity 
             << " = $" << fixed << setprecision(2) << getTotal() << endl;
    }
};

// Order class
class Order {
private:
    int orderId;
    string customerName;
    vector<OrderItem> items;
    string status; // "Pending", "Preparing", "Ready", "Completed"
    time_t createdTime;

public:
    Order(int id, string name) 
        : orderId(id), customerName(name), status("Pending") {
        createdTime = time(0);
    }

    int getOrderId() const { return orderId; }
    string getCustomerName() const { return customerName; }
    string getStatus() const { return status; }

    void addItem(MenuItem item, int quantity) {
        items.push_back(OrderItem(item, quantity));
    }

    double getTotal() const {
        double total = 0;
        for (const auto& item : items) {
            total += item.getTotal();
        }
        return total;
    }

    void setStatus(string newStatus) {
        status = newStatus;
    }

    void display() const {
        cout << "\n=== ORDER #" << orderId << " ===" << endl;
        cout << "Customer: " << customerName << endl;
        cout << "Status: " << status << endl;
        cout << "Items:" << endl;
        for (const auto& item : items) {
            item.display();
        }
        cout << "Total: $" << fixed << setprecision(2) << getTotal() << endl;
        cout << "========================\n" << endl;
    }
};

// Menu class
class Menu {
private:
    vector<MenuItem> items;
    int nextId;

public:
    Menu() : nextId(1) {}

    void addItem(string name, double price, string category) {
        items.push_back(MenuItem(nextId++, name, price, category));
    }

    MenuItem* getItemById(int id) {
        for (auto& item : items) {
            if (item.getId() == id) {
                return &item;
            }
        }
        return nullptr;
    }

    void displayMenu() const {
        cout << "\n========== MENU ==========" << endl;
        string currentCategory = "";
        for (const auto& item : items) {
            if (item.getCategory() != currentCategory) {
                currentCategory = item.getCategory();
                cout << "\n--- " << currentCategory << " ---" << endl;
            }
            item.display();
        }
        cout << "========================\n" << endl;
    }

    void displayMenuByCategory(string category) const {
        cout << "\n=== " << category << " ===" << endl;
        for (const auto& item : items) {
            if (item.getCategory() == category) {
                item.display();
            }
        }
    }

    int getMenuSize() const { return items.size(); }
};

// RestaurantSystem class
class RestaurantSystem {
private:
    Menu menu;
    vector<Order> orders;
    int nextOrderId;

public:
    RestaurantSystem() : nextOrderId(1001) {
        initializeMenu();
    }

    void initializeMenu() {
        // Appetizers
        menu.addItem("Spring Rolls", 6.99, "Appetizers");
        menu.addItem("Garlic Bread", 4.99, "Appetizers");
        menu.addItem("Chicken Wings", 8.99, "Appetizers");

        // Main Courses
        menu.addItem("Grilled Salmon", 16.99, "Main Courses");
        menu.addItem("Ribeye Steak", 24.99, "Main Courses");
        menu.addItem("Pasta Carbonara", 14.99, "Main Courses");
        menu.addItem("Chicken Teriyaki", 13.99, "Main Courses");

        // Desserts
        menu.addItem("Chocolate Cake", 7.99, "Desserts");
        menu.addItem("Cheesecake", 6.99, "Desserts");
        menu.addItem("Ice Cream Sundae", 5.99, "Desserts");

        // Beverages
        menu.addItem("Soft Drink", 2.99, "Beverages");
        menu.addItem("Coffee", 3.49, "Beverages");
        menu.addItem("Wine Glass", 7.99, "Beverages");
    }

    void displayMenu() {
        menu.displayMenu();
    }

    void createOrder(string customerName) {
        Order newOrder(nextOrderId++, customerName);
        cout << "\n=== CREATE NEW ORDER ===" << endl;
        cout << "Order ID: " << newOrder.getOrderId() << endl;
        cout << "Customer: " << customerName << endl;

        bool addingItems = true;
        while (addingItems) {
            menu.displayMenu();
            cout << "Enter menu item ID (0 to finish): ";
            int itemId;
            cin >> itemId;

            if (itemId == 0) {
                addingItems = false;
                break;
            }

            MenuItem* item = menu.getItemById(itemId);
            if (item == nullptr) {
                cout << "Invalid item ID!" << endl;
                continue;
            }

            cout << "Quantity: ";
            int quantity;
            cin >> quantity;

            if (quantity <= 0) {
                cout << "Invalid quantity!" << endl;
                continue;
            }

            newOrder.addItem(*item, quantity);
            cout << "Added " << quantity << "x " << item->getName() << " to order." << endl;
        }

        orders.push_back(newOrder);
        cout << "\nOrder created successfully!" << endl;
    }

    void viewAllOrders() {
        if (orders.empty()) {
            cout << "\nNo orders yet." << endl;
            return;
        }

        cout << "\n========== ALL ORDERS ==========" << endl;
        for (const auto& order : orders) {
            cout << "Order #" << order.getOrderId() 
                 << " | Customer: " << order.getCustomerName()
                 << " | Status: " << order.getStatus()
                 << " | Total: $" << fixed << setprecision(2) << order.getTotal() << endl;
        }
        cout << "================================\n" << endl;
    }

    void viewOrderDetails(int orderId) {
        for (auto& order : orders) {
            if (order.getOrderId() == orderId) {
                order.display();
                return;
            }
        }
        cout << "Order not found!" << endl;
    }

    void updateOrderStatus(int orderId) {
        for (auto& order : orders) {
            if (order.getOrderId() == orderId) {
                cout << "\nCurrent status: " << order.getStatus() << endl;
                cout << "New status options:" << endl;
                cout << "1. Pending" << endl;
                cout << "2. Preparing" << endl;
                cout << "3. Ready" << endl;
                cout << "4. Completed" << endl;
                cout << "Select new status (1-4): ";

                int choice;
                cin >> choice;
                cin.ignore();

                switch (choice) {
                    case 1:
                        order.setStatus("Pending");
                        break;
                    case 2:
                        order.setStatus("Preparing");
                        break;
                    case 3:
                        order.setStatus("Ready");
                        break;
                    case 4:
                        order.setStatus("Completed");
                        break;
                    default:
                        cout << "Invalid choice!" << endl;
                        return;
                }
                cout << "Status updated successfully!" << endl;
                return;
            }
        }
        cout << "Order not found!" << endl;
    }

    void generateReport() {
        if (orders.empty()) {
            cout << "\nNo orders to report." << endl;
            return;
        }

        cout << "\n========== SALES REPORT ==========" << endl;
        double totalRevenue = 0;
        int totalOrders = 0;
        int completedOrders = 0;

        for (const auto& order : orders) {
            totalRevenue += order.getTotal();
            totalOrders++;
            if (order.getStatus() == "Completed") {
                completedOrders++;
            }
        }

        cout << "Total Orders: " << totalOrders << endl;
        cout << "Completed Orders: " << completedOrders << endl;
        cout << "Total Revenue: $" << fixed << setprecision(2) << totalRevenue << endl;
        cout << "Average Order Value: $" << fixed << setprecision(2) 
             << (totalRevenue / totalOrders) << endl;
        cout << "=================================\n" << endl;
    }

    void displayMainMenu() {
        cout << "\n===== RESTAURANT SYSTEM =====" << endl;
        cout << "1. View Menu" << endl;
        cout << "2. Create New Order" << endl;
        cout << "3. View All Orders" << endl;
        cout << "4. View Order Details" << endl;
        cout << "5. Update Order Status" << endl;
        cout << "6. Generate Sales Report" << endl;
        cout << "7. Exit" << endl;
        cout << "=============================" << endl;
        cout << "Select option: ";
    }

    void run() {
        int choice;
        bool running = true;

        while (running) {
            displayMainMenu();
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1:
                    displayMenu();
                    break;

                case 2: {
                    cout << "Enter customer name: ";
                    string name;
                    getline(cin, name);
                    createOrder(name);
                    break;
                }

                case 3:
                    viewAllOrders();
                    break;

                case 4: {
                    cout << "Enter order ID: ";
                    int orderId;
                    cin >> orderId;
                    viewOrderDetails(orderId);
                    break;
                }

                case 5: {
                    cout << "Enter order ID to update: ";
                    int orderId;
                    cin >> orderId;
                    updateOrderStatus(orderId);
                    break;
                }

                case 6:
                    generateReport();
                    break;

                case 7:
                    cout << "Thank you for using Restaurant System. Goodbye!" << endl;
                    running = false;
                    break;

                default:
                    cout << "Invalid option! Please try again." << endl;
            }
        }
    }
};

// Main function
int main() {
    RestaurantSystem restaurant;
    restaurant.run();
    return 0;
}