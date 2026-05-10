#include "LoginSystem.h"

void showWelcome() {
    cout << "==========================================" << endl;
    cout << "         WELCOME TO MEDICORE PORTAL       " << endl;
    cout << "==========================================" << endl;
}

int showLogin(
    Storage<Patient>& patients,
    Storage<Doctor>& doctors,
    Admin* admin,
    Patient*& outPatient,
    Doctor*& outDoctor) {

    // initialise pointers to null
    outPatient = nullptr;
    outDoctor = nullptr;

    cout << "\nSelect role:" << endl;
    cout << "1. Patient" << endl;
    cout << "2. Doctor" << endl;
    cout << "3. Admin" << endl;
    cout << "Enter choice: ";

    int role;
    cin >> role;

    if (!Validator::isValidChoice(role, 1, 3)) {
        cout << "Invalid role." << endl;
        return 0;
    }

    // set role string for security log
    const char* roleStr = "Unknown";
    if (role == 1) roleStr = "Patient";
    else if (role == 2) roleStr = "Doctor";
    else if (role == 3) roleStr = "Admin";

    int id;
    char pwd[50];

    cout << "Enter ID: ";
    cin >> id;

    cout << "Enter password: ";
    cin >> pwd;

    // convert id to char for security log
    char idStr[20];
    int temp = id;
    if (temp == 0) {
        idStr[0] = '0'; idStr[1] = '\0';
    }
    else {
        char rev[20];
        int len = 0;
        while (temp > 0) {
            rev[len++] = '0' + (temp % 10);
            temp /= 10;
        }
        for (int i = 0; i < len; i++) {
            idStr[i] = rev[len - 1 - i];
        }
        idStr[len] = '\0';
    }

    if (role == 1) {
        int failCount = 0;

        while (failCount < 3) {
            Patient* p = patients.findbyID(id);

            if (p != nullptr && p->checkPwd(pwd)) {
                cout << "Login successful. Welcome, "
                    << p->getName() << "!" << endl;
                outPatient = p;
                return 1;
            }

            failCount++;
            if (failCount < 3) {
                cout << "Invalid ID or password. "
                    << (3 - failCount) << " attempt(s) remaining." << endl;
                cout << "Enter ID: ";
                cin >> id;
                cout << "Enter password: ";
                cin >> pwd;

                // update idStr for log
                int t2 = id;
                if (t2 == 0) {
                    idStr[0] = '0'; idStr[1] = '\0';
                }
                else {
                    char r2[20]; int l2 = 0;
                    while (t2 > 0) { r2[l2++] = '0' + (t2 % 10); t2 /= 10; }
                    for (int i = 0; i < l2; i++) idStr[i] = r2[l2 - 1 - i];
                    idStr[l2] = '\0';
                }
            }
        }

        cout << "Account locked. Contact admin." << endl;
        FileHandler::logSecurityEvent(roleStr, idStr, "LOCKED");
        return 0;
    }

    if (role == 2) {
        int failCount = 0;

        while (failCount < 3) {
            Doctor* d = doctors.findbyID(id);

            if (d != nullptr && d->checkPwd(pwd)) {
                cout << "Login successful. Welcome, Dr. "
                    << d->getName() << "!" << endl;
                outDoctor = d;
                return 2;
            }

            failCount++;
            if (failCount < 3) {
                cout << "Invalid ID or password. "
                    << (3 - failCount) << " attempt(s) remaining." << endl;
                cout << "Enter ID: ";
                cin >> id;
                cout << "Enter password: ";
                cin >> pwd;

                int t2 = id;
                if (t2 == 0) {
                    idStr[0] = '0'; idStr[1] = '\0';
                }
                else {
                    char r2[20]; int l2 = 0;
                    while (t2 > 0) { r2[l2++] = '0' + (t2 % 10); t2 /= 10; }
                    for (int i = 0; i < l2; i++) idStr[i] = r2[l2 - 1 - i];
                    idStr[l2] = '\0';
                }
            }
        }

        cout << "Account locked. Contact admin." << endl;
        FileHandler::logSecurityEvent(roleStr, idStr, "LOCKED");
        return 0;
    }


    if (role == 3) {
        int failCount = 0;

        while (failCount < 3) {
            if (admin != nullptr &&
                admin->getID() == id &&
                admin->checkPwd(pwd)) {
                cout << "Login successful. Welcome, "
                    << admin->getName() << "!" << endl;
                return 3;
            }

            failCount++;
            if (failCount < 3) {
                cout << "Invalid ID or password. "
                    << (3 - failCount) << " attempt(s) remaining." << endl;
                cout << "Enter ID: ";
                cin >> id;
                cout << "Enter password: ";
                cin >> pwd;

                int t2 = id;
                if (t2 == 0) {
                    idStr[0] = '0'; idStr[1] = '\0';
                }
                else {
                    char r2[20]; int l2 = 0;
                    while (t2 > 0) { r2[l2++] = '0' + (t2 % 10); t2 /= 10; }
                    for (int i = 0; i < l2; i++) idStr[i] = r2[l2 - 1 - i];
                    idStr[l2] = '\0';
                }
            }
        }

        cout << "Account locked. Contact admin." << endl;
        FileHandler::logSecurityEvent(roleStr, idStr, "LOCKED");
        return 0;
    }

    return 0;
}