#define _CRT_SECURE_NO_WARNINGS
#include "AdminMenu.h"

void sortAppointmentsAdminDesc(Appointment* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (Validator::compareDates(arr[j].getDate(),
                arr[j + 1].getDate()) < 0) {
                Appointment temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

//add doc
void addDoctor(Storage<Doctor>& doctors) {

    char name[50];
    char spec[50];
    char contact[12];
    char pwd[50];
    float fee = 0.0f;

    cout << "\n--- Add New Doctor ---" << endl;

    cout << "Enter name: ";
    cin.ignore();
    cin.getline(name, 50);

    if (Validator::getLength(name) == 0) {
        cout << "Name cannot be empty." << endl;
        return;
    }

    cout << "Enter specialization: ";
    cin.getline(spec, 50);

    if (Validator::getLength(spec) == 0) {
        cout << "Specialization cannot be empty." << endl;
        return;
    }

    bool validContact = false;
    for (int attempt = 0; attempt < 3 && !validContact; attempt++) {
        cout << "Enter contact (11 digits): ";
        cin >> contact;
        if (Validator::isValidContact(contact)) {
            validContact = true;
        }
        else {
            cout << "Invalid contact. Must be 11 digits." << endl;
        }
    }
    if (!validContact) {
        cout << "Too many invalid attempts." << endl;
        return;
    }

    bool validPwd = false;
    for (int attempt = 0; attempt < 3 && !validPwd; attempt++) {
        cout << "Enter password (min 6 chars): ";
        cin >> pwd;
        if (Validator::isValidPass(pwd)) {
            validPwd = true;
        }
        else {
            cout << "Password too short. Minimum 6 characters." << endl;
        }
    }
    if (!validPwd) {
        cout << "Too many invalid attempts." << endl;
        return;
    }

    bool validFee = false;
    for (int attempt = 0; attempt < 3 && !validFee; attempt++) {
        cout << "Enter consultation fee (PKR): ";
        cin >> fee;
        if (Validator::isValidAmount(fee)) {
            validFee = true;
        }
        else {
            cout << "Fee must be greater than 0." << endl;
        }
    }
    if (!validFee) {
        cout << "Too many invalid attempts." << endl;
        return;
    }

    int newID = 1;
    for (int i = 0; i < doctors.size(); i++) {
        if (doctors.getALL()[i].getID() >= newID) {
            newID = doctors.getALL()[i].getID() + 1;
        }
    }

    Doctor newDoc(newID, name, spec, contact, pwd, fee);
    doctors.add(newDoc);
    FileHandler::appendDoctors(newDoc);

    cout << "Doctor added successfully. ID: " << newID << endl;
}

// remove doc
void removeDoctor(Storage<Doctor>& doctors,
    Storage<Appointment>& appointments) {

    if (doctors.size() == 0) {
        cout << "No doctors registered." << endl;
        return;
    }

    cout << "\n--- All Doctors ---" << endl;
    for (int i = 0; i < doctors.size(); i++) {
        cout << doctors.getALL()[i] << endl;
    }

    int docID;
    cout << "Enter Doctor ID to remove: ";
    cin >> docID;

    Doctor* target = doctors.findbyID(docID);
    if (target == nullptr) {
        cout << "Doctor not found." << endl;
        return;
    }

    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getALL()[i];
        if (a.getdocID() == docID &&
            Validator::compareInsensitive(a.getStatus(), "pending")) {
            cout << "Cannot remove doctor — they have pending appointments." << endl;
            return;
        }
    }

    doctors.removeID(docID);
    FileHandler::deleteDoctor(docID);

    cout << "Doctor removed successfully." << endl;
}

// alll patients
void viewAllPatients(Storage<Patient>& patients,
    Storage<Bill>& bills) {

    if (patients.size() == 0) {
        cout << "No patients registered." << endl;
        return;
    }

    cout << "\n--- All Patients ---" << endl;
    for (int i = 0; i < patients.size(); i++) {
        Patient& p = patients.getALL()[i];
        cout << p << endl;

        int unpaidCount = 0;
        for (int j = 0; j < bills.size(); j++) {
            Bill& b = bills.getALL()[j];
            if (b.getPatientID() == p.getID() &&
                Validator::compareInsensitive(b.getStatus(), "unpaid")) {
                unpaidCount++;
            }
        }
        cout << "  Unpaid Bills: " << unpaidCount << endl;
        cout << "-----------------------------" << endl;
    }
}

// all docs
void viewAllDoctors(Storage<Doctor>& doctors) {

    if (doctors.size() == 0) {
        cout << "No doctors registered." << endl;
        return;
    }

    cout << "\n--- All Doctors ---" << endl;
    for (int i = 0; i < doctors.size(); i++) {
        cout << doctors.getALL()[i] << endl;
        cout << "-----------------------------" << endl;
    }
}

// all appointments
void viewAllAppointments(Storage<Appointment>& appointments,
    Storage<Patient>& patients,
    Storage<Doctor>& doctors) {

    if (appointments.size() == 0) {
        cout << "No appointments found." << endl;
        return;
    }

    Appointment sorted[100];
    int count = 0;
    for (int i = 0; i < appointments.size(); i++) {
        sorted[count++] = appointments.getALL()[i];
    }

    sortAppointmentsAdminDesc(sorted, count);

    cout << "\n--- All Appointments (Newest First) ---" << endl;
    for (int i = 0; i < count; i++) {
        Appointment& a = sorted[i];
        Patient* pat = patients.findbyID(a.getpatientID());
        Doctor* doc = doctors.findbyID(a.getdocID());

        cout << "App ID: " << a.getID()
            << " | Patient: " << (pat ? pat->getName() : "Unknown")
            << " | Doctor: " << (doc ? doc->getName() : "Unknown")
            << " | Date: " << a.getDate()
            << " | Time: " << a.gettimeSlot()
            << " | Status: " << a.getStatus()
            << endl;
    }
}

// unpaid bills
void viewUnpaidBills(Storage<Bill>& bills,
    Storage<Patient>& patients) {

    bool found = false;
    time_t now = time(0);

    cout << "\n--- Unpaid Bills ---" << endl;

    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getALL()[i];

        if (!Validator::compareInsensitive(b.getStatus(), "unpaid"))
            continue;

        found = true;

       //parsinf the bill date
        const char* d = b.getDate();
        struct tm billTm = {};
        billTm.tm_year = (d[0] - '0') * 1000 + (d[1] - '0') * 100 +
            (d[2] - '0') * 10 + (d[3] - '0') - 1900;
        billTm.tm_mon = (d[5] - '0') * 10 + (d[6] - '0') - 1;
        billTm.tm_mday = (d[8] - '0') * 10 + (d[9] - '0');
        time_t billTime = mktime(&billTm);

        double daysDiff = difftime(now, billTime) / 86400.0;

        Patient* pat = patients.findbyID(b.getPatientID());

        cout << b;
        if (pat) cout << " | Patient: " << pat->getName();
        if (daysDiff > 7.0) cout << " [OVERDUE]";
        cout << endl;
    }

    if (!found) {
        cout << "No unpaid bills found." << endl;
    }
}

// dsicharge
void dischargePatient(Storage<Patient>& patients,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills,
    Storage<Prescription>& prescriptions) {

    if (patients.size() == 0) {
        cout << "No patients registered." << endl;
        return;
    }

    cout << "\n--- All Patients ---" << endl;
    for (int i = 0; i < patients.size(); i++) {
        cout << patients.getALL()[i] << endl;
    }

    int patID;
    cout << "Enter Patient ID to discharge: ";
    cin >> patID;

    Patient* target = patients.findbyID(patID);
    if (target == nullptr) {
        cout << "Patient not found." << endl;
        return;
    }

    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getALL()[i];
        if (b.getPatientID() == patID &&
            Validator::compareInsensitive(b.getStatus(), "unpaid")) {
            cout << "Cannot discharge — patient has unpaid bills." << endl;
            return;
        }
    }

    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getALL()[i];
        if (a.getpatientID() == patID &&
            Validator::compareInsensitive(a.getStatus(), "pending")) {
            cout << "Cannot discharge — patient has pending appointments." << endl;
            return;
        }
    }

    FileHandler::appendToDischarge(patID, patients, appointments,
        bills, prescriptions);

    FileHandler::deletePatient(patID);
    FileHandler::deleteAppointmentsByPatient(patID);
    FileHandler::deleteBillsByPatient(patID);
    FileHandler::deletePrescriptionsByPatient(patID);

    patients.removeID(patID);

    for (int i = appointments.size() - 1; i >= 0; i--) {
        if (appointments.getALL()[i].getpatientID() == patID) {
            appointments.removeID(appointments.getALL()[i].getID());
        }
    }
    for (int i = bills.size() - 1; i >= 0; i--) {
        if (bills.getALL()[i].getPatientID() == patID) {
            bills.removeID(bills.getALL()[i].getID());
        }
    }
    for (int i = prescriptions.size() - 1; i >= 0; i--) {
        if (prescriptions.getALL()[i].getPatientID() == patID) {
            prescriptions.removeID(prescriptions.getALL()[i].getID());
        }
    }

    cout << "Patient discharged and archived successfully." << endl;
}

// sec log
void viewSecurityLog() {
    ifstream fin;
    fin.open("security_log.txt");

    if (!fin) {
        cout << "Security log not found or empty." << endl;
        return;
    }

    cout << "\n--- Security Log ---" << endl;
    cout << "Timestamp            | Role    | Entered ID | Result" << endl;
    cout << "-----------------------------------------------------" << endl;

    char line[300];
    bool hasEntries = false;

    while (fin.getline(line, 300)) {
        if (line[0] == '\0') continue;
        cout << line << endl;
        hasEntries = true;
    }

    if (!hasEntries) {
        cout << "No log entries found." << endl;
    }

    fin.close();
}

// generate dailt report
void generateDailyReport(Storage<Appointment>& appointments,
    Storage<Bill>& bills,
    Storage<Patient>& patients,
    Storage<Doctor>& doctors) {

    char today[11];
    Validator::getTodayDate(today);

    cout << "\n========================================" << endl;
    cout << "       MEDICORE DAILY REPORT" << endl;
    cout << "       Date: " << today << endl;
    cout << "========================================" << endl;

    int pending = 0, completed = 0, cancelled = 0, noshow = 0;

    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getALL()[i];

        bool sameDate = true;
        for (int k = 0; k < 10; k++) {
            if (a.getDate()[k] != today[k]) { sameDate = false; break; }
        }
        if (!sameDate) continue;

        if (Validator::compareInsensitive(a.getStatus(), "pending"))   pending++;
        else if (Validator::compareInsensitive(a.getStatus(), "completed")) completed++;
        else if (Validator::compareInsensitive(a.getStatus(), "cancelled")) cancelled++;
        else if (Validator::compareInsensitive(a.getStatus(), "noshow"))    noshow++;
    }

    cout << "\n[Appointments Today]" << endl;
    cout << "  Pending   : " << pending << endl;
    cout << "  Completed : " << completed << endl;
    cout << "  Cancelled : " << cancelled << endl;
    cout << "  No-Show   : " << noshow << endl;

    double revenue = 0.0;
    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getALL()[i];
        if (!Validator::compareInsensitive(b.getStatus(), "paid")) continue;

        bool sameDate = true;
        const char* bd = b.getDate();
        for (int k = 0; k < 10; k++) {
            if (bd[k] != today[k]) { sameDate = false; break; }
        }
        if (sameDate) revenue += b.getamount();
    }

    cout << "\n[Revenue Today]" << endl;
    cout << "  Total Paid: PKR " << revenue << endl;

    cout << "\n[Patients with Outstanding Unpaid Bills]" << endl;
    bool anyUnpaid = false;
    for (int i = 0; i < patients.size(); i++) {
        Patient& p = patients.getALL()[i];
        double owed = 0.0;
        for (int j = 0; j < bills.size(); j++) {
            Bill& b = bills.getALL()[j];
            if (b.getPatientID() == p.getID() &&
                Validator::compareInsensitive(b.getStatus(), "unpaid")) {
                owed += b.getamount();
            }
        }
        if (owed > 0.0) {
            cout << "  " << p.getName()
                << " (ID: " << p.getID() << ")"
                << " — Owes PKR " << owed << endl;
            anyUnpaid = true;
        }
    }
    if (!anyUnpaid) cout << "  None." << endl;

    cout << "\n[Doctor-wise Appointments Today]" << endl;
    for (int d = 0; d < doctors.size(); d++) {
        Doctor& doc = doctors.getALL()[d];
        int docTotal = 0;
        for (int i = 0; i < appointments.size(); i++) {
            Appointment& a = appointments.getALL()[i];
            if (a.getdocID() != doc.getID()) continue;
            bool sameDate = true;
            for (int k = 0; k < 10; k++) {
                if (a.getDate()[k] != today[k]) { sameDate = false; break; }
            }
            if (sameDate) docTotal++;
        }
        if (docTotal > 0) {
            cout << "  Dr. " << doc.getName()
                << " (" << doc.getSpec() << ")"
                << " — " << docTotal << " appointment(s)" << endl;
        }
    }

    cout << "\n========================================" << endl;
}
