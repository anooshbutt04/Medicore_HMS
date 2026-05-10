#include "Patient.h"

//defining class

Patient::Patient() : Person(0, "", "") {
	age = 0;
	gender = ' ';
	num[0] = '\0';
	balance = 0.0;
}

Patient::Patient(int id, const char* name, const char* pwd, int age,
	char gender, const char* number, float balance) : Person(id, name, pwd) {

	this->age = age;
	this->gender = gender;
	this->balance = balance;

	int i = 0;
	while (number[i] != '\0' && i < 11) {
		this->num[i] = number[i];
		i++;
	}
	this->num[i] = '\0';
}

Patient::~Patient(){}


Patient& Patient::operator+=(float amount) {
	balance += amount;
	return*this;
}

Patient& Patient::operator-=(float amount) {
	balance -= amount;
	return*this;
}

bool Patient::operator==(const Patient& other) {
	return this->id == other.id;
}

ostream& operator<<(ostream& out, const Patient& p) 
{
	out << "ID: " << p.id
		<< "| Name: " << p.name
		<< "| Age: " << p.age
		<< "| Gender: " << p.gender
		<< "| Contact: " << p.num
		<< "| Balance PKR: " << p.balance;
		return out;
}

void Patient::displayMenu() {
	cout << "Welcome, " << name << endl;
	cout << "Balance: PKR " << balance << endl;
	cout << "1. Book Appointment" << endl;
	cout << "2. Cancel Appointment" << endl;
	cout << "3. View My Appointments" << endl;
	cout << "4. View My Medical Records" << endl;
	cout << "5. View My Bills" << endl;
	cout << "6. Pay Bill" << endl;
	cout << "7. Top Up Balance" << endl;
	cout << "8. Logout" << endl;
}

void Patient::displayInfo() {
	cout << *this << endl;
}