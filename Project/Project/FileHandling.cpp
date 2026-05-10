#define _CRT_SECURE_NO_WARNINGS
#include "FileHandling.h"
#include "HospitalException.h"
#include <ctime>


void FileHandler::parseLine(const char* line,
    char input[][200], int& inputCount) {
    int i = 0;
    int j = 0;
    inputCount= 0;

    while (line[i] != '\0') {
        if (line[i] == ',') {
            input[inputCount][j] = '\0';
            inputCount++;
            j = 0;
        }
        else {
            input[inputCount][j] = line[i];
            j++;
        }
        i++;
    }
    
    input[inputCount][j] = '\0';
    inputCount++;
}

int FileHandler::charToint(const char* str) {
    int result = 0;
    int i = 0;
    while (str[i] != '\0') {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return result;
}


float FileHandler::charToFloat(const char* str) {
    float result = 0.0;
    float decimal = 0.1f;
    bool afterDot = false;
    int i = 0;

    while (str[i] != '\0') {
        if (str[i] == '.') {
            afterDot = true;
        }
        else if (!afterDot) {
            result = result * 10 + (str[i] - '0');
        }
        else {
            result = result + (str[i] - '0') * decimal;
            decimal *= 0.1f;
        }
        i++;
    }
    return result;
}

void FileHandler::intToChar(int num, char* str) {
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    char temp[20];
    int i = 0;

    while (num > 0) {
        temp[i] = '0' + (num % 10);
        
        num /= 10;
        i++;
    }

    int len = i;
    for (int j = 0; j < len; j++) {
        str[j] = temp[len - 1 - j];
    }
    str[len] = '\0';
}

void FileHandler::floatToChar(float num, char* str) {
    
    int intPart = (int)num;
    float decPart = num - intPart;

    char intStr[20];
    intToChar(intPart, intStr);

    int i = 0;
    while (intStr[i] != '\0') {
        str[i] = intStr[i];
        i++;
    }

   
    str[i] = '.';
    i++;

      decPart *= 100;
    int decInt = (int)decPart;

    char decStr[10];
    if (decInt < 10) {
        // e.g. 5000.05 — need leading zero
        str[i] = '0';
        i++;
        intToChar(decInt, decStr);
    }
    else {
        intToChar(decInt, decStr);
    }

    int j = 0;
    while (decStr[j] != '\0') {
        str[i] = decStr[j];
        i++;
        j++;
    }
    str[i] = '\0';
}

void FileHandler::loadPatients(Storage<Patient>& storage) {
    ifstream fin;
    fin.open("patients.txt");

    if (!fin) {
        throw FileNotFoundException("Cannot open patients.txt");
    }

    char line[500];
    while (fin.getline(line, 500)) {
       
        if (line[0] == '\0') continue;

        char input[20][200];
        int inputCount = 0;
        parseLine(line, input, inputCount);

        if (inputCount < 7) continue;  

        int id = charToint(input[0]);
        int age = charToint(input[2]);
        char gender = input[3][0];    
        
        float balance = charToFloat(input[6]);

        Patient p(id, input[1], input[5],
            age, gender, input[4], balance);
        storage.add(p);
    }

    fin.close();
}

void FileHandler::loadDoctors(Storage<Doctor>& storage) {
    ifstream fin;
    fin.open("doctors.txt");

    if (!fin) {
        throw FileNotFoundException("Cannot open doctors.txt");
    }

    char line[500];
    while (fin.getline(line, 500)) {
        if (line[0] == '\0') continue;

        char input[20][200];
        int inputCount = 0;
        parseLine(line, input, inputCount);

        // doctors.txt format:
       
        if (inputCount < 6) continue;

        int id = charToint(input[0]);
        float fee = charToFloat(input[5]);

        Doctor d(id, input[1], input[2],
            input[3], input[4], fee);
        storage.add(d);
    }

    fin.close();
}

void FileHandler::loadAppointments(Storage<Appointment>& storage) {
    ifstream fin;
    fin.open("appointments.txt");

    if (!fin) {
        throw FileNotFoundException("Cannot open appointments.txt");
    }

    char line[500];
    while (fin.getline(line, 500)) {
        if (line[0] == '\0') continue;

        char input[20][200];
        int inputCount = 0;
        parseLine(line, input, inputCount);

       
        if (inputCount < 6) continue;

        int appID = charToint(input[0]);
        int patientID = charToint(input[1]);
        int doctorID = charToint(input[2]);

        Appointment a(appID, patientID, doctorID,
            input[3], input[4], input[5]);
        storage.add(a);
    }

    fin.close();
}

void FileHandler::loadBills(Storage<Bill>& storage) {
    ifstream fin;
    fin.open("bills.txt");

    if (!fin) {
        throw FileNotFoundException("Cannot open bills.txt");
    }

    char line[500];
    while (fin.getline(line, 500)) {
        if (line[0] == '\0') continue;

        char input[20][200];
        int inputCount = 0;
        parseLine(line, input, inputCount);

        if (inputCount < 6) continue;

        int billID = charToint(input[0]);
        int patID = charToint(input[1]);
        int appID = charToint(input[2]);
        float amount = charToFloat(input[3]);

        Bill b(billID, patID, appID, amount, input[4], input[5]);
        storage.add(b);
    }

    fin.close();
}

void FileHandler::loadPrescriptions(Storage<Prescription>& storage) {
    ifstream fin;
    fin.open("prescriptions.txt");

    if (!fin) {
        throw FileNotFoundException("Cannot open prescriptions.txt");
    }

    char line[600];
    while (fin.getline(line, 600)) {
        if (line[0] == '\0') continue;

        char input[20][200];
        int inputCount = 0;
        parseLine(line, input, inputCount);

        if (inputCount < 7) continue;

        int presID = charToint(input[0]);
        int appID = charToint(input[1]);
        int patID = charToint(input[2]);
        int docID = charToint(input[3]);

        Prescription p(presID, appID, patID, docID,
            input[4], input[5], input[6]);
        storage.add(p);
    }

    fin.close();
}

Admin* FileHandler::loadAdmin() {
    ifstream fin;
    fin.open("admin.txt");

    if (!fin) {
        throw FileNotFoundException("Cannot open admin.txt");
    }

    char line[200];
    if (fin.getline(line, 200)) {
        char input[20][200];
        int inputCount = 0;
        parseLine(line, input, inputCount);

        if (inputCount >= 3) {
            int id = charToint(input[0]);
            fin.close();
            return new Admin(id, input[1], input[2]);
        }
    }

    fin.close();
    return nullptr;
}


void FileHandler::appendPatients(const Patient& p) {
    ofstream fout;
    fout.open("patients.txt", ios::app);
    
    char balStr[20];
    floatToChar(p.getBalance(), balStr);

    fout << p.getID() << ","
        << p.getName() << ","
        << p.getAge() << ","
        << p.getGender() << ","
        << p.getContact() << ","
        << p.getpwd() << ","
        << balStr << "\n";

    fout.close();
}

void FileHandler::appendDoctors(const Doctor& d) {
    ofstream fout;
    fout.open("doctors.txt", ios::app);

    char feeStr[20];
    floatToChar(d.getFee(), feeStr);

    fout << d.getID() << ","
        << d.getName() << ","
        << d.getSpec() << ","
        << d.getContact() << ","
        << d.getpwd() << ","
        << feeStr << "\n";

    fout.close();
}

void FileHandler::appendAppointments(const Appointment& a) {
    ofstream fout;
    fout.open("appointments.txt", ios::app);

    fout << a.getID() << ","
        << a.getpatientID() << ","
        << a.getdocID() << ","
        << a.getDate() << ","
        << a.gettimeSlot() << ","
        << a.getStatus() << "\n";

    fout.close();
}

void FileHandler::appendBills(const Bill& b) {
    ofstream fout;
    fout.open("bills.txt", ios::app);

    char amtStr[20];
    floatToChar(b.getamount(), amtStr);

    fout << b.getID() << ","
        << b.getPatientID() << ","
        << b.getappID() << ","
        << amtStr << ","
        << b.getStatus() << ","
        << b.getDate() << "\n";

    fout.close();
}

void FileHandler::appendPrescriptions(const Prescription& p) {
    ofstream fout;
    fout.open("prescriptions.txt", ios::app);

    fout << p.getID() << ","
        << p.getAppointmentID() << ","
        << p.getPatientID() << ","
        << p.getDoctorID() << ","
        << p.getDate() << ","
        << p.getMedicines() << ","
        << p.getNotes() << "\n";

    fout.close();
}


void FileHandler::updatePatient(const Patient& p) {
   // read all lines into temp array
    char lines[200][500];
    int lineCount = 0;

    ifstream fin;
    fin.open("patients.txt");
    while (fin.getline(lines[lineCount], 500)) {
        lineCount++;
    }
    fin.close();

    char balStr[20];
    floatToChar(p.getBalance(), balStr);

    for (int i = 0; i < lineCount; i++) {
        char input[20][200];
        int inputCount = 0;
        parseLine(lines[i], input, inputCount);

        if (charToint(input[0]) == p.getID()) {
           
            int k = 0;
            const char* id = input[0];

            char newLine[500];
            int pos = 0;

            int j = 0;
            while (id[j] != '\0') {
                newLine[pos++] = id[j++];
            }
            newLine[pos++] = ',';

            j = 0;
            const char* name = p.getName();
            while (name[j] != '\0') {
                newLine[pos++] = name[j++];
            }
            newLine[pos++] = ',';

            char ageStr[10];
            intToChar(p.getAge(), ageStr);
            j = 0;
            while (ageStr[j] != '\0') {
                newLine[pos++] = ageStr[j++];
            }
            newLine[pos++] = ',';

            newLine[pos++] = p.getGender();
            newLine[pos++] = ',';

            j = 0;
            const char* contact = p.getContact();
            while (contact[j] != '\0') {
                newLine[pos++] = contact[j++];
            }
            newLine[pos++] = ',';

            j = 0;
            const char* pwd = p.getpwd();
            while (pwd[j] != '\0') {
                newLine[pos++] = pwd[j++];
            }
            newLine[pos++] = ',';

            j = 0;
            while (balStr[j] != '\0') {
                newLine[pos++] = balStr[j++];
            }
            newLine[pos] = '\0';

            int m = 0;
            while (newLine[m] != '\0') {
                lines[i][m] = newLine[m];
                m++;
            }
            lines[i][m] = '\0';
            break;
        }
    }
    ofstream fout;
    fout.open("patients.txt");
    for (int i = 0; i < lineCount; i++) {
        fout << lines[i] << "\n";
    }
    fout.close();
}

void FileHandler::updateAppointment(const Appointment& a) {
    char lines[200][500];
    int lineCount = 0;

    ifstream fin;
    fin.open("appointments.txt");
    while (fin.getline(lines[lineCount], 500)) {
        lineCount++;
    }
    fin.close();

    for (int i = 0; i < lineCount; i++) {
        char input[20][200];
        int inputCount = 0;
        parseLine(lines[i], input, inputCount);

        if (charToint(input[0]) == a.getID()) {
           
            char newLine[500];
            int pos = 0;

            // id
            int j = 0;
            while (input[0][j] != '\0') newLine[pos++] = input[0][j++];
            newLine[pos++] = ',';

            // patient_id
            j = 0;
            while (input[1][j] != '\0') newLine[pos++] = input[1][j++];
            newLine[pos++] = ',';

            // doctor_id
            j = 0;
            while (input[2][j] != '\0') newLine[pos++] = input[2][j++];
            newLine[pos++] = ',';

            // date
            j = 0;
            const char* date = a.getDate();
            while (date[j] != '\0') newLine[pos++] = date[j++];
            newLine[pos++] = ',';

            // timeslot
            j = 0;
            const char* ts = a.gettimeSlot();
            while (ts[j] != '\0') newLine[pos++] = ts[j++];
            newLine[pos++] = ',';

            // status — this is what usually changes
            j = 0;
            const char* status = a.getStatus();
            while (status[j] != '\0') newLine[pos++] = status[j++];
            newLine[pos] = '\0';

            // replace line
            int m = 0;
            while (newLine[m] != '\0') {
                lines[i][m] = newLine[m];
                m++;
            }
            lines[i][m] = '\0';
            break;
        }
    }

    ofstream fout;
    fout.open("appointments.txt");
    for (int i = 0; i < lineCount; i++) {
        fout << lines[i] << "\n";
    }
    fout.close();
}

void FileHandler::updateBill(const Bill& b) {
    char lines[200][500];
    int lineCount = 0;

    ifstream fin;
    fin.open("bills.txt");
    while (fin.getline(lines[lineCount], 500)) {
        lineCount++;
    }
    fin.close();

    for (int i = 0; i < lineCount; i++) {
        char input[20][200];
        int inputCount = 0;
        parseLine(lines[i], input, inputCount);

        if (charToint(input[0]) == b.getID()) {
            char amtStr[20];
            floatToChar(b.getamount(), amtStr);

            char newLine[500];
            int pos = 0;

            int j = 0;
            while (input[0][j] != '\0') newLine[pos++] = input[0][j++];
            newLine[pos++] = ',';

            j = 0;
            while (input[1][j] != '\0') newLine[pos++] = input[1][j++];
            newLine[pos++] = ',';

            j = 0;
            while (input[2][j] != '\0') newLine[pos++] = input[2][j++];
            newLine[pos++] = ',';

            j = 0;
            while (amtStr[j] != '\0') newLine[pos++] = amtStr[j++];
            newLine[pos++] = ',';

            j = 0;
            const char* status = b.getStatus();
            while (status[j] != '\0') newLine[pos++] = status[j++];
            newLine[pos++] = ',';

            j = 0;
            const char* date = b.getDate();
            while (date[j] != '\0') newLine[pos++] = date[j++];
            newLine[pos] = '\0';

            int m = 0;
            while (newLine[m] != '\0') {
                lines[i][m] = newLine[m];
                m++;
            }
            lines[i][m] = '\0';
            break;
        }
    }

    ofstream fout;
    fout.open("bills.txt");
    for (int i = 0; i < lineCount; i++) {
        fout << lines[i] << "\n";
    }
    fout.close();
}

void FileHandler::updateDoctor(const Doctor& d) {
    char lines[100][500];
    int lineCount = 0;

    ifstream fin;
    fin.open("doctors.txt");
    while (fin.getline(lines[lineCount], 500)) {
        lineCount++;
    }
    fin.close();

    char feeStr[20];
    floatToChar(d.getFee(), feeStr);

    for (int i = 0; i < lineCount; i++) {
        char input[20][200];
        int inputCount = 0;
        parseLine(lines[i], input, inputCount);

        if (charToint(input[0]) == d.getID()) {
            char newLine[500];
            int pos = 0;

            int j = 0;
            while (input[0][j] != '\0') newLine[pos++] = input[0][j++];
            newLine[pos++] = ',';

            j = 0;
            const char* name = d.getName();
            while (name[j] != '\0') newLine[pos++] = name[j++];
            newLine[pos++] = ',';

            j = 0;
            const char* spec = d.getSpec();
            while (spec[j] != '\0') newLine[pos++] = spec[j++];
            newLine[pos++] = ',';

            j = 0;
            const char* contact = d.getContact();
            while (contact[j] != '\0') newLine[pos++] = contact[j++];
            newLine[pos++] = ',';

            j = 0;
            const char* pwd = d.getpwd();
            while (pwd[j] != '\0') newLine[pos++] = pwd[j++];
            newLine[pos++] = ',';

            j = 0;
            while (feeStr[j] != '\0') newLine[pos++] = feeStr[j++];
            newLine[pos] = '\0';

            int m = 0;
            while (newLine[m] != '\0') {
                lines[i][m] = newLine[m];
                m++;
            }
            lines[i][m] = '\0';
            break;
        }
    }

    ofstream fout;
    fout.open("doctors.txt");
    for (int i = 0; i < lineCount; i++) {
        fout << lines[i] << "\n";
    }
    fout.close();
}


void FileHandler::deletePatient(int id) {
    char lines[100][500];
    int lineCount = 0;

    ifstream fin;
    fin.open("patients.txt");
    while (fin.getline(lines[lineCount], 500)) {
        lineCount++;
    }
    fin.close();

    ofstream fout;
    fout.open("patients.txt");

    for (int i = 0; i < lineCount; i++) {
        char input[20][200];
        int inputCount = 0;
        parseLine(lines[i], input, inputCount);

  
        if (charToint(input[0]) != id)
        {
            fout << lines[i] << "\n";
        }
       
    }

    fout.close();
}

void FileHandler::deleteDoctor(int id) {
    char lines[100][500];
    int lineCount = 0;

    ifstream fin;
    fin.open("doctors.txt");
    while (fin.getline(lines[lineCount], 500)) {
        lineCount++;
    }
    fin.close();

    ofstream fout;
    fout.open("doctors.txt");

    for (int i = 0; i < lineCount; i++) {
        char input[20][200];
        int inputCount = 0;
        parseLine(lines[i], input, inputCount);

        if (charToint(input[0]) != id) {
            fout << lines[i] << "\n";
        }
    }

    fout.close();
}

void FileHandler::deleteAppointmentsByPatient(int patientID) {
    char lines[200][500];
    int lineCount = 0;

    ifstream fin;
    fin.open("appointments.txt");
    while (fin.getline(lines[lineCount], 500)) {
        lineCount++;
    }
    fin.close();

    ofstream fout;
    fout.open("appointments.txt");

    for (int i = 0; i < lineCount; i++) {
        char input[20][200];
        int inputCount = 0;
        parseLine(lines[i], input, inputCount);

      
        if (charToint(input[1]) != patientID) {
            fout << lines[i] << "\n";
        }
    }

    fout.close();
}

void FileHandler::deleteBillsByPatient(int patientID) {
    char lines[200][500];
    int lineCount = 0;

    ifstream fin;
    fin.open("bills.txt");
    while (fin.getline(lines[lineCount], 500)) {
        lineCount++;
    }
    fin.close();

    ofstream fout;
    fout.open("bills.txt");

    for (int i = 0; i < lineCount; i++) {
        char input[20][200];
        int inputCount = 0;
        parseLine(lines[i], input, inputCount);

        if (charToint(input[1]) != patientID) {
            fout << lines[i] << "\n";
        }
    }

    fout.close();
}

void FileHandler::deletePrescriptionsByPatient(int patientID) {
    char lines[200][500];
    int lineCount = 0;

    ifstream fin;
    fin.open("prescriptions.txt");
    while (fin.getline(lines[lineCount], 500)) {
        lineCount++;
    }
    fin.close();

    ofstream fout;
    fout.open("prescriptions.txt");

    for (int i = 0; i < lineCount; i++) {
        char input[20][200];
        int inputCount = 0;
        parseLine(lines[i], input, inputCount);

        if (charToint(input[2]) != patientID) {
            fout << lines[i] << "\n";
        }
    }

    fout.close();
}


void FileHandler::appendToDischarge(int patientID,
    Storage<Patient>& patients,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills,
    Storage<Prescription>& prescriptions) {
    ofstream fout;
    fout.open("discharged.txt", ios::app);

    // write patient record
    Patient* p = patients.findbyID(patientID);
    if (p != nullptr) {
        char balStr[20];
        floatToChar(p->getBalance(), balStr);
        fout << p->getID() << ","
            << p->getName() << ","
            << p->getAge() << ","
            << p->getGender() << ","
            << p->getContact() << ","
            << p->getpwd() << ","
            << balStr << "\n";
    }

    fout.close();
}

void FileHandler::logSecurityEvent(const char* role,
    const char* enteredID,
    const char* result) {
    ofstream fout;
    fout.open("security_log.txt", ios::app);

    time_t now = time(0);
    char timestamp[30];
    strftime(timestamp, 30, "%d-%m-%Y %H:%M:%S", localtime(&now));

    fout << timestamp << ","
        << role << ","
        << enteredID << ","
        << result << "\n";

    fout.close();
}