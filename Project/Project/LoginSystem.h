#ifndef LOGINSYSTEM_H
#define LOGINSYSTEM_H

#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Storage.h"
#include "FileHandling.h"
#include "Validator.h"
#include <iostream>
using namespace std;

//  1 = Patient logged in, 2 = Doctor logged in, 3 = Admin logged in,   0 = failed / locked
int showLogin(
    Storage<Patient>& patients,
    Storage<Doctor>& doctors,
    Admin* admin,
    Patient*& outPatient,
    Doctor*& outDoctor);

void showWelcome();

#endif
