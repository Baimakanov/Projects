#include "date.h"

Date ParseDate(istream& datestream) {
	int year, month, day;
	datestream >> year;
	if (datestream.peek() != '-') {
		throw runtime_error("Wrong date format: ");
	}
	else {
		datestream.ignore(1);
	}

	datestream >> month;
	if (month < 1 || month > 12) {
		throw runtime_error("Month value is invalid: " + to_string(month));
	}
	if (datestream.peek() != '-') {
		throw runtime_error("Wrong date format: ");
	}
	else {
		datestream.ignore(1);
	}

	datestream >> day;
	if (day < 1 || day > 31) {
		throw runtime_error("Day value is invalid: " + to_string(day));
	}

	Date date = {year, month, day};
	return date;
}

bool operator<(const Date& lhs, const Date& rhs) {
	tuple<int, int, int> lhs_key(lhs.year, lhs.month, lhs.day);
	tuple<int, int, int> rhs_key(rhs.year, rhs.month, rhs.day);
	return lhs_key < rhs_key;
}

bool operator<=(const Date& lhs, const Date& rhs) {
	tuple<int, int, int> lhs_key(lhs.year, lhs.month, lhs.day);
	tuple<int, int, int> rhs_key(rhs.year, rhs.month, rhs.day);
	return lhs_key <= rhs_key;
}

bool operator!=(const Date& lhs, const Date& rhs) {
	tuple<int, int, int> lhs_key(lhs.year, lhs.month, lhs.day);
	tuple<int, int, int> rhs_key(rhs.year, rhs.month, rhs.day);
	return lhs_key != rhs_key;
}

bool operator==(const Date& lhs, const Date& rhs) {
	tuple<int, int, int> lhs_key(lhs.year, lhs.month, lhs.day);
	tuple<int, int, int> rhs_key(rhs.year, rhs.month, rhs.day);
	return lhs_key == rhs_key;
}

bool operator>(const Date& lhs, const Date& rhs) {
	tuple<int, int, int> lhs_key(lhs.year, lhs.month, lhs.day);
	tuple<int, int, int> rhs_key(rhs.year, rhs.month, rhs.day);
	return lhs_key > rhs_key;
}

bool operator>=(const Date& lhs, const Date& rhs) {
	tuple<int, int, int> lhs_key(lhs.year, lhs.month, lhs.day);
	tuple<int, int, int> rhs_key(rhs.year, rhs.month, rhs.day);
	return lhs_key >= rhs_key;
}


ostream& operator<<(ostream& stream, const Date& date) {
	stream << setw(4) << setfill('0') << date.year << '-'
		 << setw(2) << setfill('0') << date.month << '-'
		 << setw(2) << setfill('0') << date.day;
	return stream;
}
