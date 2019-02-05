/*
Программа, которая реализует работу с простой базой данных.
Программа работает через стандартный ввод и вывод (потоки stdin и stdout).
Данные хранятся в БД в виде пары: дата, событие.
Дата определена как строка: Год-Месяц-День, где Год, Месяц и День — целые числа
- добавление события:                        Add Дата Событие
- удаление события:                          Del Дата Событие
- удаление всех событий за конкретную дату:  Del Дата
- поиск событий за конкретную дату:          Find Дата
- печать всех событий за все даты:           Print
Пример:
	Add 0-1-2 event1
	Add 1-2-3 event2
	Find 0-1-2

	Del 0-1-2
	Print
	Del 1-2-3 event2
	Del 1-2-3 event2
*/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <set>
#include <sstream>
#include <exception>

using namespace std;

class Date {
public:

	Date (int new_year, int new_month, int new_day) {
		year = new_year;
		month = new_month;
		day = new_day;
	}

	int GetYear() const {
		return year;
	}
	int GetMonth() const {
		return month;
	}
	int GetDay() const {
		return day;
	}

private:
	int year;
	int month;
	int day;
};

// сравнение для класса дата необходимо для использования их в качестве ключей словаря
bool operator<(const Date& lhs, const Date& rhs){
	if (lhs.GetYear() == rhs.GetYear()) {
		if (lhs.GetMonth() == rhs.GetMonth()) {
			return lhs.GetDay() < rhs.GetDay();
		}
		return lhs.GetMonth() < rhs.GetMonth();
	}
	return lhs.GetYear() < rhs.GetYear();
}

class Database {
public:
	void AddEvent(const Date& date, const string& new_event) {
		events[date].insert(new_event);
	}


	bool DeleteEvent(const Date& date, const string& event) {
		bool key = 0;
		if (events.count(date) > 0 && events[date].count(event) > 0) {
		      events[date].erase(event);
		      key = 1;
		}
		return key;
	}

	int  DeleteDate(const Date& date) {
		int eventcount = 0;
		if (events.count(date) > 0) {
			eventcount = events[date].size();
			events.erase(events.find(date));
		}
		return eventcount;
	}

	void Find(const Date& date) {
		if (events.count(date) > 0) {
			for (const string& event : events[date]) {
				cout << event << endl;
			}
		}
	}

	void Print() const {
		for (const auto& item : events) {
			for (const string& event : item.second) {
		    	cout << setw(4) << setfill('0') << item.first.GetYear() << '-'
		    		 << setw(2) << setfill('0') << item.first.GetMonth() << '-'
					 << setw(2) << setfill('0') << item.first.GetDay() << " "
					 << event << endl;
			}
		}
	}

private:
	map <Date, set<string>> events;
};

//проверка формата даты Год-Месяц-День
Date MakeDate(const string& datestring) {
	int year, month, day;
	stringstream temp(datestring);

	temp >> year;
	if (temp.peek() != '-') {
		throw runtime_error("Wrong date format: " + datestring);
	}
	else {
		temp.ignore(1);
	}

	temp >> month;
	if (month < 1 || month > 12) {
		throw runtime_error("Month value is invalid: " + to_string(month));
	}
	if (temp.peek() != '-') {
		throw runtime_error("Wrong date format: " + datestring);
	}
	else {
		temp.ignore(1);
	}

	temp >> day;
	if (day < 1 || day > 31) {
		throw runtime_error("Day value is invalid: " + to_string(day));
	}

	if (!temp.eof()) {
		throw runtime_error("Wrong date format: " + datestring);
	}

	Date date = {year, month, day};
	return date;
}

int main() {
	Database db;

	string command;
	while (getline(cin, command)) {
		try {
			stringstream temp;
			temp << command;
			string operation;
			temp >> operation;

			if (operation == "Add") {

				string datestring;
				temp >> datestring;
				Date date = MakeDate(datestring);
				if (temp.peek() == ' ') {
					string event;
					temp >> event;
					db.AddEvent(date, event);
				} else {
					throw runtime_error("Wrong date format: " + datestring);
					continue;
				}

			} else if (operation == "Del") {

				string datestring;
				temp >> datestring;
				Date date = MakeDate(datestring);
				string event;
				if (temp >> event) {
					bool key;
					key = db.DeleteEvent(date, event);
					if (key == 1) {
						cout << "Deleted successfully" << endl;
					} else {
						cout << "Event not found" << endl;
					}
					continue;
				} else {
					int count;
					count = db.DeleteDate(date);
					cout << "Deleted " << count << " events" << endl;
					continue;
				}

			} else if (operation == "Find") {

				string datestring;
				temp >> datestring;
				Date date = MakeDate(datestring);
				db.Find(date);
				continue;

			} else if (operation == "Print") {
				db.Print();
				continue;

			} else if (command.empty()) {
				continue;

			} else {
				cout << "Unknown command: " << operation << endl;
				continue;
			}
		} catch (exception& ex) {
			cout << ex.what() << endl;
		}
	}

	return 0;
}
