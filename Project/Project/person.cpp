#include "Person.h"


Person::Person(int id, const char* name, const char* pwd) {
	this->id = id;

	int i = 0;
	while (name[i] != '\0' && i < 49) {
		this->name[i] = name[i];
		i++;
	}
	this->name[i] = '\0';

	int j = 0;
	while (pwd[j] != '\0' && j < 49) {
		this->pwd[j] = pwd[j];
		j++;
	}
	this->pwd[j] = 0;

}


bool Person::checkPwd(const char* pass) {
	int i = 0;
	while (pass[i] != '\0' && pwd[i] != '\0') {
		if (pass[i] != pwd[i]) {
			return false;
		}
		i++;
	}
	return pass[i] == '\0' && pwd[i] == '\0';
}

Person::~Person() {}