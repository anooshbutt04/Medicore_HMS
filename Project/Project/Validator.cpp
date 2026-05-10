#define _CRT_SECURE_NO_WARNINGS
#include "Validator.h"

int Validator::getLength(const char* str) {
	int len = 0;
	while (str[len] != '\0') {
		len++;
	}
	return len;
}

bool Validator::isDigit(char c) {
	return c >= '0' && c <= '9';
}

bool Validator::isAlphabet(char c) {
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z');
}

char Validator::toLower(char c) {
	if (c >= 'A' && c <= 'Z') {
		return c += 32;
	}
	return c;
}

bool Validator::compareInsensitive(const char* a, const char* b) {
	int i = 0;
	while (a[i] != '\0' && b[i] != '\0') {
		if (toLower(a[i]) != toLower(b[i])) {
			return false;
		}
		i++;
	}
	return a[i] == '\0' && b[i] == '\0';
}

bool Validator::isValidContact(const char* contact) {
	int len = getLength(contact);

	if (len != 11) return false;

	for (int i = 0; i < len; i++) {
		if (!isDigit(contact[i])) return false;
	}

	return true;
}

bool Validator::isValidPass(const char* pwd) {
	return getLength(pwd) >= 6;
}

bool Validator::isValidAmount(float amount) {
	return amount > 0;
}

bool Validator::isValidChoice(int choice, int min, int max) {
	return choice >= min && choice <= max;
}

bool Validator::isValidID(int id) {
	return id > 0;
}

bool Validator::isValidTimeSlot(const char* time) {

	const char* validSlots[8] = {
		"09:00", "10:00", "11:00", "12:00",
		"13:00", "14:00", "15:00", "16:00"
	};

	for (int i = 0; i < 8; i++) {

		int j = 0;
		bool match = true;

		while (validSlots[i][j] != '\0') {

			if (time[j] != validSlots[i][j]) {
				match = false;
				break;
			}

			j++;
		}

		if (match && time[j] == '\0') {
			return true;
		}
	}

	return false;
}

// ?? NEW: only checks YYYY-MM-DD format (length, positions, digits) ????????????
bool Validator::isValidDateFormat(const char* date) {

	if (getLength(date) != 10)
		return false;

	// dashes must be at positions 4 and 7
	if (date[4] != '-' || date[7] != '-')
		return false;

	for (int i = 0; i < 10; i++) {
		if (i == 4 || i == 7)
			continue;
		if (!isDigit(date[i]))
			return false;
	}

	return true;
}

// ?? NEW: checks that the numeric values make sense AND year >= 2026 ???????????
bool Validator::isValidDateValue(const char* date) {

	int year =
		(date[0] - '0') * 1000 +
		(date[1] - '0') * 100 +
		(date[2] - '0') * 10 +
		(date[3] - '0');

	int month =
		(date[5] - '0') * 10 +
		(date[6] - '0');

	int day =
		(date[8] - '0') * 10 +
		(date[9] - '0');

	if (day < 1 || day > 31)
		return false;

	if (month < 1 || month > 12)
		return false;

	if (year < 2026)
		return false;

	return true;
}

bool Validator::isValidDate(const char* date) {
	return isValidDateFormat(date) && isValidDateValue(date);
}

int Validator::compareDates(const char* d1, const char* d2)
{
	int i = 0;

	while (d1[i] != '\0' && d2[i] != '\0') {

		if (d1[i] > d2[i])
			return 1;

		if (d1[i] < d2[i])
			return -1;

		i++;
	}

	return 0;
}

#include <ctime>

void Validator::getTodayDate(char* dateStr)
{
	time_t now = time(0);

	strftime(dateStr,
		11,
		"%Y-%m-%d",
		localtime(&now));
}