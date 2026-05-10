#include "HospitalException.h"

HospitalException::HospitalException(const char* msg){

int i = 0;
	while (msg[i] != '\0') {
		message[i] = msg[i];
		i++;
	}
	message[i] = '\0';
}

const char* HospitalException::what() {
	return message;
}

FileNotFoundException::FileNotFoundException(const char* msg) :HospitalException(msg){}
InsufficientFundsException::InsufficientFundsException(const char*msg): HospitalException(msg){}
SlotUnavailableException::SlotUnavailableException(const char*msg):HospitalException(msg){}
InvalidInputException::InvalidInputException(const char*msg):HospitalException(msg){}

