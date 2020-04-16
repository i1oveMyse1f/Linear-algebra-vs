#pragma once

#include <iostream>
#include <initializer_list>
#include <numeric>
#include "assertm.h"
#include "mconcepts.h"

template <conc_gcd T>
T my_gcd(T x, T y) {
	while (x != T(0) && y != T(0))
		if (x > y)
			x = x % y;
		else
			y = y % x;
	if (x == T(0))
		return y;
	else
		return x;
}

template<class T = int>
requires conc_gcd<T>&& conc_read<T>&& conc_write<T>&& conc_comp<T>&& conc_base_math<T>&& requires (T x) { T(1); T(-1); }
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
			a.m = T(1);
		a.normalize();
		return in;
	}

	friend std::ostream& operator<<(std::ostream& out, const Rational& a) {
		out << a.n;
		if (a.m != T(1))
			out << '/' << a.m;
		return out;
	}
private:
	void normalize() {
		if (m < T(0))
			n = m * T(-1), m = m * T(-1);
		T g = my_gcd(n, m);
		if (g != T(0)) {
			n = n / g;
			m = m / g;
		}
	}
};
