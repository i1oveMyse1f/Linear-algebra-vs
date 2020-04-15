#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <initializer_list>
#include "assertm.h"

template <typename T>
class Polynomial {
private:
	std::vector<T> poly;
	void normalize() {
		while (poly.size() > 1 && poly.back() == T(0))
			poly.pop_back();
	}

public:
	Polynomial(const std::vector<T>& a): poly(a) { normalize(); }
	template <typename N>
	Polynomial(N beg, N end): poly(beg, end) { normalize();	}
	Polynomial(const T& a = 0): poly({a}) {}
	Polynomial(const Polynomial& p): poly(p) {}
	Polynomial(std::initializer_list<T> l) : poly(l) { normalize(); }

	size_t size() const { return poly.size(); };
	int Degree() const {
		if (poly.size() == 1 && poly[0] == T(0))
			return -1;
		return poly.size() - 1;
	}

	bool operator==(const Polynomial& other) const {
		size_t n = poly.size();
		size_t m = other.poly.size();
		if (n != m)
			return false;
		for (size_t i = 0; i < n; ++i)
			if (poly[i] != other.poly[i])
				return false;
		return true;
	}
	bool operator!=(const Polynomial& other) const { return !(*this == other); }

	Polynomial operator+(const Polynomial& other) const {
		std::vector<T> p(std::max(size(), other.size()));
		for (size_t i = 0; i < poly.size() || i < other.size(); ++i)
			p[i] = poly[i] + other[i];
		return p;
	}

	Polynomial operator-() const {
		std::vector<T> a;
		for (size_t i = 0; i < poly.size(); ++i)
			a.push_back(-poly[i]);
		return a;
	}

	Polynomial operator*(const Polynomial& other) const {
		std::vector<T> p(poly.size() + other.poly.size() - 1);
		for (size_t i = 0; i < poly.size(); ++i)
			for (size_t j = 0; j < other.poly.size(); ++j)
				p[i + j] += poly[i] * other.poly[j];
		return p;
	}

	Polynomial operator-(const Polynomial& other) const { return *this + (-other); }
	Polynomial& operator+=(const Polynomial& other) { return *this = *this + other; }
	Polynomial& operator-=(const Polynomial& other) { return *this = *this - other; }
	Polynomial& operator*=(const Polynomial& other) { return *this = *this * other;	}

	T operator[](const size_t i) const {
		if (i >= poly.size())
			return T(0);
		return poly[i];
	}

	typename std::vector<T>::iterator begin() { return poly.begin(); }
	typename std::vector<T>::iterator end() { return poly.end(); }
	typename std::vector<T>::const_iterator begin() const { return poly.begin(); }
	typename std::vector<T>::const_iterator end() const { return poly.end(); }

	friend bool operator==(const T& a, const Polynomial& b) { return b == a; }
	friend bool operator!=(const T& a, const Polynomial& b) { return !(b == a);	}
	friend Polynomial operator+(const T& a, const Polynomial& b) { return b + a; };
	friend Polynomial operator-(const T& a, const Polynomial& b) { return -b + a; }
	friend Polynomial operator*(const T& a, const Polynomial& b) { return b * a; }

	T operator()(const T& a) const {
		T result = poly[0];
		T deg = a;
		for (size_t i = 1; i < poly.size(); ++i) {
			result += poly[i] * deg;
			deg *= a;
		}
		return result;
	}

	Polynomial operator&(const Polynomial& other) const {
		Polynomial p(other), result(poly[0]);
		for (size_t i = 1; i < poly.size(); ++i) {
			result += poly[i] * p;
			p *= other;
		}
		return result;
	}

	Polynomial operator/(const Polynomial& other) const {
		assertm(other != 0, "Division by 0-polynomial");
		Polynomial a(*this);
		std::vector<T> result(poly.size() + 1 - other.poly.size()), monomial = result;
		monomial.back() = T(1);
		while (a.Degree() >= other.Degree()) {
			result[a.Degree() - other.Degree()] = a.poly.back() / other.poly.back();
			Polynomial q(monomial);
			a -= q * other * result[a.Degree() - other.Degree()];
			monomial.pop_back();
			if (!monomial.empty())
				monomial.back() = T(1);
		}
		return result;
	}

	Polynomial operator%(const Polynomial& other) const {
		return *this - (*this / other) * other;
	}

	Polynomial operator,(const Polynomial& other) const {
		Polynomial a(*this), b(other);
		while (a.Degree() != -1 && b.Degree() != -1) {
			if (a.Degree() > b.Degree() ||
				(a.Degree() == b.Degree() && a.poly > b.poly))
				a = a % b;
			else
				b = b % a;
		}
		auto result = a + b;
		result = result / result.polynomial.back();
		return result;
	}

	friend std::ostream& operator<<(std::ostream& out, const Polynomial<T>& p) {
		if (p.Degree() == -1) {
			out << 0;
		}
		bool need_sign = false;
		for (int i = p.Degree(); i != -1; --i) {
			if (p[i] != T(0)) {
				if (need_sign && p[i] > T(0))
					out << "+";
				if (p[i] != T(1) && p[i] != T(-1)) {
					out << p[i];
					if (i > 0)
						out << "*";
				}
				if (p[i] == T(-1))
					out << "-";
				if (i == 0 && (p[i] == T(1) || p[i] == T(-1)))
					out << 1;
				if (i > 0)
					out << "x";
				if (i > 1)
					out << "^" << i;
				need_sign = true;
			}
		}
		return out;
	}

	friend Polynomial<T> gcd(const Polynomial<T>& a, const Polynomial<T>& b) { return (a, b); }
	friend Polynomial<T> lcm(const Polynomial<T>& a, const Polynomial<T>& b) { return a * b / gcd(a, b); }
};
