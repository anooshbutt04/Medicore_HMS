#ifndef DOCTOR_H
#define DOCTOR_H

#include "Person.h"
#include<iostream>
using namespace std;


class Doctor : public Person {
private:
	char spec[50];
	char contact[12];
	float fee;

public:
	Doctor();

	Doctor(int id, const char* name, const char* spec, 
		const char* contact, const char* pwd, float fee);

	~Doctor();

	bool operator==(const Doctor& other);
	friend ostream& operator<<(ostream& out, const Doctor& d);

	//virtual in parent =0
	//implementing in child u write override as a good practice

	void displayMenu()override;
	void displayInfo()override;

	int getID() const { return id; }
	const char* getName() const { return name; }
	float getFee() const{ return fee; }
	const char* getSpec() const{ return spec; }
	const char* getContact() const{ return contact; }

	void setFee(float f) { fee = f; }
	void setSpec(const char* s) {
		int i = 0;
		while (s[i] != '\0') {
			spec[i] = s[i];
			i++;
		}
		spec[i] = '\0';
	}
};

#endif
