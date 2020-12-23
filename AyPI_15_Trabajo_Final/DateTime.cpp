#include <string>
#include <iostream>
#include <ctime>
#include <conio.h>
#include "DateTime.h"

using UDateTime::DateTime;
using namespace std;

struct UDateTime::DateTime {
	int year;
	int month;
	int day;
	int hour;
	int minutes;
	int seconds;
};

//Funciones Auxiliares
string AddZero(int numero);
bool IsBeforeMidday(int hour);
bool IsValidData(int data, int start, int end);

bool IsValidData(int data, int start, int end) {
	bool isValidData;
	return isValidData = (data >= start && data <= end) ? true : false;
}

string AddZero(int numero) {
	string addZero;
	return addZero = numero < 10 ? to_string(0) : "";
}

bool IsBeforeMidday(int hour) {
	bool isBeforemidday;
	return isBeforemidday = (hour < 12) ? true : false;
}

int AddHour(int hour, int auxHour) {
	int addHour = (IsBeforeMidday(hour)) ? hour : auxHour - 12;
	return addHour;
}

UDateTime::DateTime* UDateTime::Now() {
	DateTime* dateTime = new DateTime;
	struct tm newtime;
	time_t now = time(NULL);
	localtime_s(&newtime, &now);
	dateTime->year = 1900 + newtime.tm_year;
	dateTime->month = 1 + newtime.tm_mon;
	dateTime->day = newtime.tm_mday;
	dateTime->hour = newtime.tm_hour;
	dateTime->minutes = newtime.tm_min;
	dateTime->seconds = newtime.tm_sec;
	return dateTime;
}

UDateTime::DateTime* UDateTime::CreateDateTime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, int minutes, int seconds) {
	DateTime* dateTime = NULL;
	bool isValidDate = true;
	while (isValidDate) {
		dateTime = new DateTime;
		dateTime->year = year;
		isValidDate = IsValidData(month, 1, 12);
		dateTime->month = month;
		isValidDate = IsValidData(day, 1, 31);
		dateTime->day = day;
		isValidDate = IsValidData(hour, 0, 23);
		dateTime->hour = hour;
		isValidDate = IsValidData(minutes, 0, 59);
		dateTime->minutes = minutes;
		isValidDate = IsValidData(seconds, 0, 59);
		dateTime->seconds = seconds;
	}
	return (isValidDate) ? dateTime : NULL;
}

unsigned int UDateTime::GetYear(const DateTime* dateTime) {
	return dateTime->year;
}

unsigned int UDateTime::GetMonth(const DateTime* dateTime) {
	return dateTime->month;
}

unsigned int UDateTime::GetDay(const DateTime* dateTime) {
	return dateTime->day;
}

unsigned int UDateTime::GetHour(const DateTime* dateTime) {
	return dateTime->hour;
}

unsigned int UDateTime::GetMinutes(const DateTime* dateTime) {
	return dateTime->minutes;
}

unsigned int UDateTime::GetSeconds(const DateTime* dateTime) {
	return dateTime->seconds;
}

string UDateTime::ToFormat(const UDateTime::DateTime* dateTime, UDateTime::DateTimeFormat format) {
	string formattedDate;
	int auxHour = dateTime->hour;
	string auxYear = to_string(dateTime->year);
	auxYear = auxYear.substr(auxYear.length() - 2, 3);
	switch (format) {
	case YYYYMMDD_HHmmss://YYYYMMDD_HHmmss: 2019-04-27 19:01:10
		formattedDate = to_string(dateTime->year) + "-";
		formattedDate += AddZero(dateTime->month) + to_string(dateTime->month) + "-";
		formattedDate += AddZero(dateTime->day) + to_string(dateTime->day) + " ";
		formattedDate += AddZero(dateTime->hour) + to_string(dateTime->hour) + ":";
		formattedDate += AddZero(dateTime->minutes) + to_string(dateTime->minutes) + ":";
		formattedDate += AddZero(dateTime->seconds) + to_string(dateTime->seconds);
		break;
	case YYYYMMDD_hhmmss://YYYYMMDD_hhmmss: 2019 - 04 - 27 07 : 01 : 10 p.m.
		formattedDate = to_string(dateTime->year) + " - ";
		formattedDate += AddZero(dateTime->month) + to_string(dateTime->month) + " - ";
		formattedDate += AddZero(dateTime->day) + to_string(dateTime->day) + " ";
		formattedDate += AddZero(AddHour(dateTime->hour, auxHour)) + to_string(AddHour(dateTime->hour, auxHour));
		formattedDate += AddZero(dateTime->minutes) + to_string(dateTime->minutes) + ":";
		formattedDate += AddZero(dateTime->seconds) + to_string(dateTime->seconds) + " ";
		formattedDate += (IsBeforeMidday(dateTime->hour)) ? "a.m." : "p.m.";
		break;
	case DDMMYYYY_hhmmss://DDMMYYYY_hhmmss: 27-04-2019 07:01:10 p.m.
		formattedDate = AddZero(dateTime->day) + to_string(dateTime->day) + "-";
		formattedDate += AddZero(dateTime->month) + to_string(dateTime->month) + "-";
		formattedDate = to_string(dateTime->year) + " ";
		formattedDate += AddZero(AddHour(dateTime->hour, auxHour)) + to_string(AddHour(dateTime->hour, auxHour));
		formattedDate += AddZero(dateTime->minutes) + to_string(dateTime->minutes) + ":";
		formattedDate += AddZero(dateTime->seconds) + to_string(dateTime->seconds) + " ";
		formattedDate += (IsBeforeMidday(dateTime->hour)) ? "a.m." : "p.m.";
		break;
	case DDMMYYYY_HHmmss://DDMMYYYY_HHmmss: 27-04-2019 19:01:10
		formattedDate = AddZero(dateTime->day) + to_string(dateTime->day) + "-";
		formattedDate += AddZero(dateTime->month) + to_string(dateTime->month) + "-";
		formattedDate = to_string(dateTime->year) + " ";
		formattedDate += AddZero(dateTime->hour) + to_string(dateTime->hour) + ":";
		formattedDate += AddZero(dateTime->minutes) + to_string(dateTime->minutes) + ":";
		formattedDate += AddZero(dateTime->seconds) + to_string(dateTime->seconds) + " ";
		break;
	case YYYYMMDDHHmmss://YYYYMMDDHHmmss: 20190427191410
		formattedDate = to_string(dateTime->year);
		formattedDate += AddZero(dateTime->month) + to_string(dateTime->month);
		formattedDate += AddZero(dateTime->day) + to_string(dateTime->day);
		formattedDate += AddZero(dateTime->hour) + to_string(dateTime->hour);
		formattedDate += AddZero(dateTime->minutes) + to_string(dateTime->minutes);
		formattedDate += AddZero(dateTime->seconds) + to_string(dateTime->seconds);
		break;
	case YYMD_Hms://YYMD_Hms: 19-4-27 19:1:10
		formattedDate = auxYear + "-" + to_string(dateTime->month) + "-";
		formattedDate += to_string(dateTime->day) + " ";
		formattedDate += to_string(dateTime->hour) + ":";
		formattedDate += to_string(dateTime->minutes) + ":";
		formattedDate += to_string(dateTime->seconds);
		break;
	case YYMD_hms://YYMD_hms: 19-4-27 7:1:10 p.m.
		formattedDate = auxYear + "-" + to_string(dateTime->month) + "-";
		formattedDate += to_string(dateTime->day) + " ";
		formattedDate += to_string(AddHour(dateTime->hour, auxHour));
		formattedDate += ":" + to_string(dateTime->minutes) + ":";
		formattedDate += ":" + to_string(dateTime->seconds);
		formattedDate += (IsBeforeMidday(dateTime->hour)) ? "a.m." : "p.m.";
		break;
	case DMYY_hms://DMYY_hms: 27-4-19 7:1:10 p.m.
		formattedDate += to_string(dateTime->day) + "-";
		formattedDate += to_string(dateTime->month) + "-";
		formattedDate += auxYear + " ";
		formattedDate += to_string(AddHour(dateTime->hour, auxHour));
		formattedDate += ":" + to_string(dateTime->minutes) + ":";
		formattedDate += ":" + to_string(dateTime->seconds);
		formattedDate += (IsBeforeMidday(dateTime->hour)) ? "a.m." : "p.m.";
		break;
	case DMYY_Hms://DMYY_Hms: 27-4-19 19:1:10
		formattedDate += to_string(dateTime->day) + "-";
		formattedDate += to_string(dateTime->month) + "-";
		formattedDate += auxYear + " ";
		formattedDate += to_string(dateTime->hour) + ":";
		formattedDate += to_string(dateTime->minutes) + ":";
		formattedDate += to_string(dateTime->seconds) + ":";
		break;
	}
	return formattedDate;
}

void UDateTime::DestroyDateTime(DateTime* dateTime) {
	delete dateTime;
}