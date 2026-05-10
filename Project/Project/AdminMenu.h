#ifndef ADMINMENU_H
#define ADMINMENU_H

#include "Admin.h"
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include "FileHandling.h"
#include "Validator.h"
#include "HospitalException.h"
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

//functions

void addDoctor(Storage<Doctor>& doctors);

void removeDoctor(Storage<Doctor>& doctors,
    Storage<Appointment>& appointments);

void viewAllPatients(Storage<Patient>& patients,
    Storage<Bill>& bills);

void viewAllDoctors(Storage<Doctor>& doctors);

void viewAllAppointments(Storage<Appointment>& appointments,
    Storage<Patient>& patients,
    Storage<Doctor>& doctors);

void viewUnpaidBills(Storage<Bill>& bills,
    Storage<Patient>& patients);

void dischargePatient(Storage<Patient>& patients,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills,
    Storage<Prescription>& prescriptions);

void viewSecurityLog();

void generateDailyReport(Storage<Appointment>& appointments,
    Storage<Bill>& bills,
    Storage<Patient>& patients,
    Storage<Doctor>& doctors);

// Helper jo banaya tha 

// sorts appointments descending by date (newest first)
void sortAppointmentsAdminDesc(Appointment* arr, int size);

#endif // !ADMINMENU_H

