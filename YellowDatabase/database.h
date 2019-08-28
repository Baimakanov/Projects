#pragma once
#include "date.h"
#include <map>
#include <deque>
#include <utility>
#include <functional>
#include <algorithm>
#include <set>

using namespace std;

ostream& operator<<(ostream& outstream, const pair<Date, string> date_event);

ostream& operator<<(ostream& output, pair<Date, vector<string>> date_events);

class Database {
public:
	void Add(const Date& date, const string& event);
	ostream& Print(ostream& date) const;
	pair<Date, string> Last(const Date& date) const;
	vector<pair<Date, string>> FindIf(std::function<bool (const Date&, const string&)> predicate) const;
	int RemoveIf(std::function<bool (const Date&, const string&)> predicate);

private:
	map<Date, vector<string>> vector_storage;
	map<Date, set<string>> set_storage;
};
