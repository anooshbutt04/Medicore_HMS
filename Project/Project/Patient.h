#ifndef PATIENT_H
#define PATIENT_H

#include "Person.h"
#include <iostream>
using namespace std;

//overload << -= == 
//needs patiend id name age gender cintact pwd balance


class Patient : public Person {
private:
	int age;
	char gender;
	char num[12];
	float balance;

public:

	Patient();

	Patient(int id, const char* name, const char* pwd, int age, char gender,
		const char* num, float balance);

	Patient& operator+=(float amount);

	//deduction when bill paid
	Patient& operator-=(float amount);

	bool operator==(const Patient& other);

	friend ostream& operator<<(ostream& out, const Patient& p);

	void displayMenu() override; //override write kyu
	void displayInfo() override;

	int getID() const{ return id; }
	int getAge() const{ return age; }
	const char* getpwd() const { return pwd; }
	char getGender() const { return gender; }
	const char* getContact()const { return num; }
	float getBalance()const { return balance; }
	void setBalance(float b) { balance = b; }
	void setAge(int a) { age = a; }
	void setContact(const char* c) {
		int i = 0;
		while (c[i] != '\0') {
			num[i] = c[i];
			i++;
		}
		num[i] = '\0';
	}

	~Patient();
};

#endif