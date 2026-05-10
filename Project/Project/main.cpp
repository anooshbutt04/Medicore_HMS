#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "FileHandling.h"
#include "PatientMenu.h"
#include "DoctorMenu.h"
#include "AdminMenu.h"
#include "HospitalException.h"
#include "GUI.h"
#include <iostream>
using namespace std;

void runDoctorMenu(Doctor& d,
    Storage<Appointment>& appointments,
    Storage<Patient>& patients,
    Storage<Bill>& bills,
    Storage<Prescription>& prescriptions) {

    int choice = 0;
    while (true) {
        d.displayMenu();
        cout << "Enter choice: ";
        cin >> choice;
        if (choice == 1) viewTodaysAppointments(d, appointments, patients);
        else if (choice == 2) markComplete(d, appointments);
        else if (choice == 3) markNoShow(d, appointments, bills);
        else if (choice == 4) writePrescription(d, appointments, prescriptions);
        else if (choice == 5) viewPatientHistory(d, patients, appointments, prescriptions);
        else if (choice == 6) { cout << "Logging out..." << endl; break; }
        else cout << "Invalid choice. Enter 1-6." << endl;
    }
}

void runAdminMenu(Admin* admin,
    Storage<Patient>& patients,
    Storage<Doctor>& doctors,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills,
    Storage<Prescription>& prescriptions) {

    int choice = 0;
    while (true) {
        admin->displayMenu();
        cout << "Enter choice: ";
        cin >> choice;
        if (choice == 1)  addDoctor(doctors);
        else if (choice == 2)  removeDoctor(doctors, appointments);
        else if (choice == 3)  viewAllPatients(patients, bills);
        else if (choice == 4)  viewAllDoctors(doctors);
        else if (choice == 5)  viewAllAppointments(appointments, patients, doctors);
        else if (choice == 6)  viewUnpaidBills(bills, patients);
        else if (choice == 7)  dischargePatient(patients, appointments, bills, prescriptions);
        else if (choice == 8)  viewSecurityLog();
        else if (choice == 9)  generateDailyReport(appointments, bills, patients, doctors);
        else if (choice == 10) { cout << "Logging out..." << endl; break; }
        else cout << "Invalid choice. Enter 1-10." << endl;
    }
}

// ── helper: convert int to std::string ───────────────────────
std::string intToStr(int n) {
    if (n == 0) return "0";
    std::string s = "";
    while (n > 0) { s = (char)('0' + n % 10) + s; n /= 10; }
    return s;
}

int main() {

    Storage<Patient>      patients;
    Storage<Doctor>       doctors;
    Storage<Appointment>  appointments;
    Storage<Bill>         bills;
    Storage<Prescription> prescriptions;

    try { FileHandler::loadPatients(patients); }
    catch (FileNotFoundException& e) { cout << "Warning: " << e.what() << endl; }

    try { FileHandler::loadDoctors(doctors); }
    catch (FileNotFoundException& e) { cout << "Warning: " << e.what() << endl; }

    try { FileHandler::loadAppointments(appointments); }
    catch (FileNotFoundException& e) { cout << "Warning: " << e.what() << endl; }

    try { FileHandler::loadBills(bills); }
    catch (FileNotFoundException& e) { cout << "Warning: " << e.what() << endl; }

    try { FileHandler::loadPrescriptions(prescriptions); }
    catch (FileNotFoundException& e) { cout << "Warning: " << e.what() << endl; }

    Admin* admin = nullptr;
    try {
        admin = FileHandler::loadAdmin();
        if (admin == nullptr) {
            cout << "FATAL: admin.txt missing or corrupt." << endl;
            return 1;
        }
    }
    catch (FileNotFoundException& e) {
        cout << "FATAL: " << e.what() << endl;
        return 1;
    }

    GUI gui;
    bool running = true;

    while (running) {

        LoginResult role = gui.runHomepage();
        if (role == LoginResult::Quit) break;

        std::string roleTitle = "";
        if (role == LoginResult::Patient) roleTitle = "Patient";
        else if (role == LoginResult::Doctor)  roleTitle = "Doctor";
        else if (role == LoginResult::Admin)   roleTitle = "Admin";

   
        int  loginID = 0;
        std::string loginPwd = "";
        bool loggedIn = false;
        int  failCount = 0;
        std::string errorMsg = "";

        while (failCount < 3) {

            // show login screen — pass error msg to display
            bool gotCreds = gui.runLoginScreen(
                roleTitle, loginID, loginPwd, errorMsg);

            if (!gotCreds) break;  // clicked Back

            // convert pwd to char*
            char pwdCStr[50];
            int pi = 0;
            for (char c : loginPwd) pwdCStr[pi++] = c;
            pwdCStr[pi] = '\0';

            // check credentials
            bool valid = false;

            if (role == LoginResult::Patient) {
                Patient* p = patients.findbyID(loginID);
                if (p != nullptr && p->checkPwd(pwdCStr)) {
                    gui.showMessage("Welcome, " +
                        std::string(p->getName()) + "!");
                    gui.runPatientMenuGUI(*p, doctors, appointments, bills, prescriptions);
                    loggedIn = true; valid = true;
                }
            }
            else if (role == LoginResult::Doctor) {
                Doctor* d = doctors.findbyID(loginID);
                if (d != nullptr && d->checkPwd(pwdCStr)) {
                    gui.showMessage("Welcome, Dr. " +
                        std::string(d->getName()) + "!");
                    gui.runDoctorMenuGUI(*d, appointments, patients, bills, prescriptions);

                    loggedIn = true; valid = true;
                }
            }
            else if (role == LoginResult::Admin) {
                if (admin != nullptr &&
                    admin->getID() == loginID &&
                    admin->checkPwd(pwdCStr)) {
                    gui.showMessage("Welcome, " +
                        std::string(admin->getName()) + "!");
                    gui.runAdminMenuGUI(admin, patients, doctors, appointments, bills, prescriptions);

                    loggedIn = true; valid = true;
                }
            }

            if (valid) break;

            // wrong credentials
            failCount++;
            int remaining = 3 - failCount;

            if (remaining > 0) {
                errorMsg = "Invalid ID or password. " +
                    intToStr(remaining) + " attempt(s) left.";
            }
            else {
                // 3 fails — log and show locked message
                char idStr[20];
                std::string ids = intToStr(loginID);
                for (int i = 0; i <= (int)ids.size(); i++)
                    idStr[i] = ids[i];

                const char* roleStr =
                    role == LoginResult::Patient ? "Patient" :
                    role == LoginResult::Doctor ? "Doctor" : "Admin";

                FileHandler::logSecurityEvent(
                    roleStr, idStr, "LOCKED");

                gui.showMessage(
                    "Account locked after 3 failed attempts.\nContact admin.");
            }
        }
        // after logout or locked — loop back to homepage automatically
    }

    delete admin;
    admin = nullptr;
    return 0;
}