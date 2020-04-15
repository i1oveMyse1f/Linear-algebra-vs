#pragma once

#include <iostream>
#include <initializer_list>
#include <numeric>
#include "assertm.h"

template<class T = int>
class Rational {
public:
	T n, m;

	Rational(int n = 0, int m = 1) : n(n), m(m) { normalize(); }

	Rational operator+(const Rational& other) const { return { n * other.m + other.n * m, m * other.m }; }
	Rational operator-(const Rational& other) const { return { n * other.m - other.n * m, m * other.m }; }
	Rational operator*(const Rational& other) const { return { n * other.n, m * other.m }; }
	Rational operator/(const Rational& other) const { 
		assertm(other != 0, "Division by 0");
		return { n * other.m, m * other.n };
	}

	Rational& operator+=(const Rational& other) { return *this = *this + other; }
	Rational& operator-=(const Rational& other) { return *this = *this - other; }
	Rational& operator*=(const Rational& other) { return *this = *this * other; }
	Rational& operator/=(const Rational& other) { return *this = *this / other; }

	Rational operator-() const { return { -n, m }; }

	bool operator==(const Rational& other) const { return n == other.n && m == other.m; }
	bool operator!=(const Rational& other) const { return !(*this == other); }
	bool operator<(const Rational& other) const { return n * other.m < m * other.n; }
	bool operator>(const Rational& other) const { return n * other.m > m * other.n;	}
	bool operator<=(const Rational& other) const { return n * other.m <= m * other.n; }
	bool operator>=(const Rational& other) const { return n * other.m >= m* other.n; }

	friend std::istream& operator>>(std::istream& in, Rational& a) {
		in >> a.n;
		char delim = getchar();
		if (delim == '/')
			in >> a.m;
		else
			a.m = 1;
		return in;
	}

	friend std::ostream& operator<<(std::ostream& out, const Rational& a) {
		out << a.n;
		if (a.m != 1)
			out << '/' << a.m;
		return out;
	}
private:
	void normalize() {
		if (m < 0)
			n *= -1, m *= -1;
		int g = std::gcd(n, m);
		if (g) {
			n /= g;
			m /= g;
		}
	}
};