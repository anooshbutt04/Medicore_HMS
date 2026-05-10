#include "Bill.h"

Bill::Bill() {
	billID = 0;
	patientID = 0;
	appID = 0;
	amount = 0.0;
	status[0] = '\0';
	date[0] = '\0';
}

Bill::Bill(int billID, int patientID, int appID, double amount,
	const char* status, const char* date) {

	this->billID = billID;
	this->patientID = patientID;
	this->appID = appID;
	this->amount = amount;

	int i = 0;
	while (status[i] != '\0'&& i<9) {
		this->status[i] = status[i];
		i++;
	}
	this->status[i] = '\0';

	int j = 0;
	while (date[j] != '\0' && j < 10) {
		this->date[j] = date[j];
		j++;
	}
	this->date[j] = '\0';
}

Bill::~Bill(){}

ostream& operator<<(ostream& out, const Bill& b) {
	out << "BILL ID: " << b.billID
		<< " | Patient ID: " << b.patientID
		<< " | Appointment ID: " << b.appID
		<< " | Amount in PKR: " << b.amount
		<< " | Status: " << b.status
		<< "  | Date : " << b.date;

	return out;
}


