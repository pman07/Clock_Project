
//////////////////////////////////////
//  Project 1 - Clock				//
//									//
//  Date: 1/21/2022					//
//  Author: Michael Pierson			//
//////////////////////////////////////

#include <iostream>
#include<iomanip>
#include <thread>
#include <chrono>
#include <conio.h>
using namespace std;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

int hour = 0;    // initialize global clock variables
int minute = 0;
int second = 0;

void incHour() {   // increment hour, rollover at midnight
	if (hour < 23) {
		hour += 1;
	}
	else {
		hour = 0;
	}
}

void incMinute() {   // increment minute, rollover at 60 minutes and increment hour
	if (minute < 59) {
		minute += 1;
	}
	else {
		incHour();
		minute = 0;
	}
}

void incSecond() {   // increment second, rollover at 60 seconds and increment minute
	if (second < 59) {
		second += 1;
	}
	else {
		incMinute();
		second = 0;
	}
}

void printTime12Hr() {   // print time in 12 hour format
	string suffix;
	int hour12;

	hour12 = hour;  // get current hour in 24 hour format
	if (hour12 < 12) {   // correct for 12 hour format and decide A.M. or P.M.
		if (hour12 == 0) {
			hour12 = 12;
		}
		suffix = "AM";
	}
	else {
		if (hour12 > 12) {
			hour12 = hour - 12;
		}
		suffix = "PM";
	}
	cout << setw(2) << setfill('0') << hour12 << ":";   // print time in 12 hour format
	cout << setw(2) << setfill('0') << minute << ":";
	cout << setw(2) << setfill('0') << second << " " << suffix;
}

void printTime24Hr() {   // print time in 24 hour format
	cout << setw(2) << setfill('0') << hour << ":";
	cout << setw(2) << setfill('0') << minute << ":";
	cout << setw(2) << setfill('0') << second;
}

void printClocks() {   // print formatted 12 and 24 hour clocks

	cout << "    *************************    *************************" << endl;
	cout << "    *     12-Hour Clock     *    *     24-Hour Clock     *" << endl;
	cout << "    *      "; 
	printTime12Hr(); 
	cout << "      *    *       "; 
	printTime24Hr(); 
	cout << "        *" << endl;
	cout << "    *************************    *************************" << endl;

}

void printMenu() {   // print formatted menu options

	cout << endl;
	cout << "    *************************" << endl;
	cout << "    * 1 - Add One Hour      *" << endl;
	cout << "    * 2 - Add One Minute    *" << endl;
	cout << "    * 3 - Add One Second    *" << endl;
	cout << "    * 4 - Exit Program      *" << endl;
	cout << "    * 5 - Toggle Menu       *" << endl;
	cout << "    *************************" << endl;
	cout << endl;

}

void getInput(int& userOption, bool& showMenu) {

	userOption = int(_getch() - '0');  // convert character to numerical values 0 - 9.
	cout << "   You entered option: " << userOption << " ";   // mirror user entered value

	if (userOption == 1) {   // increment hour and print confirmation
		incHour();
		cout << "- Add One Hour. \n   Action completed.\n\n";
	}
	else if (userOption == 2) {   // increment minute and print confirmation
		incMinute();
		cout << "- Add One Minute. \n   Action completed.\n\n";
	}
	else if (userOption == 3) {   // increment second and print confirmation
		incSecond();
		cout << "- Add One Second. \n   Action completed.\n\n";
	}
	else if (userOption == 4) {   // exit program
		cout << "- Exit Program. \n   Goodbye!\n\n";
	}
	else if (userOption == 5) {
		showMenu = !showMenu;
		cout << "- Toggle Menu.  \n   Action completed.\n\n";
	}
	else {   // otherwise option is invalid, print error message
		cout << "- Invalid option.\n\n";
	}

	if (userOption != 4) {
		userOption = 0;   // reset userOption unless exit program selected
	}
	

}


void waitOneSecond(int& userOption, long timeDelay) {
	sleep_for(chrono::milliseconds(timeDelay));   // wait ~1 second, adjusted for previous scan time(s)
}

auto now() {
	return std::chrono::high_resolution_clock::now();   // return time point value for current time 
}

void updateDelayTime(double& timeDelay, std::chrono::high_resolution_clock::time_point& start) {
	double timeDiscrepancy = 0;
	
	std::chrono::duration<double, std::milli> elapsed{ now() - start };  // get duration of scan since last loop
	start = now();   // get time point for start of next loop
	timeDiscrepancy = 1000 - elapsed.count(); // calculate difference between actual scan time and 1 second
	timeDelay += (timeDiscrepancy); // Adjust time delay based on last scans discrepancy

}

int main() {
	int userOption = 0;   // initialize userOption to enter while loop
	double timeDelay = 1000;  // start time delay at 1 second
	bool showMenu = true;
	
	std::chrono::high_resolution_clock::time_point start = now();   // get time point for start of first loop

	while (userOption != 4) {   // exit loop when user enters option 4

		do {
			printClocks();   // print clocks
			if (showMenu) {
				printMenu();     // print menu options
			}
			waitOneSecond(userOption, timeDelay);   // wait ~1 second
			updateDelayTime(timeDelay, start);   // adjust delay time based on scantime actual vs target
			incSecond();    // increment time by 1 second

		} while (!_kbhit());   // keep looping until keyboard key is pressed

		getInput(userOption, showMenu);  // when key is pressed, get character as userOption
	}

	return 0;
}