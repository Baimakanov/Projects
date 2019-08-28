#pragma once

#include "date.h"
#include <memory>

using namespace std;

enum class Comparison {
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
	Equal,
	NotEqual,
};

enum class LogicalOperation {
	Or,
	And,
};


class Node {
public:
	virtual bool Evaluate(const Date& new_date, const string& new_event) const = 0;
};


class EmptyNode : public Node {
	bool Evaluate(const Date& date , const string& event) const override;
};

class DateComparisonNode : public Node {
public:
	DateComparisonNode(const Comparison& cmp, Date date);
	bool Evaluate(const Date& date, const string& event) const override;

private:
	Comparison cmp_;
	Date date_;
};

class EventComparisonNode : public Node {
public:
	EventComparisonNode(const Comparison& cmp, string event);
	bool Evaluate(const Date& date, const string& event) const override;

private:
	Comparison cmp_;
	string event_;
};

class LogicalOperationNode : public Node {
public:
	LogicalOperationNode(const LogicalOperation& logical_operation, shared_ptr<Node> left, shared_ptr<Node> right);
	bool Evaluate(const Date& date, const string& event) const override;

private:
	shared_ptr<Node> left_;
	shared_ptr<Node> right_;
	LogicalOperation operation_;
};
