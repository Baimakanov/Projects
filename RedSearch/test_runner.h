#pragma once

#include <sstream>
#include <stdexcept>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator << (ostream& tr_os, const vector<T>& s) {
	tr_os << "{";
	bool first = true;
	for (const auto& x : s) {
		if (!first) {
			tr_os << ", ";
		}
		first = false;
		tr_os << x;
	}
	return tr_os << "}";
}

template <class T>
ostream& operator << (ostream& tr_os, const set<T>& s) {
	tr_os << "{";
	bool first = true;
	for (const auto& x : s) {
		if (!first) {
			tr_os << ", ";
		}
		first = false;
		tr_os << x;
	}
	return tr_os << "}";
}

template <class K, class V>
ostream& operator << (ostream& tr_os, const map<K, V>& m) {
	tr_os << "{";
	bool first = true;
	for (const auto& kv : m) {
		if (!first) {
			tr_os << ", ";
		}
		first = false;
		tr_os << kv.first << ": " << kv.second;
	}
	return tr_os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
	if (!(t == u)) {
		ostringstream tr_os;
		tr_os << "Assertion failed: " << t << " != " << u;
		if (!hint.empty()) {
			tr_os << " hint: " << hint;
		}
		throw runtime_error(tr_os.str());
	}
}

inline void Assert(bool b, const string& hint) {
	AssertEqual(b, true, hint);
}

class TestRunner {
public:
	template <class TestFunc>
	void RunTest(TestFunc func, const string& test_name) {
		try {
			func();
			cerr << test_name << " OK" << endl;
		} catch (exception& e) {
			++fail_count;
			cerr << test_name << " fail: " << e.what() << endl;
		} catch (...) {
			++fail_count;
			cerr << "Unknown exception caught" << endl;
		}
	}

	~TestRunner() {
		if (fail_count > 0) {
			cerr << fail_count << " unit tests failed. Terminate" << endl;
			exit(1);
		}
	}

private:
	int fail_count = 0;
};

#define ASSERT_EQUAL(x, y) {				\
	ostringstream tr_os;					\
	tr_os << #x << " != " << #y << ", "		\
		<< __FILE__ << ":" << __LINE__;		\
    AssertEqual(x, y, tr_os.str());			\
}

#define ASSERT(x) {							\
	ostringstream tr_os;					\
	tr_os << #x << " is false, "			\
		<< __FILE__ << ":" << __LINE__;		\
	Assert(x, tr_os.str());                 \
}

#define RUN_TEST(tr, func) \
	tr.RunTest(func, #func)
