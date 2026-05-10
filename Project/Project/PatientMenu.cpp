#include "PatientMenu.h"



void sortAppointmentsAsc(Appointment* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (Validator::compareDates(arr[j].getDate(),
                arr[j + 1].getDate()) > 0) {
                Appointment temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void sortAppointmentsDesc(Appointment* arr, int size) {
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



void bookAppointment(Patient& p,
    Storage<Doctor>& doctors,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills) {

    char spec[50];
    cout << "Enter specialization to search: ";
    cin >> spec;

    
    bool found = false;
    for (int i = 0; i < doctors.size(); i++) {
        Doctor* d = &doctors.getALL()[i];
        if (Validator::compareInsensitive(d->getSpec(), spec)) {
            cout << *d << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No doctors available for that specialization." << endl;
        return;
    }

    int docID;
    cout << "Enter Doctor ID: ";
    cin >> docID;

    Doctor* chosenDoc = doctors.findbyID(docID);
    if (chosenDoc == nullptr) {
        cout << "Doctor not found." << endl;
        return;
    }

    // verify specialization matches

    if (!Validator::compareInsensitive(chosenDoc->getSpec(), spec)) {
        cout << "Doctor not found." << endl;
        return;
    }

    
    char date[11];
    int dateAttempts = 0;
    bool validDate = false;

    while (dateAttempts < 3 && !validDate) {
        cout << "Enter date (YYYY-MM-DD): ";
        cin >> date;

        if (Validator::isValidDate(date)) {
            validDate = true;
        }
        else {
            cout << "Invalid date. Use format YYYY-MM-DD "
                << "(e.g. 2026-06-15)." << endl;
            dateAttempts++;
        }
    }

    if (!validDate) {
        cout << "Too many invalid date attempts." << endl;
        return;
    }

 //available:

    const char* allSlots[8] = {
        "09:00","10:00","11:00","12:00",
        "13:00","14:00","15:00","16:00"
    };

    cout << "Available slots for " << date << ":" << endl;
    for (int s = 0; s < 8; s++) {
        bool taken = false;
        for (int i = 0; i < appointments.size(); i++) {
            Appointment& a = appointments.getALL()[i];
            if (a.getdocID() == docID) {
                bool sameDate = true;
                for (int k = 0; k < 10; k++) {
                    if (a.getDate()[k] != date[k]) {
                        sameDate = false;
                        break;
                    }
                }
                bool sameSlot = true;
                for (int k = 0; k < 5; k++) {
                    if (a.gettimeSlot()[k] != allSlots[s][k]) {
                        sameSlot = false;
                        break;
                    }
                }
                bool cancelled = Validator::compareInsensitive(
                    a.getStatus(), "cancelled");

                if (sameDate && sameSlot && !cancelled) {
                    taken = true;
                    break;
                }
            }
        }
        if (!taken) {
            cout << allSlots[s] << endl;
        }
    }

    char chosenSlot[6];
    bool slotBooked = false;

    while (!slotBooked) {
        cout << "Enter time slot (e.g. 09:00): ";
        cin >> chosenSlot;

        if (!Validator::isValidTimeSlot(chosenSlot)) {
            cout << "Invalid time slot. "
                << "Choose from: 09:00 10:00 11:00 12:00 "
                << "13:00 14:00 15:00 16:00" << endl;
            continue;
        }

        
        bool taken = false;
        for (int i = 0; i < appointments.size(); i++) {
            Appointment& a = appointments.getALL()[i];
            if (a.getdocID() == docID) {
                bool sameDate = true;
                for (int k = 0; k < 10; k++) {
                    if (a.getDate()[k] != date[k]) {
                        sameDate = false;
                        break;
                    }
                }
                bool sameSlot = true;
                for (int k = 0; k < 5; k++) {
                    if (a.gettimeSlot()[k] != chosenSlot[k]) {
                        sameSlot = false;
                        break;
                    }
                }
                bool cancelled = Validator::compareInsensitive(
                    a.getStatus(), "cancelled");

                if (sameDate && sameSlot && !cancelled) {
                    taken = true;
                    break;
                }
            }
        }

        if (taken) {
            try {
                throw SlotUnavailableException(
                    "Slot already taken. Pick another.");
            }
            catch (SlotUnavailableException& e) {
                cout << e.what() << endl;

                

                cout << "Available slots:" << endl;
                for (int s = 0; s < 8; s++) {
                    bool slotTaken = false;
                    for (int i = 0; i < appointments.size(); i++) {
                        Appointment& a = appointments.getALL()[i];
                        if (a.getdocID() == docID) {
                            bool sd = true;
                            for (int k = 0; k < 10; k++) {
                                if (a.getDate()[k] != date[k]) {
                                    sd = false; break;
                                }
                            }
                            bool ss = true;
                            for (int k = 0; k < 5; k++) {
                                if (a.gettimeSlot()[k] != allSlots[s][k]) {
                                    ss = false; break;
                                }
                            }
                            bool canc = Validator::compareInsensitive(
                                a.getStatus(), "cancelled");
                            if (sd && ss && !canc) {
                                slotTaken = true; break;
                            }
                        }
                    }
                    if (!slotTaken) cout << allSlots[s] << endl;
                }
            }
        }
        else {
            slotBooked = true;
        }
    }

  
    try {
        if (p.getBalance() < chosenDoc->getFee()) {
            throw InsufficientFundsException(
                "Insufficient balance to book appointment.");
        }
    }
    catch (InsufficientFundsException& e) {
        cout << e.what() << endl;
        return;
    }

    
    int newAppID = 1;
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getALL()[i].getID() >= newAppID) {
            newAppID = appointments.getALL()[i].getID() + 1;
        }
    }

    int newBillID = 1;
    for (int i = 0; i < bills.size(); i++) {
        if (bills.getALL()[i].getID() >= newBillID) {
            newBillID = bills.getALL()[i].getID() + 1;
        }
    }

   
    Appointment newApp(newAppID, p.getID(), docID,
        date, chosenSlot, "pending");
    appointments.add(newApp);
    FileHandler::appendAppointments(newApp);

    char today[11];
    Validator::getTodayDate(today);

    Bill newBill(newBillID, p.getID(), newAppID,
        chosenDoc->getFee(), "unpaid", today);
    bills.add(newBill);
    FileHandler::appendBills(newBill);

    
    p -= chosenDoc->getFee();
    FileHandler::updatePatient(p);

    cout << "Appointment booked! ID: " << newAppID
        << " | Fee deducted: PKR " << chosenDoc->getFee()
        << " | Remaining balance: PKR " << p.getBalance() << endl;
}



void cancelAppointment(Patient& p,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills,
    Storage<Doctor>& doctors) {

    
    bool hasPending = false;
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getALL()[i];
        if (a.getpatientID() == p.getID() &&
            Validator::compareInsensitive(a.getStatus(), "pending")) {

            Doctor* d = doctors.findbyID(a.getdocID());
            cout << "ID: " << a.getID()
                << " | Dr. " << (d ? d->getName() : "Unknown")
                << " | Date: " << a.getDate()
                << " | Time: " << a.gettimeSlot()
                << endl;
            hasPending = true;
        }
    }

    if (!hasPending) {
        cout << "You have no pending appointments." << endl;
        return;
    }

    int cancelID;
    cout << "Enter Appointment ID to cancel: ";
    cin >> cancelID;

    // validate it belongs to this patient and is pending
    Appointment* target = nullptr;
    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getALL()[i];
        if (a.getID() == cancelID &&
            a.getpatientID() == p.getID() &&
            Validator::compareInsensitive(a.getStatus(), "pending")) {
            target = &a;
            break;
        }
    }

    if (target == nullptr) {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    // get  fee in case of refund
    Doctor* d = doctors.findbyID(target->getdocID());
    float refund = d ? d->getFee() : 0.0f;

    // update appointment to cancelled
    target->setStatus("cancelled");
    FileHandler::updateAppointment(*target);

   
    p += refund;
    FileHandler::updatePatient(p);

    // update  bill  
    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getALL()[i];
        if (b.getappID() == cancelID) {
            b.setstatus("cancelled");
            FileHandler::updateBill(b);
            break;
        }
    }

    cout << "Appointment cancelled. PKR "
        << refund << " refunded. "
        << "New balance: PKR " << p.getBalance() << endl;
}



void viewAppointments(Patient& p,
    Storage<Appointment>& appointments,
    Storage<Doctor>& doctors) {

    Appointment patientApps[100];
    int count = 0;

    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getALL()[i];
        if (a.getpatientID() == p.getID()) {
            patientApps[count++] = a;
        }
    }

    if (count == 0) {
        cout << "No appointments found." << endl;
        return;
    }

    sortAppointmentsAsc(patientApps, count);

    cout << "\n--- Your Appointments ---" << endl;
    for (int i = 0; i < count; i++) {
        Doctor* d = doctors.findbyID(patientApps[i].getdocID());
        cout << "ID: " << patientApps[i].getID()
            << " | Dr. " << (d ? d->getName() : "Unknown")
            << " | Spec: " << (d ? d->getSpec() : "Unknown")
            << " | Date: " << patientApps[i].getDate()
            << " | Time: " << patientApps[i].gettimeSlot()
            << " | Status: " << patientApps[i].getStatus()
            << endl;
    }
}


void viewMedicalRecords(Patient& p,
    Storage<Prescription>& prescriptions,
    Storage<Doctor>& doctors) {

    Prescription patPres[100];
    int count = 0;

    for (int i = 0; i < prescriptions.size(); i++) {
        Prescription& pr = prescriptions.getALL()[i];
        if (pr.getPatientID() == p.getID()) {
            patPres[count++] = pr;
        }
    }

    if (count == 0) {
        cout << "No medical records found." << endl;
        return;
    }

    // bubble sort  

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

    cout << "\n--- Your Medical Records ---" << endl;
    for (int i = 0; i < count; i++) {
        Doctor* d = doctors.findbyID(patPres[i].getDoctorID());
        cout << "Date: " << patPres[i].getDate()
            << " | Dr. " << (d ? d->getName() : "Unknown")
            << " | Medicines: " << patPres[i].getMedicines()
            << " | Notes: " << patPres[i].getNotes()
            << endl;
    }
}


void viewBills(Patient& p,
    Storage<Bill>& bills) {

    bool found = false;
    double totalUnpaid = 0.0;

    cout << "\n--- Your Bills ---" << endl;
    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getALL()[i];
        if (b.getPatientID() == p.getID()) {
            cout << b << endl;
            found = true;
            if (Validator::compareInsensitive(b.getStatus(), "unpaid")) {
                totalUnpaid += b.getamount();
            }
        }
    }

    if (!found) {
        cout << "No bills found." << endl;
        return;
    }

    cout << "Total outstanding: PKR " << totalUnpaid << endl;
}


void payBill(Patient& p,
    Storage<Bill>& bills) {

    bool hasUnpaid = false;
    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getALL()[i];
        if (b.getPatientID() == p.getID() &&
            Validator::compareInsensitive(b.getStatus(), "unpaid")) {
            cout << b << endl;
            hasUnpaid = true;
        }
    }

    if (!hasUnpaid) {
        cout << "No unpaid bills." << endl;
        return;
    }

    int billID;
    cout << "Enter Bill ID to pay: ";
    cin >> billID;

    // validate belongs to patient and is unpaid
    Bill* target = nullptr;
    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getALL()[i];
        if (b.getID() == billID &&
            b.getPatientID() == p.getID() &&
            Validator::compareInsensitive(b.getStatus(), "unpaid")) {
            target = &b;
            break;
        }
    }

    if (target == nullptr) {
        cout << "Invalid bill ID." << endl;
        return;
    }

    try {
        if (p.getBalance() < target->getamount()) {
            throw InsufficientFundsException(
                "Insufficient balance to pay bill.");
        }
    }
    catch (InsufficientFundsException& e) {
        cout << e.what() << endl;
        return;
    }

    // deduct using -= operator
    p -= (float)target->getamount();
    FileHandler::updatePatient(p);

    target->setstatus("paid");
    FileHandler::updateBill(*target);

    cout << "Bill paid successfully. "
        << "Remaining balance: PKR " << p.getBalance() << endl;
}



void topUpBalance(Patient& p) {
    int attempts = 0;

    while (attempts < 3) {
        float amount;
        cout << "Enter amount to add (PKR): ";
        cin >> amount;

        try {
            if (!Validator::isValidAmount(amount)) {
                throw InvalidInputException(
                    "Amount must be greater than 0.");
            }
            p += amount;
            FileHandler::updatePatient(p);
            cout << "Balance updated. New balance: PKR "
                << p.getBalance() << endl;
            return;
        }
        catch (InvalidInputException& e) {
            cout << e.what() << endl;
            attempts++;
        }
    }

    cout << "Too many invalid attempts." << endl;
}


void runPatientMenu(Patient& p,
    Storage<Doctor>& doctors,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills,
    Storage<Prescription>& prescriptions) {

    int choice = 0;

    while (true) {
        p.displayMenu();
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            bookAppointment(p, doctors, appointments, bills);
        }
        else if (choice == 2) {
            cancelAppointment(p, appointments, bills, doctors);
        }
        else if (choice == 3) {
            viewAppointments(p, appointments, doctors);
        }
        else if (choice == 4) {
            viewMedicalRecords(p, prescriptions, doctors);
        }
        else if (choice == 5) {
            viewBills(p, bills);
        }
        else if (choice == 6) {
            payBill(p, bills);
        }
        else if (choice == 7) {
            topUpBalance(p);
        }
        else if (choice == 8) {
            cout << "Logging out..." << endl;
            break;
        }
        else {
            cout << "Invalid choice. Enter 1-8." << endl;
        }
    }
}