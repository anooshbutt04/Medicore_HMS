#include "Doctor.h"

Doctor::Doctor() : Person(0, "", "") {
    spec[0] = '\0';
    contact[0] = '\0';
    fee = 0.0;
}

Doctor::Doctor(int id, const char* name, const char* spec, const char* contact,
	const char* pwd, float fee) : Person(id, name, pwd) {


	this->fee = fee;

	int i = 0;
	while (spec[i] != '\0' && i < 49) {
		this->spec[i] = spec[i];
		i++;
	}
	this->spec[i] = '\0';

	int j = 0;
	while (contact[j] != '\0' && j < 11) {
		this->contact[j] = contact[j];
		j++;
	}
	this->contact[j] = '\0';
}


	bool Doctor::operator==(const Doctor & other) {
		return this->id == other.id;
	}

	ostream& operator<<(ostream& out, const Doctor& d) {
		//menu
		out << "ID: " << d.id
			<< "| Name: " << d.name
			<< "| Specialization: " << d.spec
			<< "| Contact: " << d.contact
			<< "| Fee: " << d.fee;
		return out;
	}

	void Doctor::displayMenu() {
		cout << "\nWelcome, Dr. " << name << "| " << "\nSpecialization: " << spec << endl;
		cout << "===============================" << endl;
		cout << "1. View Today's Appointments" << endl;
		cout << "2. Mark Appointment Complete" << endl;
		cout << "3. Mark Appointment No-Show" << endl;
		cout << "4. Write Prescription" << endl;
		cout << "5. View Patient Medical History" << endl;
		cout << "6. Logout" << endl;
	}

	void Doctor::displayInfo() {
		cout << *this << endl;
	}

	Doctor::~Doctor() {}
