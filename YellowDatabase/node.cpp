#include "node.h"

bool  EmptyNode::Evaluate(const Date& date, const string& event) const {
	return true;
}

template <typename T>
bool CompareTo(const T& lhs, const T& rhs, Comparison cmp) {
	switch (cmp) {
	case Comparison::Less:
		return lhs < rhs;
	case Comparison::LessOrEqual:
		return lhs <= rhs;
	case Comparison::Equal:
		return lhs == rhs;
	case Comparison::NotEqual:
		return lhs != rhs;
	case Comparison::Greater:
		return lhs > rhs;
	case Comparison::GreaterOrEqual:
		return lhs >= rhs;
	}
	return false; // make compiler happy
}

DateComparisonNode::DateComparisonNode(const Comparison& cmp, Date date) : cmp_(cmp), date_(date) {};

bool DateComparisonNode::Evaluate(const Date& date, const string&) const {
	return CompareTo(date, date_, cmp_);
}

EventComparisonNode::EventComparisonNode(const Comparison& cmp, string event) : cmp_(cmp), event_(event) {};

bool EventComparisonNode::Evaluate(const Date&, const string& event) const {
	return CompareTo(event, event_, cmp_);
}

LogicalOperationNode::LogicalOperationNode(
	const LogicalOperation& operation, shared_ptr<Node> left, shared_ptr<Node> right) :
		operation_(operation), left_(left), right_(right) {};

bool LogicalOperationNode::Evaluate(const Date& date, const string& event) const {
	bool left_result = left_->Evaluate(date, event);
	bool right_result = right_->Evaluate(date, event);

	if(operation_ == LogicalOperation::Or) {
		return left_result || right_result;
	} else if(operation_ == LogicalOperation::And) {
		return left_result && right_result;
	}

	return false; // make compiler happy
}
