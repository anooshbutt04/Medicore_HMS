#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <iostream>
#include<ctime>
using namespace std;

class Validator {

public:

	//validator not storing anything toh static 

	static bool isValidDate(const char* date);

	static bool isValidDateFormat(const char* date);

	static bool isValidDateValue(const char* date);

	static bool isValidTimeSlot(const char* time);

	static bool isValidContact(const char* contact);

	static bool isValidPass(const char* pwd);

	static bool isValidAmount(float amount);

	static bool isValidChoice(int choice, int min, int max);

	static bool isValidID(int id);

	static int compareDates(const char* d1, const char* d2);

	static void getTodayDate(char* dateStr);
	//some helping 

	static int getLength(const char* str);

	static char toLower(char c);

	static bool isDigit(char c);

	static bool isAlphabet(char c);

	static bool compareInsensitive(const char* a, const char* b);
};






#endif // !VALIDATOR_H
