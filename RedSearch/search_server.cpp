#include "search_server.h"

#include "parse.h"
#include "iterator_range.h"

#include <algorithm>
#include <future>
#include <numeric>
#include <iterator>

vector<string> SplitIntoWords(const string& line) {
	istringstream words_input(line);
	return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

// добавляет строку в вектор документов, добавляет слово в словарь индексов
InvertedIndex::InvertedIndex(istream& document_input) {
	for (string current_document; getline(document_input, current_document); ) {
		docs.push_back(move(current_document));
		size_t docid = docs.size() - 1;
		for (const string& word : SplitIntoWords(docs.back())) {
			auto& docids = index[word];
			if (!docids.empty() && docids.back().docid == docid) {
				++docids.back().word_count;
			} else {
				docids.push_back({docid, 1});
			}
		}
	}
}

// ищет слово в словаре индексов, возвращает вектор из порядковых номеров строк-документов
const vector<InvertedIndex::Stat>& InvertedIndex::Lookup(const string& word) const {
	if (auto it = index.find(word); it != index.end()) {
		return it->second;
	} else {
		return empty;
	}
}

void UpdateIndex(istream& document_input, Synchronized<InvertedIndex>& index) {
	InvertedIndex new_index(document_input);
	swap(index.GetAccess().ref_to_value, new_index);
}

// добавление документа в базу
void SearchServer::UpdateDocumentBase(istream& document_input) {
	async_tasks.push_back(async(UpdateIndex, ref(document_input), ref(index)));
}

void MultiAddQueriesStream(
istream& query_input,
ostream& search_results_output,
Synchronized<InvertedIndex>& async_index) {
	//индекс массива - номер документа, элемент массива - количество слова
	vector<size_t> docid_count;
	for (string current_query; getline(query_input, current_query); ) {
		const auto words = SplitIntoWords(current_query);
		{
			// доступ осуществляется под мьютексом
			auto access_index = async_index.GetAccess();
			docid_count.assign(access_index.ref_to_value.GetDocuments().size(), 0);
			auto& index = access_index.ref_to_value;
			for (const auto& word : words) {
				for (const auto& [docid, word_count] : index.Lookup(word)) {
					docid_count[docid] += word_count;
				}
			}

		}

		vector<pair<size_t, size_t>> search_results;
		for (auto it = begin(docid_count); it != end(docid_count); ++it) {
			if (*it > 0) {
				search_results.push_back({it - begin(docid_count), *it});
			}
		}
		partial_sort(
			begin(search_results),
			Head(search_results, 5).end(),
			end(search_results),
			[](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
				int64_t lhs_docid = lhs.first;
				auto lhs_hit_count = lhs.second;
				int64_t rhs_docid = rhs.first;
				auto rhs_hit_count = rhs.second;
				return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
			}
		);

		search_results_output << current_query << ':';
		for (auto [docid, hitcount] : Head(search_results, 5)) {
			search_results_output << " {"
					<< "docid: " << docid << ", "
					<< "hitcount: " << hitcount << '}';
		}
		search_results_output << endl;
	}
}

// отдает результаты поиска
void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
	async_tasks.push_back(
		async(MultiAddQueriesStream, ref(query_input), ref(search_results_output), ref(index))
	);
}
