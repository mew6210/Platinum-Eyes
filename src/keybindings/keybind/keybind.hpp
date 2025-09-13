#pragma once
#include <string>
class KeyBind {

	int key;
	std::string description;

public:
	KeyBind(int key, std::string description):key(key),description(description) {}

	int getKey() {
		return key;
	}
	std::string getDescription() {
		return description;
	}
};
