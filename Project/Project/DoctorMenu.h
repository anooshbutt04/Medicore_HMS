
#ifndef DOCTORMENU_H
#define DOCTORMENU_H

#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Prescription.h"
#include "Storage.h"
#include "FileHandling.h"
#include "Validator.h"
#include "HospitalException.h"
#include <iostream>
using namespace std;


void viewTodaysAppointments(Doctor& d,
    Storage<Appointment>& appointments,
    Storage<Patient>& patients);

void markComplete(Doctor& d,
    Storage<Appointment>& appointments);

void markNoShow(Doctor& d,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills);

void writePrescription(Doctor& d,
    Storage<Appointment>& appointments,
    Storage<Prescription>& prescriptions);

void viewPatientHistory(Doctor& d,
    Storage<Patient>& patients,
    Storage<Appointment>& appointments,
    Storage<Prescription>& prescriptions);

// helper — sorts appointments by timeslot ascending
void sortByTimeSlot(Appointment* arr, int size);

// helper — compares two HH:MM time strings
int compareTimeSlots(const char* a, const char* b);

#endif


