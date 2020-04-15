#pragma once

#include <iostream>
#include <initializer_list>
#include <vector>
#include "assertm.h"

template<class T>
class MathVector {
public:
	MathVector(size_t n = 0): a(n) {}
	MathVector(std::initializer_list<T> l): a(l) {}
	MathVector(std::vector<T> a): a(a) {}

	size_t size() const { return a.size(); }

	T& operator[](size_t i) { return a[i]; }
	T operator[](size_t i) const { return a[i]; }

	MathVector operator+(const MathVector& other) {
		assertm(size() == other.size(), "Wrong MathVector sizes in operator+");
		MathVector res = *this;
		for (size_t i = 0; i < size(); i++)
			res[i] += other[i];
		return res;
	}

	MathVector operator-(const MathVector& other) {
		assertm(size() == other.size(), "Wrong MathVector sizes in operator-");
		MathVector res = *this;
		for (size_t i = 0; i < size(); i++)
			res[i] -= other[i];
		return res;
	}

	MathVector operator*(const T& coef) {
		MathVector res = *this;
		for (size_t i = 0; i < size(); i++)
			res *= coef;
		return res;
	}

	T operator*(const MathVector& other) {
		assertm(size() == other.size(), "Wrong MathVector sizes in operaor*");
		T res = 0;
		for (size_t i = 0; i < size(); i++)
			res += a[i] * other[i];
		return res;
	}

	MathVector operator-() {
		MathVector res(size());
		for (size_t i = 0; i < size(); i++)
			res[i] = -a[i];
		return res;
	}

	friend MathVector operator*(const T& coef, const MathVector& other) { return other * coef; }
	MathVector& operator+=(const MathVector& other) { return *this = *this + other; }
	MathVector& operator-=(const MathVector& other) { return *this = *this + other; }
	MathVector& operator*=(const T &coef) { return *this = *this + coef; }
	
	typename std::vector<T>::iterator begin() { return a.begin(); }
	typename std::vector<T>::iterator end() { return a.end(); }
	typename std::vector<T>::const_iterator begin() const { return a.begin(); }
	typename std::vector<T>::const_iterator end() const { return a.end(); }

	friend std::istream& operator>>(std::istream& in, MathVector& v) {
		for (auto& x : v)
			in >> x;
		return in;
	}

	friend std::ostream& operator<<(std::ostream& out, const MathVector& v) {
		out << "( ";
		for (auto& x : v)
			out << x << ' ';
		return out << ")";
	}

private:
	std::vector<T> a;
};
