#pragma once
#include <string>

class Node {
public:
	Node* next;
	Node* prev;
	std::string key;
	std::string value;

	Node(const std::string& k, const std::string& v)
		: next(nullptr), prev(nullptr), key(k), value(v) {}
};
