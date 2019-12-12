# YellowDatabase
Программа, которая реализует работу поисковой системы. Программа может одновременно обрабатывать несколько потоков поисковых запросов, а также обновления базы документов.

## Использование
Класс SearchServer, позволяет выполнять поиск в базе документов.
Конструктор класса SearchServer принимает поток ввода, содержащий базу документов. При этом
- один документ — это одна строка входного потока
- документы состоят из слов, разделённых одним или несколькими пробелами
- слова состоят из строчных латинских букв.
Например, код, приведённый ниже, загружает в объект класса SearchServer базу из трёх документов:

	string docs;
	docs += "london is the capital of great britain\n";
	    // документ содержит 7 слов
	docs += "i am travelling down the river\n";
	    // документ содержит 6 слов
	docs += "  here     we    go             ";
	    // документ содержит 3 слова

	istringstream document_input(docs);
	SearchServer srv(document_input);

Метод AddQueriesStream выполняет собственно поиск. Он принимает входной поток поисковых запросов и выходной поток для записи результатов поиска. При этом
- один запрос — это одна строка в потоке query_input
- каждый поисковый запрос состоит из слов, разделённых одним или несколькими пробелами
- так же, как и в документах, слова в запросах состоят из строчных латинских букв

Результатом обработки поискового запроса является набор из максимум пяти наиболее релевантных документов. В качестве метрики релевантности используется суммарное количество вхождений всех слов запроса в документ. Например, допустим, имеется поисковая база из трёх документов: "london is the capital of great britain", "moscow is the capital of the russian federation", "paris is the capital of france", — и поисковый запрос "the best capital". Тогда метрика релевантности документов будет такой:

	london is the capital of great britain — 2 (слово "the" входит в документ 1 раз, слово "best" — ни разу, слово "capital" — 1 раз)
	moscow is the capital of the russian federation — 3 (слово "the" входит в документ 2 раза, слово "best" — ни разу, слово "capital" — 1 раз)
	paris is the capital of france — 2 ("the" — 1, "best" — 0, "capital" — 1)

В итоге получается, что документ "moscow is the capital of the russian federation" оказывается наиболее релевантным запросу "the best capital".
Для каждого поискового запроса метод AddQueriesStream выводит в поток search_results_output одну строку в формате [текст запроса]: {docid: <значение>, hitcount: <значение>} {docid: <значение>, hitcount: <значение>} ..., где docid — идентификатор документа, а hitcount — значение метрики релевантности для данного документа (то есть суммарное количество вхождений всех слов запроса в данный документ). При этом
- в результаты поиска не добавляются  документы, hitcount которых равен нулю
- при подсчёте hitcount учитываются только слова целиком, то есть слово «there» не является вхождением слова «the».

Метод UpdateDocumentBase заменяет текущую базу документов на новую, которая содержится в потоке document_input. При этом документ из первой строки этого потока будет иметь идентификатор (docid) 0, документ из второй строки — идентификатор 1 и т.д. Точно так же конструктор класса SearchServer назначает идентификаторы документам. Например, код

	const string doc1 = "london is the capital of great britain";
	const string doc2 = "moscow is the capital of the russian federation";
	istringstream doc_input1(doc1 + '\n' + doc2);
	SearchServer srv(doc_input1);

	const string query = "the capital";
	istringstream query_input1(query);
	srv.AddQueriesStream(query_input1, cout);

	istringstream doc_input2(doc2 + '\n' + doc1);
	srv.UpdateDocumentBase(doc_input2);
	istringstream query_input2(query);
	srv.AddQueriesStream(query_input2, cout);

выводит

	the capital: {docid: 1, hitcount: 3} {docid: 0, hitcount: 2}
	the capital: {docid: 0, hitcount: 3} {docid: 1, hitcount: 2}

