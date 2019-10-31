#pragma once

#include <string>
#include <map>
#include <list>

using std::string;
using std::map;
using std::list;

namespace util {
	string SHA1(string input);
	string base64Encode(string input);

	void startWinsock();
	void cleanWinsock();

	string createJson(map<string, string> pairs);
	string createJsonArray(list<string> elements);

	template<class T>
	T lerp(T a, T b, double t) {
		return a + (b - a) * t;
	}
}
