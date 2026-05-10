#ifndef PERSON_H
#define PERSON_H

//Person — Abstract base class. Must have at least two pure virtual methods.Patient, Doctor and Admin inherit from it
//abstract class = cannot create object from it 
//make class abstract by putting atleast one function =0 

//common attributes taken from txt file info 


class Person {
protected:
	int id;
	char name[50];
	char pwd[50];
	//char bcs string not allowed

public:
	Person(int id, const char* name, const char* pwd);
	//chose these 2 on the basis of them being 
	//used for every child 

	virtual void displayMenu() = 0;
	virtual void displayInfo() = 0;

	int getID() {
		return id;
	}

	const char* getName() const{
		return name;
	}

	const char* getpwd()const {
		return pwd;
	}

	void setName(const char* n) {
		int i = 0; 
		while (n[i] != '\0' && i<49) {
			name[i] = n[i];
			i++;
		}
		name[i] = '\0';
	}

	bool checkPwd(const char* pass);

	virtual ~Person();
};

#endif