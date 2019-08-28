#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <tuple>

using namespace std;

struct Date {
	int year;
	int month;
	int day;
};

Date ParseDate(istream& date);

bool operator<(const Date& lhs, const Date& rhs);
bool operator<=(const Date& lhs, const Date& rhs);
bool operator!=(const Date& lhs, const Date& rhs);
bool operator==(const Date& lhs, const Date& rhs);
bool operator>(const Date& lhs, const Date& rhs);
bool operator>=(const Date& lhs, const Date& rhs);

ostream& operator<<(ostream& stream, const Date& date);
