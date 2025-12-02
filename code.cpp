#include<iostream>
#include<vector>
#include<fstream>

using namespace std;

struct Time {
    int hh, mm, ss;
    char col1, col2;
};

struct Date {
    int day, month, year;
    char sym1, sym2;
};

class Vehicle {
    string pltno;
    int type;
    Date dt;
    Time arrive;
    Time departure;

public:
    void addVehicle();
    void deleteVehicle();
    void printVehicle(const Vehicle &v);
    void show();
    void displayTokens();
    void showTotalBill();
};

// Global Variables
vector<Vehicle> veh;
int totalVehicle = 0, totalCar = 0, totalBike = 0, totalAmt = 0;
fstream file;

void Vehicle::addVehicle() {
    Vehicle v;
    cout << "Enter vehicle type (1 for Car, 2 for Bike): ";
    cin >> v.type;
    cout << "Enter vehicle number: ";
    cin >> v.pltno;
    cout << "Enter arrival time (HH:MM:SS): ";
    cin >> v.arrive.hh >> v.arrive.col1 >> v.arrive.mm >> v.arrive.col2 >> v.arrive.ss;
    cout << "Enter date (DD/MM/YYYY): ";
    cin >> v.dt.day >> v.dt.sym1 >> v.dt.month >> v.dt.sym2 >> v.dt.year;

    veh.push_back(v);
    totalVehicle++;

    if (v.type == 1)
        totalCar++;
    else
        totalBike++;

    cout << "\nVehicle added successfully!" << endl;
}

// Compute time difference in hours
int computeTimeDifference(Time t1, Time t2) {
    int sec1 = t1.hh * 3600 + t1.mm * 60 + t1.ss;
    int sec2 = t2.hh * 3600 + t2.mm * 60 + t2.ss;
    
    if (sec2 < sec1) { // Handling cases where departure is after midnight
        sec2 += 24 * 3600;
    }

    return (sec2 - sec1) / 3600; // Returning hours only
}

void Vehicle::deleteVehicle() {
    string pno;
    int typ;
    Time depart;
    int timeDiff;
    int charge = 0;

    cout << "Enter vehicle type (1 for Car, 2 for Bike): ";
    cin >> typ;
    cout << "Enter vehicle number: ";
    cin >> pno;
    cout << "Enter departure time (HH:MM:SS): ";
    cin >> depart.hh >> depart.col1 >> depart.mm >> depart.col2 >> depart.ss;

    bool found = false;
    for (size_t j = 0; j < veh.size(); j++) {
        if (veh[j].pltno == pno && veh[j].type == typ) {
            found = true;
            veh[j].departure = depart;
            timeDiff = computeTimeDifference(veh[j].arrive, depart);

            if (veh[j].type == 1) { // Car
                totalCar--;
                charge = (timeDiff < 2) ? 20 : (timeDiff < 5) ? 40 : 50;
            } else { // Bike
                totalBike--;
                charge = (timeDiff < 2) ? 5 : (timeDiff < 5) ? 10 : 20;
            }

            cout << "\nVehicle " << veh[j].pltno << " has to pay Rs. " << charge << endl;
            totalAmt += charge;

            // Write to file
            file.open("parkingDatabase.txt", ios::app);
            if (!file) {
                cerr << "Error: File could not be opened!" << endl;
                return;
            }
            file << veh[j].type << "\t" << veh[j].pltno << "\t" << veh[j].dt.day << "/" << veh[j].dt.month << "/" << veh[j].dt.year
                 << "\t" << veh[j].arrive.hh << ":" << veh[j].arrive.mm << ":" << veh[j].arrive.ss
                 << "\t" << veh[j].departure.hh << ":" << veh[j].departure.mm << ":" << veh[j].departure.ss << endl;
            file.close();

            // Remove vehicle from vector
            veh.erase(veh.begin() + j);
            totalVehicle--;
            break;
        }
    }
    if (!found) {
        cout << "Vehicle not found!" << endl;
    }
}

void Vehicle::printVehicle(const Vehicle &v) {
    cout << v.type << "\t" << v.pltno << "\t" << v.dt.day << "/" << v.dt.month << "/" << v.dt.year
         << "\t" << v.arrive.hh << ":" << v.arrive.mm << ":" << v.arrive.ss << endl;
}

void Vehicle::show() {
    cout << "Type\tNumber\tDate\tArrival Time\n";
    for (const auto &v : veh) {
        printVehicle(v);
    }
}

void Vehicle::displayTokens() {
    cout << "Tokens:\n";
    for (const auto &v : veh) {
        cout << "Token: " << v.pltno << endl;
        cout << "Arrival Time: " << v.arrive.hh << ":" << v.arrive.mm << ":" << v.arrive.ss << endl;
        cout << "Date: " << v.dt.day << "/" << v.dt.month << "/" << v.dt.year << endl;
        cout << "------------------------------------" << endl;
    }
}

void Vehicle::showTotalBill() {
    cout << "Total Collection: Rs. " << totalAmt << endl;
}

void totalVehicles() {
    cout << "Total Vehicles: " << totalVehicle << endl;
    cout << "Total Cars: " << totalCar << endl;
    cout << "Total Bikes: " << totalBike << endl;
}

int main() {
    int choice;
    char ans;
    do {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        cout << "\nVEHICLE PARKING SYSTEM\n";
        cout << "1. Arrival of a vehicle\n";
        cout << "2. Total number of vehicles parked\n";
        cout << "3. Total Amount collected\n";
        cout << "4. Display Vehicles\n";
        cout << "5. Show Tokens\n";
        cout << "6. Remove Vehicle\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        Vehicle v;
        switch (choice) {
            case 1:
                v.addVehicle();
                break;
            case 2:
                totalVehicles();
                break;
            case 3:
                v.showTotalBill();
                break;
            case 4:
                v.show();
                break;
            case 5:
                v.displayTokens();
                break;
            case 6:
                v.deleteVehicle();
                break;
            case 7:
                exit(0);
        }
        cout << "\nDo you want to continue (y/n)? ";
        cin >> ans;
    } while (ans == 'y' || ans == 'Y');

    return 0;
}
