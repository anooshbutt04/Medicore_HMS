#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include<iostream>
using namespace std;

class Prescription {
private:
	int presID;
	int patientID;
	int appID;
	int docID;
	char date[11];
	char medicines[500];
	char notes[300];

public:

	Prescription();
	Prescription(int presID, int patientID, int appID,
		int docID, const char* date, const char* medicines, const char* notes);

	~Prescription();
	
	int getID()const
	{
		return presID;
	}
	int getPatientID()const
	{ 
		return patientID; 
	}
	int getAppointmentID()const
	{ 
		return appID;
	}
	int getDoctorID()const
	{ 
		return docID;
	}
	const char* getDate()const
	{
		return date; 
	}
	const char* getMedicines() const
	{ 
		return medicines; 
	}
	const char* getNotes()const
	{
		return notes;
	}

	void setMedicines(const char* m);
	void setNotes(const char* n);

	friend ostream& operator<<(ostream& out, const Prescription& p);
};

#endif // !PRESCRIPTION_H
