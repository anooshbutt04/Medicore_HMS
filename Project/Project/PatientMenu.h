#ifndef PATIENTMENU_H
#define PATIENTMENU_H

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
using namespace std;

void bookAppointment(Patient& p,
    Storage<Doctor>& doctors,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills);

void cancelAppointment(Patient& p,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills,
    Storage<Doctor>& doctors);

void viewAppointments(Patient& p,
    Storage<Appointment>& appointments,
    Storage<Doctor>& doctors);

void viewMedicalRecords(Patient& p,
    Storage<Prescription>& prescriptions,
    Storage<Doctor>& doctors);

void viewBills(Patient& p,
    Storage<Bill>& bills);

void payBill(Patient& p,
    Storage<Bill>& bills);

void topUpBalance(Patient& p);

void runPatientMenu(Patient& p,
    Storage<Doctor>& doctors,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills,
    Storage<Prescription>& prescriptions);

// helpers
void sortAppointmentsAsc(Appointment* arr, int size);
void sortAppointmentsDesc(Appointment* arr, int size);

#endif