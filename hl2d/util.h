#pragma once

#include <string>
#include <map>

using std::string;
using std::map;

namespace util {
	string SHA1(string input);
	string base64Encode(string input);

	void startWinsock();
	void cleanWinsock();

	string createJson(map<string, string> pairs);
}
