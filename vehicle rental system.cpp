#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Base class for Vehicle
class Vehicle
{
protected:
    string model;
    string licensePlate;
    bool isAvailable;
    double ratePerDay;

public:
    Vehicle(string model, string licensePlate, double ratePerDay)
        : model(model), licensePlate(licensePlate), ratePerDay(ratePerDay), isAvailable(true) {}

    virtual void display()
    {
        cout << "Model: " << model << ", License Plate : " << licensePlate << ", Rate per day: $" << ratePerDay << ", Available: " << (isAvailable ? "Yes" : "No") << endl;
    }

    bool isVehicleAvailable()
    {
        return isAvailable;
    }

    void setAvailability(bool status)
    {
        isAvailable = status;
    }

    double getRate()
    {
        return ratePerDay;
    }

    void updateDetails(string newModel, string newLicensePlate, double newRatePerDay)
    {
        model = newModel;
        licensePlate = newLicensePlate;
        ratePerDay = newRatePerDay;
    }

    virtual ~Vehicle() {}
};

// Derived classes for different vehicle types
class Car : public Vehicle
{
public:
    Car(string model, string licensePlate, double ratePerDay)
        : Vehicle(model, licensePlate, ratePerDay) {}

    void display() override
    {
        cout << "[Car] ";
        Vehicle::display();
    }
};

class Bike : public Vehicle
{
public:
    Bike(string model, string licensePlate, double ratePerDay)
        : Vehicle(model, licensePlate, ratePerDay) {}

    void display() override
    {
        cout << "[Bike] ";
        Vehicle::display();
    }
};

class Truck : public Vehicle
{
public:
    Truck(string model, string licensePlate, double ratePerDay)
        : Vehicle(model, licensePlate, ratePerDay) {}

    void display() override
    {
        cout << "[Truck] ";
        Vehicle::display();
    }
};

// Base class for User
class User
{
protected:
    string name;
    string email;
    string contact;
    string password;
    vector<Vehicle *> bookedVehicles; // Store booked vehicles

public:
    User() : name(""), email(""), contact(""), password("") {}
    User(string name, string email, string contact, string password)
        : name(name), email(email), contact(contact), password(password) {}

    virtual void displayUserInfo()
    {
        cout << "Name: " << name << ", Email: " << email << ", Contact: " << contact << ", Password" << password << endl;
    }

    string getName() const
    {
        return name;
    }
    string getPassword() const
    {
        return password;
    }
    string getEmail() const
    {
        return email;
    }

    void bookVehicle(Vehicle *vehicle)
    {
        bookedVehicles.push_back(vehicle);
    }

    void returnVehicle(Vehicle *vehicle)
    {
        auto it = find(bookedVehicles.begin(), bookedVehicles.end(), vehicle);
        if (it != bookedVehicles.end())
        {
            bookedVehicles.erase(it);
        }
        else
        {
            cout << "This vehicle was not booked by you!" << endl;
        }
    }

    bool hasBookedVehicle(Vehicle *vehicle)
    {
        return find(bookedVehicles.begin(), bookedVehicles.end(), vehicle) != bookedVehicles.end();
    }

    void displayBookedVehicles()
    {
        cout << "\n--- Booked Vehicles ---" << endl;
        if (bookedVehicles.size() == 0)
        {
            cout << "NO vechiles booked" << endl;
            return;
        }
        for (size_t i = 0; i < bookedVehicles.size(); i++)
        {
            cout << "Index: " << i << " ";
            bookedVehicles[i]->display();
        }
    }

    virtual ~User() {}
};

// Admin class inherits from User
class Admin : public User
{
private:
    vector<Vehicle *> vehicles; // Inventory of vehicles

public:
    Admin(string name, string email, string contact, string password)
        : User(name, email, contact, password) {}

    // Admin Authentication
    bool authenticate(string inputEmail, string inputPassword)
    {
        return (inputEmail == email && inputPassword == this->password);
    }

    // Add vehicle to inventory
    void addVehicle(Vehicle *vehicle)
    {
        vehicles.push_back(vehicle);
        cout << "Vehicle added successfully!" << endl;
    }

    // Delete vehicle from inventory
    void deleteVehicle(int index)
    {
        if (index >= 0 && index < vehicles.size())
        {
            delete vehicles[index]; // Free memory
            vehicles.erase(vehicles.begin() + index);
            cout << "Vehicle deleted successfully!" << endl;
        }
        else
        {
            cout << "Invalid vehicle index!" << endl;
        }
    }

    // Update vehicle details
    void updateVehicleDetails(int index, string newModel, string newLicensePlate, double newRatePerDay)
    {
        if (index >= 0 && index < vehicles.size())
        {
            vehicles[index]->updateDetails(newModel, newLicensePlate, newRatePerDay);
            cout << "Vehicle details updated successfully!" << endl;
        }
        else
        {
            cout << "Invalid vehicle index!" << endl;
        }
    }

    // Display all vehicles in the inventory
    bool displayInventory()
    {
        cout << "\n--- Vehicle Inventory ---" << endl;
        if (vehicles.size() == 0)
        {
            cout << "No vehicles added yet" << endl;
            return false;
        }
        for (size_t i = 0; i < vehicles.size(); i++)
        {
            cout << "Index: " << i << " ";
            vehicles[i]->display();
        }
        return true;
    }

    Vehicle *getVehicle(int index)
    {
        return vehicles[index];
    }

    int getInventorySize()
    {
        return vehicles.size();
    }
};

// Rental class for managing bookings
class Rental
{
private:
    Admin &admin;

public:
    Rental(Admin &admin) : admin(admin) {}

    void displayInventory()
    {
        admin.displayInventory();
    }
    void bookVehicle(int vehicleIndex, int days, User &user)
    {
        if (vehicleIndex >= 0 && vehicleIndex < admin.getInventorySize())
        {
            Vehicle *vehicle = admin.getVehicle(vehicleIndex);
            if (vehicle->isVehicleAvailable())
            {
                double cost = vehicle->getRate() * days;
                cout << "Booking confirmed for " << user.getName() << " for " << days << " days. Total cost: $" << cost << endl;
                vehicle->setAvailability(false);
                user.bookVehicle(vehicle);
            }
            else
            {
                cout << "Sorry, this vehicle is not available!" << endl;
            }
        }
        else
        {
            cout << "Invalid vehicle index!" << endl;
        }
    }

    void returnVehicle(int vehicleIndex, User &user)
    {
        if (vehicleIndex >= 0 && vehicleIndex < admin.getInventorySize())
        {
            Vehicle *vehicle = admin.getVehicle(vehicleIndex);
            if (user.hasBookedVehicle(vehicle))
            {
                vehicle->setAvailability(true);
                user.returnVehicle(vehicle);
                cout << "Vehicle returned successfully!" << endl;
            }
            else
            {
                cout << "This vehicle was not booked by you!" << endl;
            }
        }
        else
        {
            cout << "Invalid vehicle index!" << endl;
        }
    }
};

// Function to show admin menu
void adminMenu(Admin &admin)
{
    while (true)
    {
        cout << "\n--- Admin Menu ---" << endl;
        cout << "1. Add Vehicle" << endl;
        cout << "2. Delete Vehicle" << endl;
        cout << "3. Update Vehicle Details" << endl;
        cout << "4. Display Inventory" << endl;
        cout << "5. Return to Main Menu" << endl;
        cout << "Choose an option: ";

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            cout << "Enter vehicle type (Car/Bike/Truck): ";
            string type;
            cin >> type;
            cout << "Enter model: ";
            string model;
            cin >> model;
            cout << "Enter license plate: ";
            string licensePlate;
            cin >> licensePlate;
            cout << "Enter rate per day: ";
            double ratePerDay;
            cin >> ratePerDay;

            if (type == "Car")
            {
                admin.addVehicle(new Car(model, licensePlate, ratePerDay));
            }
            else if (type == "Bike")
            {
                admin.addVehicle(new Bike(model, licensePlate, ratePerDay));
            }
            else if (type == "Truck")
            {
                admin.addVehicle(new Truck(model, licensePlate, ratePerDay));
            }
            else
            {
                cout << "Invalid vehicle type!" << endl;
            }
        }
        else if (choice == 2)
        {
            if (admin.displayInventory() == true)
            {
                admin.displayInventory();
                cout << "Enter vehicle index to delete: ";
                int index;
                cin >> index;
                admin.deleteVehicle(index);
            }
            // else
            // {
            //     cout << "No vechiles was added";
            // }
        }
        else if (choice == 3)
        {
            if (admin.displayInventory() == true)
            {
                admin.displayInventory();
                cout << "Enter vehicle index to update: ";
                int index;
                cin >> index;

                cout << "Enter new model: ";
                string newModel;
                cin >> newModel;
                cout << "Enter new license plate: ";
                string newLicensePlate;
                cin >> newLicensePlate;
                cout << "Enter new rate per day: ";
                double newRatePerDay;
                cin >> newRatePerDay;

                admin.updateVehicleDetails(index, newModel, newLicensePlate, newRatePerDay);
            }
            // else
            // {
            //     cout << "No vechiles was added";
            // }
        }
        else if (choice == 4)
        {
            admin.displayInventory();
        }
        else if (choice == 5)
        {
            break;
        }
        else
        {
            cout << "Invalid option! Please try again." << endl;
        }
    }
}

// Function to show user menu
void userMenu(Rental &rental, User &user)
{
    while (true)
    {
        cout << "\n--- User Menu ---" << endl;
        cout << "1. Book Vehicle" << endl;
        cout << "2. Return Vehicle" << endl;
        cout << "3. Display Booked Vehicles" << endl;
        cout << "4. Return to Main Menu" << endl;
        cout << "Choose an option: ";

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            rental.displayInventory(); // Show inventory before booking
            cout << "Enter vehicle index to book: ";
            int index, days;
            cin >> index;
            cout << "Enter number of days to book: ";
            cin >> days;

            rental.bookVehicle(index, days, user);
        }
        else if (choice == 2)
        {
            user.displayBookedVehicles();
            cout << "Enter vehicle index to return: ";
            int index;
            cin >> index;

            rental.returnVehicle(index, user);
        }
        else if (choice == 3)
        {
            user.displayBookedVehicles();
        }
        else if (choice == 4)
        {
            break;
        }
        else
        {
            cout << "Invalid option! Please try again." << endl;
        }
    }
}

// Main function for testing
int main()
{
    // Create Admin
    Admin admin("Admin", "admin@gmail.com", "7206493923", "12345678");

    // Create rental system
    Rental rental(admin);

    unordered_map<string, User> registeredUsers;

    while (true)
    {
        cout << "Login as  (1)Admin or  (2)User: ";
        int loginChoice;
        cin >> loginChoice;

        if (loginChoice == 1)
        {
            // Admin Authentication
            cout << "Admin Login\nEnter Email: ";
            string email;
            cin >> email;
            cout << "Enter Password: ";
            string password;
            cin >> password;

            // Authenticate Admin
            if (!admin.authenticate(email, password))
            {
                cout << "Invalid admin credentials!" << endl;
                continue;
            }

            cout << "Admin authenticated successfully!" << endl;

            // Admin menu
            adminMenu(admin);
        }
        else if (loginChoice == 2)
        {
            cout << "Are you a registered user? (yes/no): ";
            string isRegistered;
            cin >> isRegistered;

            if (isRegistered == "yes")
            {
                cout << "Enter Email: ";
                string userEmail;
                cin >> userEmail;
                cout << "Enter Password: ";
                string password;
                cin >> password;

                if (registeredUsers.find(userEmail) != registeredUsers.end())
                {
                    if (registeredUsers[userEmail].getPassword() != password)
                    {
                        cout << "User not found or incorrect credentials!" << endl;
                        return 0;
                    }
                    User &user = registeredUsers[userEmail];
                    cout << "User logged in successfully!" << endl;
                    userMenu(rental, user);
                }
                else
                {
                    cout << "User not found or incorrect credentials!" << endl;
                }
            }
            else
            {
                // User registration
                cout << "\nUser Registration\nEnter Name: ";
                string name;
                cin >> name;
                cout << "Enter Email: ";
                string userEmail;
                cin >> userEmail;
                cout << "Enter Contact No: ";
                string contact;
                cin >> contact;
                cout << "Enter Password:";
                string password;
                cin >> password;
                User user(name, userEmail, contact, password);
                registeredUsers[userEmail] = user;

                cout << "User registered successfully!" << endl;
                userMenu(rental, user);
            }
        }
        else
        {
            cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}