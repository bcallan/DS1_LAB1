/* 
Title: Lab 1 - PhoneCalls.cpp

Purpose: Take a list of phone records from a file, validate
and format them into a more user friendly output.

Class: CSC 2430 Spring 2018

Author: Ben Callan
*/

// The includes.
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cctype>
#include <cstdlib>

// Instead of using namespace std.
using std::ifstream;
using std::string; using std::stoi;
using std::cout; using std::cin; using std::endl;
using std::setw; using std::left; using std::right;

//Prototypes of all of the functions.
bool IsValidDate(string& dateValue);
bool IsValidPhone(string& phoneValue);
bool IsLocalPhone(string& phoneValue);
string GetCountryCode(string& phoneValue);
string GetPhoneNumber(string& phoneValue);
int TimeDifference(string& startDateValue, string& dateValue);
void FormatDateTime(string& dateValue);
void FormatPhoneNumber(string& phoneValue);

int main() {
	// User input for file name.
	string fileName;
	// Variable to open string input from user.
	ifstream fileRead;
	// Flag for while loop.
	bool fileFail = true;
	// Variables for string values holding values from the text file.
	string dateValue; string startDateValue;
	string phoneValue;
	// A flag to determine whether the startDateValue is a valid date.
	bool timeDifFlag = true;

	// Welcoming statement and user-prompt
	cout << "Welcome to the phone call processing program." << endl;
	cout << "Please enter a file name to continue." << endl << endl;
	// While loop and if statement to Re-Prompt User input until a valid file is entered.
	while (fileFail == true) {
		// User-input
		cout << "Filename: " << endl; cin >> fileName; cout << endl;
		// Opening File
		fileRead.open(fileName);
		// Checks to make sure the file was found and opened.
		if (fileRead.fail()) {
			cout << "Unable to open file '" << fileName << "'" << endl;
			cout << "Please try again..." << endl;
		}
		// Sets fileFail to false in the case that the file opened successfully.
		// This will exit the while loop and continue the program.
		else {
			fileFail = false;
		}
	}
	// This will output the header for the formatted output.
	cout << setw(24) << left << "Start" << setw(24) << "End" << setw(8) << 
		"Country" << setw(15) << "Phone" << setw(7) << "Local" << setw(10) << "Duration(s)" << endl;
	// This will out put the border set under the formatted header.
	for (int i = 0; i < 89; i++) {
		cout << "-";
	}
	// A new line before everything starts.
	cout << endl;
	// While loop that contiunes until it reaches the end of the file.
	while (!fileRead.eof()) {
		// Variable for duration to be manipulated whether the dates are valid.
		int timeDif = 0;
		
		//Loading first string of characters of the line.
		fileRead >> dateValue;

		// Validates that the date is of correct format and values.
		// Outputs if the date is valid
		if (IsValidDate(dateValue) == false) {
			cout << setw(24) << left << ".Invalid Start Time.";
			timeDifFlag = false;
		}
		// Outputs the formatted start date if the date is valid.
		else {
			FormatDateTime(dateValue);
			cout << "    ";
		}

		// Saves the start date into another value so not to be overwritten.
		startDateValue = dateValue;

		// Reads in the next set of characters, the end date.
		fileRead >> dateValue;

		// Validates that the date is of correct format and values.
		// Outputs if the date is invalid.
		if (IsValidDate(dateValue) == false) {
			cout << setw(24) << left << ".Invalid End Time.";
		}
		// Outputs the formatted end date if the date is valid.
		else {
			FormatDateTime(dateValue);
			cout << "    ";
		}
		// Reads in the next set of characters, the phone number.	
		fileRead >> phoneValue;
		// Validates that the phone number is correct format and has the correct values.
		// Outputs if the phone number is invalid.
		if (IsValidPhone(phoneValue) == false) {
			cout << setw(23) << ".Invalid Phone Number.";
				cout << setw(3) << "I" << setw(4);
		}
		else {
			// Outputs the country code followed by a formatted phone number.
			cout << right << setw(7) << GetCountryCode(phoneValue);
			FormatPhoneNumber(phoneValue);

			// Outputs whether the phone is a local "+1" number or not.
			if (IsLocalPhone(phoneValue) == true) {
				cout << setw(4) << "Y" << setw(7);
			}
			else if (IsLocalPhone(phoneValue) == false) {
				cout << setw(4) << "N";
			}
		}
		// Sets duration to -1 if either the start or end dates are invalid.
		if (IsValidDate(dateValue) == false || timeDifFlag == false) {
			timeDif = -1;
		}
		// Sets duration to the time difference calculated in thh function TimeDifference.
		else {
			timeDif = TimeDifference(startDateValue, dateValue);
		}
		// Outputs duration
		cout << setw(14) << timeDif;
		// Starts a new line.
		cout << endl;
		// Rests the flag for the next loop.
		timeDifFlag = true;
	}
	// Closes the file
	fileRead.close();

	return 0;
}

// Validates that the string of dateValue is of correct format.
// Params: string input from file in yyyymmddhhmmss format (unless invalid)
// Returns: True or False depending on if dateValue is of correct format
// Format Error: returns false if of incorrect length, contains anything but digits,
// and that each individual portion of the string is in the correct contraints.
bool IsValidDate(string& dateValue) {
	

	// Checking if the length is 14 characters.
	if (dateValue.length() != 14) {
		return false;
	}

	// For loop to check if the the date is all digits.
	for (unsigned int i = 0; i < dateValue.length(); i++) {
		if (!isdigit(dateValue[i])) {
			return false;
		}
	}

	// Breaking down the individual date values into substrings.
	int dateYear = stoi(dateValue.substr(0, 4));
	int dateMonth = stoi(dateValue.substr(4, 2));
	int dateDay = stoi(dateValue.substr(6, 2));
	int dateHour = stoi(dateValue.substr(8, 2));
	int dateMin = stoi(dateValue.substr(10, 2));
	int dateSec = stoi(dateValue.substr(12, 2));

	
	
	// Checking if the date values are within the correct constraints.
	if (dateYear > 2050 || dateYear < 1980 || dateMonth > 12 || dateMonth < 0
		|| dateHour > 24 || dateHour < 0 || dateMin > 60 || dateMin < 0
		|| dateSec > 60 || dateSec < 0) {
		return false;
	}
	// Checking if the day's value is within the correct contraints of the months value.
	// Checking 31 day months.
	if (dateMonth == 1 || dateMonth == 3 || dateMonth == 5 || dateMonth == 7
		|| dateMonth == 8 || dateMonth == 10 || dateMonth == 12) {
		if (dateDay > 31 || dateDay < 0) {
			return false;
		}
	}
	// Checking 30 day months.
	else if (dateMonth == 4 || dateMonth == 6 || dateMonth == 9 || dateMonth == 11) {
		if (dateDay > 30 || dateDay < 0) {
			return false;
		}
	}
	// Checking February on the Leap Year.
	else if (dateMonth == 2 && dateYear % 4 == 0) {
		if (dateDay > 29 || dateDay < 0) {
			return false;
		}
	}
	// Checking February not on the Leap Year.
	else if (dateMonth == 2 && dateYear % 4 != 0) {
		if (dateDay > 28 || dateDay < 0) {
			return false;
		}
	}
	return true;
}

// Validates that the string of phoneValue is of correct format.
// Params: string input from file in a 12 number format (unless invalid)
// Returns: True or False depending on if phoneValue is of correct format
// Format Error: returns false if of incorrect length, contains anything but digits,
// and if there is a "+" at the begining.
bool IsValidPhone(string& phoneValue) {

	// Checks that the phone number is of correct length.
	if (phoneValue.length() != 12) {
		return false;
	}
	string validPhone = GetPhoneNumber(phoneValue);

	// Checks that there are only digits.
	for (unsigned int i = 0; i < validPhone.length(); i++){
		if (!isdigit(validPhone[i])) {
			return false;
		}
	}
	// Checks for a '+' at the begining of the string.
	if (phoneValue.substr(0, 1) != "+") {
		return false;
	}
	return true;
}

// Checks if the phone number is a US phone (or local)
// Params: 12 number string
// Returns: True or False depending on if a +1 is found.
// Format Error: there should be no errors, the string must have 
// been previously validated.
bool IsLocalPhone(string& phoneValue) {
	// Checks if the number is a local number.
	if (phoneValue.substr(0, 2) == "+1") {
		return true;
	}
	return false;
}

// Seeks and returns the country code at the begining of each phone number.
// Params: string of exactly 12 characters
// Returns: string value corresponding with what country code is being used.
// Format Error: there should be no errors, the string must have been previously
// validated
string GetCountryCode(string& phoneValue) {
	// string used to return country code.
	string getCode;

	// If statement to find what country code is being used.
	if (phoneValue.find("+1") == 0) {
		getCode = "1";
	}
	else if (phoneValue.find("+502") == 0) {
		getCode = "502";
	}
	else if (phoneValue.find("+503") == 0) {
		getCode = "503";
	}
	else if (phoneValue.find("+504") == 0) {
		getCode = "504";
	}
	else if (phoneValue.find("+505") == 0) {
		getCode = "505";
	}
	else if (phoneValue.find("+506") == 0) {
		getCode = "506";
	}
	else if (phoneValue.find("+866") == 0) {
		getCode = "866";
	}

	// Outputs error if code is faulty. (Just a precaution).
	else {
		return "ERROR";
	}
	return getCode;
}

// Returns the phone number without the country code.
// Params: string of exactly 12 characters
// Returns: string of 10 or 8 characters based on the country code present.
// Format Error: there should be no errors, the string must have been previously
// validated
string GetPhoneNumber(string& phoneValue) {
	// string used to return new phone number
	string phoneNumber;

	// Checking for the local extension to return a 10 character string.
	if (phoneValue.find("+1") == 0) {
		phoneNumber = phoneValue.substr(2, 10);
	}
	// All other phone formats are the same, this returns an 8 character string.
	else {
		phoneNumber = phoneValue.substr(4, 8);
	}

	return phoneNumber;
}

// This function returns the duration of the phone call.
// Params: 2 strings of exactly 14 characters the format yyyymmddhhmmss. The start and end date values.
// Returns: an integer value of duration of a phone call.
// Format Error: there should be no errors, the string must have been previously
// validated
int TimeDifference(string& startDateValue, string& dateValue) {
	 
	// Variables for calculation.
	int timeDif; int startTime; int endTime;
	int startHour; int startMin; int startSec;
	int endHour; int endMin; int endSec;
	int startDay; int endDay;
	
	// Part of the Extra Credit. Finding the day values.
	startDay = stoi(startDateValue.substr(6, 2));
	endDay = stoi(dateValue.substr(6, 2));

	// Start time individual values from string to integer.
	startHour = stoi(startDateValue.substr(8, 2));
	startMin = stoi(startDateValue.substr(10, 2));
	startSec = stoi(startDateValue.substr(12, 2));

	// Start time in seconds format.
	startTime = (((startHour * 60) + startMin) * 60) + startSec;
	
	// End time individual values from string to integer.
	endHour = stoi(dateValue.substr(8, 2));
	endMin = stoi(dateValue.substr(10, 2));
	endSec = stoi(dateValue.substr(12, 2));

	// End time in seconds format.
	endTime = (((endHour * 60) + endMin) * 60) + endSec;

	// Second part of extra credit.
	// If the days do not equal each other, 
	// a days worth of seconds is added to the
	// end time to account for calls going over night.
	if (startDay != endDay) {
		endTime = endTime + 86400;
	}

	// The final calculation of the duration.
	timeDif = endTime - startTime;

	return timeDif;
}

// Produce user friendly version of call start and end time
// Params: string of exactly 14 characters in format yyyymmddhhmmss
// Returns: new string formatted Jan 0 hh:mm:ss
// Format Error: there should be no errors, the string must have been previously
// validated.
void FormatDateTime(string& dateValue) {
	
	//Substrings of each variable inside the date and time.
	int dateYear = stoi(dateValue.substr(0, 4));
	string dateMonth = dateValue.substr(4, 2);
	int dateDay = stoi(dateValue.substr(6, 2));
	string dateHour = dateValue.substr(8, 2);
	string dateMin = dateValue.substr(10, 2);
	string dateSec = dateValue.substr(12, 2);

	// Adds a 0 in front of the a number if there is only one character.
	if (dateHour.length() == 1) {
		dateHour = "0" + dateHour;
	}
	if (dateMin.length() == 1) {
		dateMin = "0" + dateMin;
	}
	if (dateSec.length() == 1) {
		dateSec = "0" + dateSec;
	}
	// The amazing integer to string conversion for the Month.
	if (dateMonth == "01") {
		dateMonth = "Jan";
	}
	else if (dateMonth == "02") {
		dateMonth = "Feb";
	}
	else if (dateMonth == "03") {
		dateMonth = "Mar";
	}
	else if (dateMonth == "04") {
		dateMonth = "Apr";
	}
	else if (dateMonth == "05") {
		dateMonth = "May";
	}
	else if (dateMonth == "06") {
		dateMonth = "Jun";
	}
	else if (dateMonth == "07") {
		dateMonth = "Jul";
	}
	else if (dateMonth == "08") {
		dateMonth = "Aug";
	}
	else if (dateMonth == "09") {
		dateMonth = "Sep";
	}
	else if (dateMonth == "10") {
		dateMonth = "Oct";
	}
	else if (dateMonth == "11") {
		dateMonth = "Nov";
	}
	else if (dateMonth == "12") {
		dateMonth = "Dec";
	}

	// The actual output of the formatted date.
	cout << dateMonth << " " << setw(2) << dateDay << " " << dateHour << ":"
		<< dateMin << ":" << dateSec << " " << dateYear;
}


// Produce user friendly version of the phone number
// Params: string of 12 characters
// Returns: new string formatted in either (000)-000-0000 or 0000-0000
// Format Error: there should be no errors, the string must have been previously
// validated
void FormatPhoneNumber(string& phoneValue) {
	string formattedNumber;
	string cCode = GetCountryCode(phoneValue);
	string phoneNumber = GetPhoneNumber(phoneValue);

		if (cCode == "1") {
			formattedNumber = "(" + phoneNumber.substr(0, 3) + ")-" + phoneNumber.substr(3, 3) + "-" + phoneNumber.substr(6, 4);																		// formatting for US number
		}
		// All other phone numbers are formatted the same.
		else {
			formattedNumber = phoneNumber.substr(0, 4) + "-" + phoneNumber.substr(4, 8);
		}

		cout << setw(15) << formattedNumber;
}