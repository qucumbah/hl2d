#include "Util.h"

#include <sstream>
#include <map>
#include <iostream>

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using std::string;
using std::stringstream;
using std::map;

char* stringToBits(string input, int length) {
	const char* data = input.data();

	char* result = new char[length] {0};
	for (unsigned int i = 0; i < input.length(); i++) {
		result[i * 8 + 0] = (data[i] >> 7) & 1;
		result[i * 8 + 1] = (data[i] >> 6) & 1;
		result[i * 8 + 2] = (data[i] >> 5) & 1;
		result[i * 8 + 3] = (data[i] >> 4) & 1;
		result[i * 8 + 4] = (data[i] >> 3) & 1;
		result[i * 8 + 5] = (data[i] >> 2) & 1;
		result[i * 8 + 6] = (data[i] >> 1) & 1;
		result[i * 8 + 7] = (data[i] >> 0) & 1;
	}

	return result;
}

char* int64ToBits(uint64_t input) {
	char* result = new char[64] {0};
	for (int i = 0; i < 64; i++) {
		result[63-i] = (input >> i) & 1;
	}
	return result;
}

char* int32ToBits(uint32_t input) {
	char* result = new char[32] {0};
	for (int i = 0; i < 32; i++) {
		result[31 - i] = (input >> i) & 1;
	}
	return result;
}

uint32_t getBlockFromBits(char* input, int start) {
	uint32_t result = 0;
	for (int i = 0; i < 32; i++) {
		result += input[start + 31 - i] << i;
	}
	return result;
}

uint32_t leftrotate(uint32_t input, unsigned int count) {
	return input << count | input >> (32 - count);
}

char* bitsToString(char* bits, int length) {
	char* result = new char[length / 8 + 1] {0};
	for (int i = 0; i < length / 8; i++) {
		for (int offset = 0; offset < 8; offset++) {
			result[i] += bits[i*8 + 7 - offset] << offset;
		}
	}
	return result;
}

char bitsToCharacter(char* bits) {
	char result = 0;
	for (int offset = 0; offset < 8; offset++) {
		result += bits[7 - offset] << offset;
	}
	return result;
}

string util::SHA1(string input) {
	uint32_t h0 = 0x67452301;
	uint32_t h1 = 0xEFCDAB89;
	uint32_t h2 = 0x98BADCFE;
	uint32_t h3 = 0x10325476;
	uint32_t h4 = 0xC3D2E1F0;

	const int LENGTH = input.length() * 8; //Bit message length
	int newLength;
	if (LENGTH % 512 < 448) {
		newLength = (LENGTH / 512 + 1) * 512;
	}
	else {
		newLength = (LENGTH / 512 + 2) * 512;
	}

	char* bits = stringToBits(input, newLength);

	bits[LENGTH] = 1;

	char* finalBits = int64ToBits(LENGTH);
	for (int i = 0; i < 64; i++) {
		bits[i + newLength - 64] = finalBits[i];
	}
	delete[] finalBits;

	for (int start = 0; start < newLength; start += 512) {
		uint32_t w[80]; //Blocks
		for (int i = 0; i < 16; i++) {
			w[i] = getBlockFromBits(bits, start + i * 32);
			/*
			for (int j = 0; j < 32; j++) {
				cout << (int)bits[start + start + i * 32 + j];
			}
			cout << "|" << w[i] << endl;
			*/
		}

		for (int i = 16; i < 80; i++) {
			w[i] = (w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16]);
			w[i] = leftrotate(w[i], 1);
		}

		uint32_t a = h0;
		uint32_t b = h1;
		uint32_t c = h2;
		uint32_t d = h3;
		uint32_t e = h4;

		for (int i = 0; i < 80; i++) {
			uint32_t f;
			uint32_t k;
			if (i < 20) {
				f = (b & c) | (~b & d);
				k = 0x5A827999;
			}
			else if (i < 40) {
				f = b ^ c ^ d;
				k = 0x6ED9EBA1;
			}
			else if (i < 60) {
				f = (b & c) | (b & d) | (c & d);
				k = 0x8F1BBCDC;
			}
			else if (i < 80) {
				f = b ^ c ^ d;
				k = 0xCA62C1D6;
			}

			uint32_t temp = leftrotate(a, 5) + f + e + k + w[i];
			e = d;
			d = c;
			c = leftrotate(b, 30);
			b = a;
			a = temp;
		}

		h0 += a;
		h1 += b;
		h2 += c;
		h3 += d;
		h4 += e;
	}

	delete[] bits;

	char* resultBits = new char[160];

	char* h0bits = int32ToBits(h0);
	char* h1bits = int32ToBits(h1);
	char* h2bits = int32ToBits(h2);
	char* h3bits = int32ToBits(h3);
	char* h4bits = int32ToBits(h4);

	for (int i = 0; i < 32; i++) {
		resultBits[i + 0] = h0bits[i];
	}
	for (int i = 0; i < 32; i++) {
		resultBits[i + 32] = h1bits[i];
	}
	for (int i = 0; i < 32; i++) {
		resultBits[i + 64] = h2bits[i];
	}
	for (int i = 0; i < 32; i++) {
		resultBits[i + 96] = h3bits[i];
	}
	for (int i = 0; i < 32; i++) {
		resultBits[i + 128] = h4bits[i];
	}

	delete[] h0bits;
	delete[] h1bits;
	delete[] h2bits;
	delete[] h3bits;
	delete[] h4bits;

	/*
	for (int i = 0; i < 160; i++) {
		cout << (int)resultBits[i];
		if (i % 8 == 7) {
			//cout << " ";
		}
	}
	cout << endl;
	*/

	char* result = bitsToString(resultBits, 160);
	string resultString(result);
	delete[] resultBits;
	delete[] result;
	return resultString;
	/*
	//Same as
	char* result = new char[21] {0};
	for (int i = 0; i < 20; i++) {
		result[i] = bitsToCharacter( &resultBits[i*8] );
	}
	*/
}

const char* base64Chars =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

string util::base64Encode(const string input) {
	string result;

	int val = 0, valb = -6;
	for (unsigned char c : input) {
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0) {
			result.push_back(base64Chars[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}

	if (valb > -6) {
		result.push_back( base64Chars[((val << 8) >> (valb + 8)) & 0x3F] );
	}

	while (result.size() % 4 != 0) {
		result.push_back('=');
	}

	return result;
}

void util::startWinsock() {
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cerr << "WSAStartup failed: " << result << "\n";
		return;
	}
}

void util::cleanWinsock() {
	WSACleanup();
}

string util::createJson(map<string, string> pairs) {
	stringstream result;
	result << "{";

	int elementNumber = 1;
	const int size = pairs.size();
	for (auto pair : pairs) {
		result << '"' << pair.first << "\":\"" << pair.second << '"';

		bool last = elementNumber == size;
		if (!last) {
			result << ",";
		}
		elementNumber++;
	}

	result << "}";

	return result.str();
}

string util::createJsonArray(list<string> elements) {
	stringstream result;
	result << "[";

	int elementNumber = 1;
	const int size = elements.size();
	for (auto element : elements) {
		result << element;

		bool last = elementNumber == size;
		if (!last) {
			result << ",";
		}
		elementNumber++;
	}

	result << "]";

	return result.str();
}
