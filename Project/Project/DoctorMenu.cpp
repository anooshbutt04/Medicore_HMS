#include "DoctorMenu.h"
#include "Validator.h" 
#include <ctime>

int compareTimeSlots(const char* a, const char* b) {
   
    for (int i = 0; i <= 1; i++) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
    }
    // then minutes (chars 3-4)
    for (int i = 3; i <= 4; i++) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
    }
    return 0;
}

void sortByTimeSlot(Appointment* arr, int size) {
    // bubble sort by time slot ascending
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (compareTimeSlots(arr[j].gettimeSlot(),
                arr[j + 1].gettimeSlot()) > 0) {
                Appointment temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// helper — checks if two char arrays are equal
bool strEqual(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}

void viewTodaysAppointments(Doctor& d,
    Storage<Appointment>& appointments,
    Storage<Patient>& patients) {
    // get today's date
    char today[11];
    Validator::getTodayDate(today);

    // collect today's appointments for this doctor
    Appointment todayApps[100];
    int count = 0;

    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getALL()[i];

        // check doctor matches
        if (a.getdocID() != d.getID()) continue;

        // check date matches today
        if (!strEqual(a.getDate(), today)) continue;

        todayApps[count++] = a;
    }

    if (count == 0) {
        cout << "No appointments scheduled for today." << endl;
        return;
    }

    // sort by time slot ascending
    sortByTimeSlot(todayApps, count);

    // display
    for (int i = 0; i < count; i++) {
        Patient* pat = patients.findbyID(todayApps[i].getpatientID());
        cout << "Appointment ID: " << todayApps[i].getID()
            << " | Patient: "
            << (pat ? pat->getName() : "Unknown")
            << " | Time: " << todayApps[i].gettimeSlot()
            << " | Status: " << todayApps[i].getStatus()
            << endl;
    }
}

void markComplete(Doctor& d,
    Storage<Appointment>& appointments) {

    // show today's pending appointments for this doctor
    char today[11];
    Validator::getTodayDate(today);

    bool hasPending = false;
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getALL()[i];
        if (a.getdocID() == d.getID() &&
            strEqual(a.getDate(), today) &&
            Validator::compareInsensitive(a.getStatus(), "pending")) {
            cout << "ID: " << a.getID()
                << " | Time: " << a.gettimeSlot()
                << " | Status: " << a.getStatus()
                << endl;
            hasPending = true;
        }
    }

    if (!hasPending) {
        cout << "No pending appointments for today." << endl;
        return;
    }

    int appID;
    cout << "Enter Appointment ID: ";
    cin >> appID;

    // validate — must belong to this doctor
    // must be pending and today
    Appointment* target = nullptr;
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getALL()[i];
        if (a.getID() == appID &&
            a.getdocID() == d.getID() &&
            strEqual(a.getDate(), today) &&
            Validator::compareInsensitive(a.getStatus(), "pending")) {
            target = &a;
            break;
        }
    }

    if (target == nullptr) {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    target->setStatus("completed");
    FileHandler::updateAppointment(*target);
    cout << "Appointment marked as completed." << endl;
}

// ─────────────────────────────────────────
// 3. MARK APPOINTMENT NO-SHOW
// ─────────────────────────────────────────
void markNoShow(Doctor& d,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills) {

    char today[11];
    Validator::getTodayDate(today);

    // show today's pending appointments
    bool hasPending = false;
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getALL()[i];
        if (a.getdocID() == d.getID() &&
            strEqual(a.getDate(), today) &&
            Validator::compareInsensitive(a.getStatus(), "pending")) {
            cout << "ID: " << a.getID()
                << " | Time: " << a.gettimeSlot()
                << " | Status: " << a.getStatus()
                << endl;
            hasPending = true;
        }
    }

    if (!hasPending) {
        cout << "No pending appointments for today." << endl;
        return;
    }

    int appID;
    cout << "Enter Appointment ID: ";
    cin >> appID;

    // validate
    Appointment* target = nullptr;
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getALL()[i];
        if (a.getID() == appID &&
            a.getdocID() == d.getID() &&
            strEqual(a.getDate(), today) &&
            Validator::compareInsensitive(a.getStatus(), "pending")) {
            target = &a;
            break;
        }
    }

    if (target == nullptr) {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    // update appointment to noshow
    target->setStatus("noshow");
    FileHandler::updateAppointment(*target);

    // update corresponding bill to cancelled
    // NO refund for noshow
    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getALL()[i];
        if (b.getappID() == appID) {
            b.setstatus("cancelled");
            FileHandler::updateBill(b);
            break;
        }
    }

    cout << "Appointment marked as no-show." << endl;
}

void writePrescription(Doctor& d,
    Storage<Appointment>& appointments,
    Storage<Prescription>& prescriptions) {

    int appID;
    cout << "Enter Appointment ID: ";
    cin >> appID;

    // validate — must belong to this doctor and be completed
    Appointment* target = nullptr;
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getALL()[i];
        if (a.getID() == appID &&
            a.getdocID() == d.getID() &&
            Validator::compareInsensitive(a.getStatus(), "completed")) {
            target = &a;
            break;
        }
    }

    if (target == nullptr) {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    // check prescription doesnt already exist for this appointment
    for (int i = 0; i < prescriptions.size(); i++) {
        if (prescriptions.getALL()[i].getAppointmentID() == appID) {
            cout << "Prescription already written "
                << "for this appointment." << endl;
            return;
        }
    }

    // read medicines (max 499 chars, truncate silently)
    char medicines[500];
    cout << "Enter medicines "
        << "(format: MedicineName Dosage; "
        << "e.g. Paracetamol 500mg;Amoxicillin 250mg): ";
    cin.ignore();   // clear buffer before getline
    cin.getline(medicines, 500);

    // read notes (max 299 chars, truncate silently)
    char notes[300];
    cout << "Enter notes (max 300 chars): ";
    cin.getline(notes, 300);

    // generate new prescription ID
    int newPresID = 1;
    for (int i = 0; i < prescriptions.size(); i++) {
        if (prescriptions.getALL()[i].getID() >= newPresID) {
            newPresID = prescriptions.getALL()[i].getID() + 1;
        }
    }

    // get today's date for prescription
    char today[11];
    Validator::getTodayDate(today);

    // create prescription
    Prescription newPres(newPresID,
        target->getpatientID(),
        appID,
        d.getID(),
        today,
        medicines,
        notes);

    prescriptions.add(newPres);
    FileHandler::appendPrescriptions(newPres);

    cout << "Prescription saved." << endl;
}


void viewPatientHistory(Doctor& d,
    Storage<Patient>& patients,
    Storage<Appointment>& appointments,
    Storage<Prescription>& prescriptions) {

    int patID;
    cout << "Enter Patient ID: ";
    cin >> patID;

    // validate patient exists
    Patient* pat = patients.findbyID(patID);
    if (pat == nullptr) {
        cout << "Access denied. You can only view " 
            << "records of your own patients." << endl;
        return;
    }

    // validate patient has at least one completed
    // appointment with THIS doctor
    bool hasCompleted = false;
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getALL()[i];
        if (a.getpatientID() == patID &&
            a.getdocID() == d.getID() &&
            Validator::compareInsensitive(a.getStatus(), "completed")) {
            hasCompleted = true;
            break;
        }
    }

    if (!hasCompleted) {
        cout << "Access denied. You can only view "
            << "records of your own patients." << endl;
        return;
    }

    // collect prescriptions written by THIS doctor for this patient
    Prescription patPres[100];
    int count = 0;

    for (int i = 0; i < prescriptions.size(); i++) {
        Prescription& pr = prescriptions.getALL()[i];
        if (pr.getPatientID() == patID &&
            pr.getDoctorID() == d.getID()) {
            patPres[count++] = pr;
        }
    }

    if (count == 0) {
        cout << "No prescriptions found for this patient." << endl;
        return;
    }

    // sort descending by date (most recent first)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (Validator::compareDates(patPres[j].getDate(),
                patPres[j + 1].getDate()) < 0) {
                Prescription temp = patPres[j];
                patPres[j] = patPres[j + 1];
                patPres[j + 1] = temp;
            }
        }
    }

    // display
    for (int i = 0; i < count; i++) {
        cout << patPres[i] << endl;
    }
}