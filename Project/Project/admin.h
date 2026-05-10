#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"
#include<iostream>
using namespace std;

class Admin : public Person {
public:

	Admin(int id, const char* name, const char* pwd);
	
	~Admin();

	void displayMenu() override;
	void displayInfo() override;

};

#endif
