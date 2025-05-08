#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// Structure to store customer information
struct Customer {
    char name[100];        // Customer's name
    char address[100];     // Customer's address
    char phone[12];        // Customer's phone number
    char from_date[20];    // Reservation start date
    char to_date[20];      // Reservation end date
    float payment_advance; // Advance payment
    int booking_id;        // Booking ID
};

// Structure to store room information
struct Room {
    int roomNumber;        // Room number
    char type[10];         // Room type (AC/Non-AC)
    char stype[10];        // Room comfort type (Standard/Non-Standard)
    char ac[10];           // Air conditioning (Yes/No)
    int rent;              // Daily rent
    int status;            // Room status: 0 = available, 1 = reserved, 2 = checked-in
    Customer cust;         // Associated customer information
};

// Function prototypes
void addReservation();
void removeReservation();
void checkIn();
void checkOut();
void displayRoomAvailability();
void loadRooms(Room rooms[], int& count);
void saveRooms(const Room rooms[], int count);
void displayRoom(const Room& room);

// Maximum number of rooms
const int MAX_ROOMS = 100;
// File to store room information
const char ROOMS_FILE[] = "rooms.dat";

int main() {
    int choice;

    while (true) {
        // Display menu
        cout << "\nHotel Management System";
        cout << "\n1. Add Reservation";
        cout << "\n2. Remove Reservation";
        cout << "\n3. Check-In";
        cout << "\n4. Check-Out";
        cout << "\n5. Display Room Availability";
        cout << "\n6. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        // Perform action based on user choice
        switch (choice) {
            case 1:
                addReservation();
                break;
            case 2:
                removeReservation();
                break;
            case 3:
                checkIn();
                break;
            case 4:
                checkOut();
                break;
            case 5:
                displayRoomAvailability();
                break;
            case 6:
                return 0;
            default:
                cout << "\nInvalid choice!";
        }
    }

    return 0;
}

// Function to add a reservation
void addReservation() {
    Room rooms[MAX_ROOMS];
    int count;
    loadRooms(rooms, count); // Load existing room information

    if (count >= MAX_ROOMS) {
        cout << "\nMaximum room limit reached!";
        return;
    }

    Room room;
    cout << "\nEnter Room Number: ";
    cin >> room.roomNumber;
    cout << "\nEnter Type (AC/Non-AC): ";
    cin >> room.ac;
    cout << "\nEnter Comfort Type (S/N): ";
    cin >> room.type;
    cout << "\nEnter Size (B/S): ";
    cin >> room.stype;
    cout << "\nEnter Rent: ";
    cin >> room.rent;

    room.status = 1; // Mark room as reserved

    // Get customer details
    cout << "\nEnter Customer Name: ";
    cin.ignore();
    cin.getline(room.cust.name, 100);
    cout << "Enter Customer Address: ";
    cin.getline(room.cust.address, 100);
    cout << "Enter Customer Phone: ";
    cin.getline(room.cust.phone, 12);
    cout << "Enter From Date: ";
    cin.getline(room.cust.from_date, 20);
    cout << "Enter To Date: ";
    cin.getline(room.cust.to_date, 20);
    cout << "Enter Payment Advance: ";
    cin >> room.cust.payment_advance;

    rooms[count++] = room; // Add room to the array
    saveRooms(rooms, count); // Save updated room information
    cout << "\nReservation Added Successfully!";
}

// Function to remove a reservation
void removeReservation() {
    Room rooms[MAX_ROOMS];
    int count;
    loadRooms(rooms, count); // Load existing room information

    int roomNumber;
    cout << "\nEnter Room Number to Remove Reservation: ";
    cin >> roomNumber;

bool found = false;
    for (int i = 0; i < count; ++i) {
        if (rooms[i].roomNumber == roomNumber && rooms[i].status == 1) {
            rooms[i].status = 0; // Mark room as available
            rooms[i].cust = Customer(); // Reset customer information
            found = true;
            break;
        }
    }

    if (found) {
        saveRooms(rooms, count); // Save updated room information
        cout << "\nReservation Removed Successfully!";
    } else {
        cout << "\nReservation Not Found!";
    }
}

// Function to check-in a customer
void checkIn() {
    Room rooms[MAX_ROOMS];
    int count;
    loadRooms(rooms, count); // Load existing room information

    int roomNumber;
    cout << "\nEnter Room Number for Check-In: ";
    cin >> roomNumber;

    bool found = false;
    for (int i = 0; i < count; ++i) {
        if (rooms[i].roomNumber == roomNumber && rooms[i].status == 1) {
            rooms[i].status = 2; // Mark room as checked-in
            found = true;
            break;
        }
    }

    if (found) {
        saveRooms(rooms, count); // Save updated room information
        cout << "\nChecked-In Successfully!";
    } else {
        cout << "\nRoom Not Found or Not Reserved!";
    }
}

// Function to check-out a customer
void checkOut() {
    Room rooms[MAX_ROOMS];
    int count;
    loadRooms(rooms, count); // Load existing room information

    int roomNumber;
    cout << "\nEnter Room Number for Check-Out: ";
    cin >> roomNumber;

    bool found = false;
    for (int i = 0; i < count; ++i) {
        if (rooms[i].roomNumber == roomNumber && rooms[i].status == 2) {
            rooms[i].status = 0; // Mark room as available
            rooms[i].cust = Customer(); // Reset customer information
            found = true;
            break;
        }
    }

    if (found) {
        saveRooms(rooms, count); // Save updated room information
        cout << "\nChecked-Out Successfully!";
    } else {
        cout << "\nRoom Not Found or Not Checked-In!";
    }
}

// Function to display available rooms
void displayRoomAvailability() {
    Room rooms[MAX_ROOMS];
    int count;
    loadRooms(rooms, count); // Load existing room information

    cout << "\nAvailable Rooms:\n";
    for (int i = 0; i < count; ++i) {
        if (rooms[i].status == 0) { // Check if room is available
            displayRoom(rooms[i]);
        }
    }
}

// Function to load rooms from file
void loadRooms(Room rooms[], int& count) {
    ifstream inFile(ROOMS_FILE, ios::binary);
    if (!inFile) {
        count = 0;
        return; // No rooms file found
    }

    count = 0;
    while (inFile.read((char*)&rooms[count], sizeof(Room))) {
        ++count;
    }

    inFile.close();
}

// Function to save rooms to file
void saveRooms(const Room rooms[], int count) {
    ofstream outFile(ROOMS_FILE, ios::binary);
    for (int i = 0; i < count; ++i) {
        outFile.write((char*)&rooms[i], sizeof(Room));
    }
    outFile.close();
}

// Function to display room information
void displayRoom(const Room& room) {
    cout << "\nRoom Number: " << room.roomNumber;
    cout << "\nType AC/Non-AC: " << room.ac;
    cout << "\nType Comfort: " << room.type;
    cout << "\nType Size: " << room.stype;
    cout << "\nRent: " << room.rent;
    cout << "\nStatus: " << (room.status == 0 ? "Available" : (room.status == 1 ? "Reserved" : "Checked-In")) << endl;
}
