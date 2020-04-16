#pragma once

#include <iostream>
#include <vector>

#include "math_vector.h"
#include "rational.h"

// TO-DO list:
// add inersect
// add sum
// add solve

template<class T> class Matrix;

template<class T = Rational<int>>
class ContainerMathVectors {
private:
	std::vector<MathVector<T>> v;
public:
	ContainerMathVectors() {}
	ContainerMathVectors(std::vector<MathVector<T>> a);
	ContainerMathVectors(std::initializer_list<MathVector<T>>& v);
	ContainerMathVectors(Matrix<T> m);

	std::pair<size_t, size_t> size() const;
	ContainerMathVectors basis();

	MathVector<T>& operator[](size_t i);
	const MathVector<T>& operator[](size_t i) const;

	void push_back(const MathVector<T>& a);
	void pop_back();

	typename std::vector<MathVector<T>>::iterator begin();
	typename std::vector<MathVector<T>>::iterator end();
	typename std::vector<MathVector<T>>::const_iterator begin() const;
	typename std::vector<MathVector<T>>::const_iterator end() const;

	template<class T>
	friend std::ostream& operator<<(std::ostream& out, const ContainerMathVectors& cont);
};

#include "matrix.h"

template<class T>
ContainerMathVectors<T>::ContainerMathVectors(std::vector<MathVector<T>> a) : v(a) {}

template<class T>
ContainerMathVectors<T>::ContainerMathVectors(std::initializer_list<MathVector<T>>& v) : v(v) {}

template<class T>
ContainerMathVectors<T>::ContainerMathVectors(Matrix<T> other) {
	auto [m, n] = other.size();
	v.resize(m, MathVector<T>(n));
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < m; j++)
			v[j][i] = other[i][j];
}

template<class T>
std::pair<size_t, size_t> ContainerMathVectors<T>::size() const {
	if (v.size())
		return { v.size(), v[0].size() };
	else
		return { 0, 0 };
}

template<class T>
ContainerMathVectors<T> ContainerMathVectors<T>::basis() {
	return Matrix<T>(*this).Im();
}

template<class T>
MathVector<T>& ContainerMathVectors<T>::operator[](size_t i) {
	return v[i];
}

template<class T>
const MathVector<T>& ContainerMathVectors<T>::operator[](size_t i) const {
	return v[i];
}

template<class T>
void ContainerMathVectors<T>::push_back(const MathVector<T>& a) { v.push_back(a); }
template<class T>
void ContainerMathVectors<T>::pop_back() { v.pop_back(); }

template<class T>
typename std::vector<MathVector<T>>::iterator ContainerMathVectors<T>::begin() { return v.begin(); }
template<class T>
typename std::vector<MathVector<T>>::iterator ContainerMathVectors<T>::end() { return v.end(); }
template<class T>
typename std::vector<MathVector<T>>::const_iterator ContainerMathVectors<T>::begin() const { return v.begin(); }
template<class T>
typename std::vector<MathVector<T>>::const_iterator ContainerMathVectors<T>::end() const { return v.end(); }

template<class T>
std::ostream& operator<<(std::ostream& out, const ContainerMathVectors<T>& cont) {
	for (auto v : cont)
		out << v << '\n';
	return out;
}
