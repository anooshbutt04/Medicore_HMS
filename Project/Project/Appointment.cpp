#include "Appointment.h"


Appointment::Appointment() {
	appID = 0;
	patientID = 0;
	docID = 0;
	date[0] = '\0';
	timeSlot[0] = '\0';
	status[0] = '\0';

}
Appointment::Appointment(int appID, int patientID, int docID, const char* date,
	const char* timeSlot, const char* status) {

	this->appID = appID;
	this->patientID = patientID;
	this->docID = docID;

	int i = 0;
	while (date[i] != '\0' && i < 10) {
		this->date[i] = date[i];
		i++;
	}
	this->date[i] = '\0';

	int j = 0;
	while (timeSlot[j] != '\0' && j < 5) {
		this->timeSlot[j] = timeSlot[j];
		j++;
	}
	this->timeSlot[j] = '\0';

	int k = 0;
	while (status[k] != '\0' && k < 9) {
		this->status[k] = status[k];
		k++;
	}
	this->status[k] = '\0';
}


bool Appointment::operator==(const Appointment& other)
{
	//if one is cancelled, no issue
	//will compare status w cancelled word if equal then no issue
	//if not toh dekhte

	bool thisCancelled = true;
	bool otherCancelled = true;

	const char* word = "cancelled";

	//comparing!

	int i = 0;
	while (word[i] != '\0') {
		if (this->status[i] != word[i]) {
			thisCancelled = false;
			break;
		}
		i++;
	}

	int j = 0;
	while (word[j] != '\0') {
		if (other.status[j] != word[j]) {
			otherCancelled = false;
			break;
		}
		j++;
	}

	if (thisCancelled || otherCancelled) {
		return false; //no clash
	}

	//checking doc, time, date

	if (this->docID != other.docID) return false;

	int d = 0;
	while (this->date[d] != '\0' && other.date[d] != '\0') {
		if (this->date[d] != other.date[d]) {
			return false;
		}
		d++;
	}
	if (this->date[d] != other.date[d]) { return false; }

	int t = 0;
	while (this->timeSlot[t] != '\0' && other.timeSlot[t] != '\0') {
		if (this->timeSlot[t] != other.timeSlot[t]) {
			return false;
		}
		t++;	
	}
	if (this->timeSlot[t] != other.timeSlot[t]) {
		return false;
		}
	//all match, none cancelled -> conflict
	return true;
}

ostream& operator<<(ostream & out, const Appointment & a){
	out << "Appointment ID: " << a.appID
		<< " | Patient ID: " << a.patientID
		<< " | Doctor ID: " << a.docID
		<< " | Date: " << a.date
		<< " | Time: " << a.timeSlot
		<< " | Status: " << a.status;
		return out;
}

