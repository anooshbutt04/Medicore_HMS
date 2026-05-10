#include "admin.h"

Admin::Admin(int id, const char* name, const char* pwd) :Person
(id, name, pwd) {

}

Admin::~Admin(){}

void Admin::displayMenu() {
	cout << "\nAdmin Panel — MediCore" << endl;
	cout << "======================" << endl;
	cout << "1. Add Doctor" << endl;
	cout << "2. Remove Doctor" << endl;
	cout << "3. View All Patients" << endl;
	cout << "4. View All Doctors" << endl;
	cout << "5. View All Appointments" << endl;
	cout << "6. View Unpaid Bills" << endl;
	cout << "7. Discharge Patient" << endl;
	cout << "8. View Security Log" << endl;
	cout << "9. Generate Daily Report" << endl;
	cout << "10. Logout" << endl;
}

void Admin::displayInfo() {
	cout << "Admin ID: " << id << endl;
	cout << "Name: " << name << endl;
}

//too simple bcs inherits everything from person nothing new except menu & info deets
