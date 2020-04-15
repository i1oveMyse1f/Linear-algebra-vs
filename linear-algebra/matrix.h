#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <initializer_list>
#include <numeric>
#include <vector>

#include "permutation.h"
#include "polynomial.h"
#include "rational.h"
#include "math_vector.h"
#include "assertm.h"

// TO-DO list:
// make beauty print
// add fast characterestyc poly

template<class T> class ContainerMathVectors;

template<class T = Rational<int>>
class Matrix {
public:
	// constructions 
	Matrix();
	Matrix(size_t n, size_t m);
	Matrix(std::pair<size_t, size_t> sz);
	Matrix(const std::vector<std::vector<T>>& a);
	Matrix(const std::initializer_list<std::vector<T>>& l);
	Matrix(const ContainerMathVectors<T>& v);
	Matrix(const Matrix<T>& other);

	Matrix(Matrix&& other) noexcept;

	// base operations
	Matrix operator=(Matrix&& other);

	void resize(size_t n, size_t m);
	void resize(std::pair<size_t, size_t> sz);

	std::vector<T>& operator[](size_t i);
	const std::vector<T>& operator[](size_t i) const;

	std::pair<size_t, size_t> size() const;

	// pro base operations
	Matrix operator|(const Matrix& other) const;
	Matrix transpose() const;

	// math operations

	Matrix operator+(const Matrix& other) const;
	Matrix operator-(const Matrix& other) const;
	Matrix operator*(const Matrix& other) const;
	Matrix operator*(const T coef) const;
	Matrix operator-();

	Matrix operator+(const T& coef) const;
	Matrix operator-(const T& coef) const;

	friend Matrix operator+(const T& coef, const Matrix<T>& other);
	friend Matrix operator-(const T& coef, const Matrix<T>& other);
	friend Matrix operator*(const T& coef, const Matrix<T>& other);
	
	Matrix& operator+=(const Matrix& other);
	Matrix& operator-=(const Matrix& other);
	Matrix& operator*=(const Matrix& other);
	Matrix& operator+=(const T& coef);
	Matrix& operator-=(const T& coef);
	Matrix& operator*=(const T& coef);

	// equality operations
	bool operator==(const Matrix& other) const;
	bool operator!=(const Matrix& other) const;

	// pro-math operations
	Matrix to_stepped_view() const;
	Matrix to_improved_stepped_view() const;

	bool have_inverce() const;
	Matrix inverce() const;

	T det() const;
	T det_slow() const;

	ContainerMathVectors<T> fse() const;

	Polynomial<T> char_poly_slow() const;

	// matrix is a leaner operator
	ContainerMathVectors<T> Ker() const;
	ContainerMathVectors<T> Im() const;

	// read and write
	//friend std::istream& operator>>(std::istream& in, Matrix<T>& a);
	//friend std::ostream& operator<<(std::ostream& out, const Matrix<T>& a);

	// matrix E
	template<class T>
	friend Matrix<T> get_e_matrix(std::pair<size_t, size_t> sz);
	template<class T>
	friend Matrix<T> get_e_matrix(size_t n, size_t m);

	// binary pow
	friend Matrix pow(Matrix a, size_t deg);
private:
	std::vector<std::vector<T>> a;
};

#include "container_math_vectors.h"

template<class T>
class ContainerMathVectors;

//constructions 
template<class T>
Matrix<T>::Matrix() {}

template<class T>
Matrix<T>::Matrix(size_t n, size_t m) : a(n, vector<T>(m)) {}

template<class T>
Matrix<T>::Matrix(std::pair<size_t, size_t> sz) : a(sz.first, vector<T>(sz.second)) {}

template<class T>
Matrix<T>::Matrix(const std::vector<std::vector<T>>& a) : a(a) {}

template<class T>
Matrix<T>::Matrix(const std::initializer_list<std::vector<T>>& l) : a(l) {}

template<class T>
Matrix<T>::Matrix(const ContainerMathVectors<T>& v) {
	auto [m, n] = v.size();
	resize(n, m);
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < m; j++)
			a[i][j] = v[j][i];
}

template<class T>
Matrix<T>::Matrix(Matrix&& other) noexcept { a.swap(other.a), other.a.clear(); }

template<class T>
Matrix<T>::Matrix(const Matrix<T>& other) : a(other.a) {}

//base operators

template<class T>
Matrix<T> Matrix<T>::operator=(Matrix&& other) {
	a.swap(other.a);
	other.a.clear();
}

template<class T>
void Matrix<T>::resize(size_t n, size_t m) {
	a.resize(n);
	for (size_t i = 0; i < n; i++)
		a[i].resize(m);
}

template<class T>
void Matrix<T>::resize(std::pair<size_t, size_t> sz) { resize(sz.first, sz.second); }

template<class T>
std::vector<T>& Matrix<T>::operator[](size_t i) { return a[i]; }

template<class T>
const std::vector<T>& Matrix<T>::operator[](size_t i) const { return a[i]; }

template<class T>
std::pair<size_t, size_t> Matrix<T>::size() const {
	return { a.size(), a[0].size() };
}

// pro base operations
template<class T>
Matrix<T> Matrix<T>::operator|(const Matrix& other) const {
	auto [n, m] = size();
	auto [n1, k] = other.size();
	assertm(n == n1, "Wrong matrix sizes in operator|");

	Matrix res(n, m + k);
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < m; j++)
			res[i][j] = a[i][j];
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < k; j++)
			res[i][j + m] = other[i][j];

	return res;
}

template<class T>
Matrix<T> Matrix<T>::transpose() const {
	auto [n, m] = size();
	Matrix<T> res(m, n);
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < m; j++)
			res[j][i] = a[i][j];
	return res;
}

// math operations
template<class T>
Matrix<T> Matrix<T>::operator+(const Matrix& other) const {
	auto [n, m] = size();
	auto [n1, m1] = other.size();
	assertm(n == n1 && m == m1, "Wrong matrix sizes in operator+");

	Matrix res = a;
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < m; j++)
			res[i][j] += other[i][j];
	return res;
}

template<class T>
Matrix<T> Matrix<T>::operator-(const Matrix& other) const {
	auto [n, m] = size();
	auto [n1, m1] = other.size();
	assertm(n == n1 && m == m1, "Wrong matrix sizes in operator-");

	Matrix res = a;
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < m; j++)
			res[i][j] -= other[i][j];
	return res;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const Matrix& other) const {
	auto [n, k] = size();
	auto [k1, m] = other.size();
	assertm(k == k1, "Wrong matrix sizes in operaor*");

	Matrix res(n, m);
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < m; j++)
			for (size_t t = 0; t < k; t++)
				res[i][j] += a[i][t] * other[t][j];

	return res;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const T coef) const {
	auto [n, m] = size();
	Matrix res = *this;
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < m; j++)
			res[i] *= coef;
	return res;
}

template<class T>
Matrix<T> Matrix<T>::operator-() {
	auto [n, m] = size();
	Matrix res(n, m);
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < m; j++)
			res[i][j] = -a[i][j];
	return res;
}

template<class T>
Matrix<T> Matrix<T>::operator+(const T& coef) const { return *this + coef * get_e_matrix(size()); }
template<class T>
Matrix<T> Matrix<T>::operator-(const T& coef) const { return *this - coef * get_e_matrix(size()); }

template<class T>
Matrix<T> operator+(const T& coef, const Matrix<T>& other) { return other + coef; }
template<class T>
Matrix<T> operator-(const T& coef, const Matrix<T>& other) { return -other + coef; }
template<class T>
Matrix<T> operator*(const T& coef, const Matrix<T>& other) { return other * coef; }

template<class T>
Matrix<T>& Matrix<T>::operator+=(const Matrix& other) { return *this = *this + other; }
template<class T>
Matrix<T>& Matrix<T>::operator-=(const Matrix& other) { return *this = *this - other; }
template<class T>
Matrix<T>& Matrix<T>::operator*=(const Matrix& other) { return *this = *this * other; }
template<class T>
Matrix<T>& Matrix<T>::operator+=(const T& coef) { return *this = *this + coef; }
template<class T>
Matrix<T>& Matrix<T>::operator-=(const T& coef) { return *this = *this - coef; }
template<class T>
Matrix<T>& Matrix<T>::operator*=(const T& coef) { return *this = *this * coef; }

// equality operations
template<class T>
bool Matrix<T>::operator==(const Matrix& other) const { return a == other.a; }
template<class T>
bool Matrix<T>::operator!=(const Matrix& other) const { return a != other.a; }

template<class T>
Matrix<T> Matrix<T>::to_stepped_view() const {
	auto [n, m] = size();
	Matrix res = *this;
	for (size_t i = 0; i < n; i++) {
		for (size_t j = i; j < n; j++)
			if (res[j][i] != 0 && (res[i][i] == 0 || abs(res[j][i]) < abs(res[i][i])))
				res[j].swap(res[i]);

		if (res[i][i] == 0)
			continue;
		if (res[i][i] < 0)
			for (size_t j = i; j < m; j++)
				res[i][j] *= -1;

		for (size_t j = i + 1; j < n; j++) {
			T coef = -res[j][i] / res[i][i];
			for (size_t k = i; k < m; k++)
				res[j][k] += coef * res[i][k];
		}
	}
	return res;
}

template<class T>
Matrix<T> Matrix<T>::to_improved_stepped_view() const {
	auto [n, m] = size();
	Matrix res = this->to_stepped_view();
	for (int i = (int)n - 1; i >= 0; i--) {
		T coef = res[i][i];
		if (coef == 0)
			continue;
		for (size_t j = i; j < m; j++)
			res[i][j] /= coef;

		for (size_t j = 0; j < (size_t)i; j++) {
			coef = -res[j][i];
			for (size_t k = i; k < m; k++)
				res[j][k] += coef * res[i][k];
		}
	}
	return res;
}

template<class T>
Matrix<T> Matrix<T>::inverce() const {
	auto [n, m] = size();
	assertm(n == m, "Wrong matrix sizes in inverce");
	auto tmp = *this | get_e_matrix(n, m);
	tmp = tmp.to_improved_stepped_view();
	for (size_t i = 0; i < n; i++)
		assertm(tmp[i][i] == 1, "Matrix hasn't inverce");
	Matrix res(n, n);
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < n; j++)
			res[i][j] = tmp[i][j + n];
	return res;
}

template <class T>
T Matrix<T>::det() const {
	auto [n, m] = size();
	assertm(n == m, "Wrong matrix sizes in determinate");
	Matrix stepped = to_stepped_view();
	T res = 1;
	for (size_t i = 0; i < n; i++)
		res *= stepped[i][i];
	return res;
}

template <class T>
bool Matrix<T>::have_inverce() const {
	return det() != 0;
}

template <class T>
T Matrix<T>::det_slow() const {
	auto [n, m] = size();
	assertm(n == m, "Wrong matrix sizes in slow determinate");
	T res = T(0);
	Permutation perm(n);
	do {
		T cur = T(perm.sign());
		for (size_t i = 0; i < n; i++)
			cur *= a[i][perm[i] - 1];
		res += cur;
	} while (perm.next());
	return res;
}

template<class T>
ContainerMathVectors<T> Matrix<T>::fse() const {
	auto sv = this->to_improved_stepped_view();
	auto [n, m] = size();
	std::vector<bool> is_main(m, false);
	std::vector<size_t> pos_main;
	for (size_t i = 0, j = 0; i < n; i++) {
		while (j < m && sv[i][j] == 0)
			++j;
		if (j < m) {
			pos_main.push_back(j);
			is_main[j++] = true;
		}
	}
	reverse(pos_main.begin(), pos_main.end());
	
	ContainerMathVectors<T> res;
	for (size_t i = 0; i < m; i++)
		if (!is_main[i]) {
			MathVector<T> cur(m);
			cur[i] = 1;
			size_t i1 = pos_main.size() - 1;
			for (auto j : pos_main) {
				T sum = 0;
				for (size_t k = j + 1; k < m; k++)
					sum += cur[k] * sv[i1][k];
				cur[j] = -sum;
				--i1;
			}
			res.push_back(cur);
		}
	return res;
}

template<class T>
Polynomial<T> Matrix<T>::char_poly_slow() const {
	auto [n, m] = size();
	Matrix<Polynomial<Rational<int>>> tmp(n, m);
	for (size_t i = 0; i < n; i++) {
		tmp[i][i] = Polynomial<Rational<int>>({ 0, 1 });
		for (size_t j = 0; j < m; j++)
			tmp[i][j] -= -a[i][j];
	}
	return tmp.det_slow();
}

// matrix is a leaner operator
template<class T>
ContainerMathVectors<T> Matrix<T>::Im() const {
	auto [n, m] = size();
	Matrix add(m, 1);
	for (size_t i = 0; i < m; i++)
		add[i][0] = i;
	auto tr = this->transpose();
	auto tmp = tr | add;
	ContainerMathVectors<T> res;
	for (size_t i = 0; i < m; i++) {
		for (size_t j = i; j < m; j++)
			if (tmp[j][i] != 0 && (tmp[i][i] == 0 || abs(tmp[j][i]) < abs(tmp[i][i])))
				tmp[j].swap(tmp[i]);

		if (tmp[i][i] == 0)
			continue;
		for (size_t j = i + 1; j < m; j++) {
			T coef = -tmp[j][i] / tmp[i][i];
			for (size_t k = i; k < n; k++)
				tmp[j][k] += coef * tmp[i][k];
		}
		res.push_back(tr[i]);
	}
	return res;
}

template<class T>
ContainerMathVectors<T> Matrix<T>::Ker() const {
	return this->fse();
}

//read and write
template<class T>
std::istream& operator>>(std::istream& in, Matrix<T>& a) {
	auto [n, m] = a.size();
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < m; j++)
			in >> a[i][j];
	return in;
}

template<class T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& a) {
	auto [n, m] = a.size();
	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < m; j++)
			out << a[i][j] << '\t';
		out << '\n';
	}
	return out;
}

// E
template<class T>
Matrix<T> get_e_matrix(size_t n, size_t m) {
	Matrix res(n, m);
	for (size_t i = 0; i < n && i < m; i++)
		res[i][i] = 1;
	return res;
}

template<class T>
Matrix<T> get_e_matrix(std::pair<size_t, size_t> sz) {
	return Matrix<T>::get_e_matrix(sz.first, sz.second);
}

template<class T>
Matrix<T> pow(Matrix<T> a, size_t deg) {
	auto [n, m] = a.size();
	assertm(n == m, "Wrong matrix sizes in pow");
	Matrix res = get_e_matrix(n, m);
	while (deg) {
		res *= res;
		if (deg & 1)
			res *= a;
		deg >>= 1;
	}
	return res;
}

template <class T> 
T abs(T x) {
	if (x < 0)
		return -x;
	else
		return x;
}
