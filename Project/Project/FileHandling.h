#ifndef FILEHANDLER_H
#define FILEHANDLER_H


#include "Storage.h"
#include "Admin.h"
#include "Appointment.h"
#include "Prescription.h"
#include "Bill.h"
#include "Patient.h"
#include "Doctor.h"


#include<fstream>

using namespace std;

class FileHandler {
public:

    static void loadPatients(Storage<Patient>& storage);
    static void loadDoctors(Storage<Doctor>& storage);
    static void loadAppointments(Storage<Appointment>& storage);
    static void loadBills(Storage<Bill>& storage);
    static void loadPrescriptions(Storage<Prescription>& storage);
    static Admin* loadAdmin();


    static void appendPatients(const Patient& p);
    static void appendDoctors(const Doctor& d);
    static void appendAppointments(const Appointment& a);
    static void appendBills(const Bill& b);
    static void appendPrescriptions(const Prescription& p);


    static void updatePatient(const Patient& p);
    static void updateDoctor(const Doctor& d);
    static void updateAppointment(const Appointment& a);
    static void updateBill(const Bill& b);

    static void deletePatient(int id);
    static void deleteDoctor(int id);
    static void deleteAppointmentsByPatient(int patientID);
    static void deleteBillsByPatient(int patientID);
    static void deletePrescriptionsByPatient(int patientID);



    //project says: "Copy the patient's full
    //record into discharged.txt then delete from all four files"

    static void appendToDischarge(int patientID,
        Storage<Patient>& patients,
        Storage<Appointment>& appointments,
        Storage<Bill>& bills,
        Storage<Prescription>& prescriptions);

    //"After 3 failed login attempts → log to security_log.txt"

    static void logSecurityEvent(const char* role,
        const char* enteredID,
        const char* result);

    //private helper fuctions:

    //charToInt for takinf age etc from char to int from txt to cpp

private:

    static void parseLine(const char* line,
        char input[][200], int& inputCount);

    static int charToint(const char* str);
    static float charToFloat(const char* str);
    static void intToChar(int num, char* str);
    static void floatToChar(float num, char* str);
};

#endif
