 #ifndef HOSPITALEXCEPTION_H
 #define HOSPITALEXCEPTION_H
 
//parent class for all the errors
//other child exceptions will be derived from this

class HospitalException {
protected: 
	char message[200];

public:
	//parameterised constructor
	HospitalException(const char* msg);

	virtual const char* what();
		//w/o what you'd hace no way to GET the msg 
		//msg is protected so what is acting like a GETTER here

};

//child errors this one will be used in all^ 

class FileNotFoundException : public HospitalException {
public: 
	FileNotFoundException(const char* msg);
};

class InsufficientFundsException : public HospitalException {
public:
	InsufficientFundsException(const char* msg);
	//mil bcs msg hoga nai paas woh parent mai hai toh run kya hoga

};

class InvalidInputException : public HospitalException {
public:
	InvalidInputException(const char* msg);
};

class SlotUnavailableException : public HospitalException {
public:
	SlotUnavailableException(const char* msg);
};

#endif
