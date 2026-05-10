#ifndef BILL_H
#define BILL_H

#include<iostream>
using namespace std;

class Bill {
private:
	int billID;
	int patientID;
	int appID;
	double amount;
	char status[10];
	char date[11];

public:

	Bill();
	Bill(int billID, int patientID, int appID, double amount,
		const char* status, const char* date);
	~Bill();

	int getID() const
	{
		return billID;
	}
	int getPatientID()const
	{
		return patientID;
	}
	int getappID()  const
	{
		return appID;
	}
	double getamount() const
	{
		return amount;
	}
	const char* getStatus()const
	{
		return status;
	}
	const char* getDate()const
	{
		return date;
	}

	void setstatus(const char* newstat) {
		int i = 0;
		while (newstat[i] != '\0' && i<9) {
			status[i] = newstat[i];
			i++;
		}
		status[i] = '\0';
	}

	void setAmount(int a) {
		amount = a;
	}

	friend ostream& operator<<(ostream& out, const Bill& b);

};
#endif 
