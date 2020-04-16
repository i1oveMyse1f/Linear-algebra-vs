#pragma once

#include <iostream>
#include <concepts>

template<class T>
concept conc_read = requires (std::istream& in, T x) {
	in >> x;
};

template<class T>
concept conc_write = requires (std::ostream& out, T x) {
	out << x;
};

template<class T>
concept conc_gcd = requires (T x, T y) {
	x% y;
	T(0);
	-x;
	x < y;
	x == y;
	x != y;
};

template<class T>
concept conc_comp = requires (T x, T y) {
	x == y;
	x != y;
	x < y;
	x > y;
	x <= y;
	x >= y;
};

template<class T>
concept conc_base_math = requires (T x, T y) {
	x + y;
	x - y;
	x / y;
	x * y;
};

template<class T>
concept conc_num = conc_gcd<T> && conc_read<T> && conc_write<T> && conc_comp<T> && conc_base_math<T>;