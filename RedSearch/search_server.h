#pragma once

#include "search_server.h"
#include "synchronized.h"

#include <istream>
#include <ostream>
#include <vector>
#include <string>
#include <string_view>
#include <queue>
#include <future>
#include <map>

using namespace std;

class InvertedIndex {
public:
	struct Stat {
		size_t docid;
		size_t word_count;
	};

	InvertedIndex() = default;
	explicit InvertedIndex(istream& document_input);

	const vector<Stat>& Lookup(const string& word) const;

	const deque<string>& GetDocuments() const {
		return docs;
	}

private:
	deque<string> docs;
	map<string, vector<Stat>> index;
	vector<Stat> empty;
};

class SearchServer {
public:
	SearchServer() = default;
	explicit SearchServer(istream& document_input)
		: index(InvertedIndex(document_input)) {
	}

	void UpdateDocumentBase(istream& document_input);
	void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
	Synchronized<InvertedIndex> index;
	vector<future<void>> async_tasks;
};
