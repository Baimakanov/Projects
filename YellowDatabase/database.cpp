#include "database.h"
#include <unistd.h>

ostream& operator<<(ostream& output, pair<Date , string> date_event) {
	output << date_event.first << " " << date_event.second;
	return output;
}

ostream& operator<<(ostream& output, pair<Date, vector<string>> date_events) {
	for(auto event: date_events.second) {
		output << date_events.first << " " << event << endl;
	}
	return output;
}

void Database::Add(const Date& new_date, const string& new_event) {

	if (set_storage[new_date].count(new_event) == 0) {
		vector_storage[new_date].push_back(new_event);
		set_storage[new_date].insert(new_event);
	}
}

ostream& Database::Print(ostream& out) const {
	for (const auto& item : vector_storage) {
		for (const string& event : item.second) {
			out << item.first << " " << event << endl;
		}
	}

	return out;
}

pair<Date, string> Database::Last(const Date& date) const {
	if (vector_storage.size() == 0 || date < vector_storage.begin()->first) {
		throw invalid_argument("No entries");
	}
	auto it = vector_storage.upper_bound(date);
	--it;

	return make_pair(it->first, *it->second.rbegin());
}

vector<pair<Date, string>> Database::FindIf(std::function<bool (const Date&, const string&)> predicate) const {
	vector<pair<Date, string>> result_events;

	for(auto date : vector_storage) {
		for(auto event : date.second) {
			if(predicate(date.first, event)) {
				result_events.push_back(make_pair(date.first, event));
			}
		}
	}
	return result_events;
}

int Database :: RemoveIf(std::function<bool (const Date&, const string&)> predicate) {
	int count = 0;
	map<Date, vector<string>> vector_storage_copy = vector_storage;

	for(auto& date : vector_storage_copy) {
		vector<string> events = date.second;
		auto it = remove_if(begin(events), end(events), [date, predicate](const string& s){
			return predicate(date.first, s);
		});
		count += end(events) - it;
		events.erase(it, end(events));

		set_storage[date.first] = set<string>(begin(events), end(events));
		if(events.size() == 0) {
			vector_storage.erase(date.first);
		} else {
			vector_storage[date.first] = events;
		}
	}

	return count;
}
