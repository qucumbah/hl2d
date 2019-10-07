#pragma once

#include <cmath>
#include <iostream>

using std::ostream;
using std::cout;
using std::endl;

class Vec2 {
public:
	const double x, y;

	static double getIntersectionCoefficient(Vec2 p, Vec2 m, Vec2 w, Vec2 b) {
		double upper = (w - p) ^ b;
		double lower = (m ^ b);

		if (lower == 0) {
			return NAN;
		}

		return upper / lower;
	}

	static bool isFacing(Vec2 p, Vec2 m, Vec2 w, Vec2 b) {
		//Returns true even if wall is behind player (see skecth 3)
		bool rightSide = (m ^ (w - p)) <= 0;
		bool leftSide = (m ^ (w + b - p)) >= 0;

		//cout << (p + m) << (w - p) << (w + b - p) << endl;

		//cout << "right: " << rightSide << endl;
		//cout << "left: " << leftSide << endl;

		return rightSide && leftSide;
	}

	Vec2(double _x, double _y) : x(_x), y(_y) {

	}

	double length() {
		return sqrt(x*x + y*y);
	}

	Vec2 normal() {
		if (this->length() == 0) {
			return Vec2(0, 0);
		}
		return *this / this->length();
	}

	Vec2 perpendicularClockwise() {
		return Vec2(y, -x);
	}
	Vec2 perpendicularCounterClockwise() {
		return Vec2(-y, x);
	}

	//Cross product
	friend double operator^(Vec2 a, Vec2 b) {
		return a.x * b.y - a.y * b.x;
	}

	friend Vec2 operator+(Vec2 a, Vec2 b) {
		return Vec2(a.x + b.x, a.y + b.y);
	}
	friend Vec2 operator-(Vec2 a, Vec2 b) {
		return Vec2(a.x - b.x, a.y - b.y);
	}

	friend double operator*(Vec2 a, Vec2 b) {
		return a.x * b.x + a.y * b.y;
	}

	friend Vec2 operator*(Vec2 a, double num) {
		return Vec2(a.x * num, a.y * num);
	}
	friend Vec2 operator*(double num, Vec2 a) {
		return Vec2(a.x * num, a.y * num);
	}
	friend Vec2 operator/(Vec2 a, double num) {
		return Vec2(a.x / num, a.y / num);
	}

	friend bool operator>(Vec2 a, Vec2 b) {
		return a.length() > b.length();
	}
	friend bool operator<(Vec2 a, Vec2 b) {
		return a.length() < b.length();
	}

	friend ostream& operator<<(ostream& out, Vec2 vector) {
		out << "{" << vector.x << ":" << vector.y << "}";
		
		return out;
	}
};