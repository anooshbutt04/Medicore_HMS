
//association here
//will not own pateint or doc just use their id etc
//appointment del -> patient still exists
//== to detect conflict  & << for display 

#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <iostream>
using namespace std;

class Appointment {
private:
	int appID;
	int patientID;
	int docID;
	char date[11];
	char timeSlot[6];
	char status[10];

public:

	//default const apparently needed for the template function
	Appointment();

	Appointment(int appID, int patientID, int docID,
		const char* date, const char* timeSlot, const char* status);

	~Appointment() {}

	bool operator==(const Appointment& other);
	friend ostream& operator<<(ostream& out, const Appointment& a);

	int getID() const{ return appID; }
	int getpatientID() const { return patientID; }
	int getdocID() const { return docID; }
	const char* getDate() const { return date; }
	const char* gettimeSlot() const{ return timeSlot; }
	const char* getStatus() const { return status; }

	void setStatus(const char* stat) {
		int i = 0;
		while (stat[i] != '\0' && i < 15) {
			status[i] = stat[i];
			i++;
		}
		status[i] = '\0';
	}

	void setDate(const char* d) {
		int j = 0;
		while (d[j] != '\0') {
			date[j] = d[j];
			j++;
		}
		date[j]='\0';
	}
};

#endif
