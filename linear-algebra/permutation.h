#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

class Permutation {
private:
	std::vector<size_t> a;
public:
	Permutation(size_t n = 1) {
		a.resize(n);
		for (size_t i = 0; i < n; i++)
			a[i] = i + 1;
	}
	Permutation(const std::vector<size_t>& a) : a(a) {}
	Permutation(std::initializer_list<size_t> perm) : a(perm) {}

	size_t size() const { return a.size(); }

	size_t operator[](size_t i) const { return a[i]; }
	size_t& operator[](size_t i) { return a[i]; }

	bool next() { return std::next_permutation(a.begin(), a.end());	}

	typename std::vector<size_t>::iterator begin() { return a.begin(); }
	typename std::vector<size_t>::iterator end() { return a.end(); }
	typename std::vector<size_t>::const_iterator begin() const { return a.begin(); }
	typename std::vector<size_t>::const_iterator end() const { return a.end(); }

	bool operator==(const Permutation& other) const { return a == other.a; }
	bool operator!=(const Permutation& other) const { return a != other.a; }

	Permutation operator*(const Permutation& other) const {
		size_t n = size();
		Permutation res(n);
		for (size_t i = 0; i < n; ++i)
			res[i] = a[other[i] - 1];
		return res;
	}

	Permutation inverce() const {
		size_t n = size();
		Permutation res(n);
		for (size_t i = 0; i < n; i++)
			res[a[i] - 1] = i + 1;
		return res;
	}

	int sign() {
		size_t n = size();
		size_t count_cycles = 0;
		std::vector<bool> used(n);
		for (size_t i = 0; i < n; i++)
			if (!used[i]) {
				for (size_t j = i; !used[j]; j = a[j - 1])
					used[j] = true;
				count_cycles++;
			}
		if ((n - count_cycles) & 1)
			return -1;
		else
			return 1;
	}

	friend Permutation pow(const Permutation& perm, int p) {
		size_t n = perm.size();

		std::vector<bool> used(n);
		std::vector<std::vector<size_t>> cycles;
		std::vector<size_t> ncycle(n);
		std::vector<size_t> pos_in_cycle(n);

		for (size_t i = 0; i < n; i++)
			if (!used[i]) {
				cycles.emplace_back();
				for (size_t j = i, pos = 0; !used[j]; j = perm[j] - 1, pos++) {
					cycles.back().push_back(j);
					pos_in_cycle[j] = pos;
					ncycle[j] = cycles.size() - 1;
					used[j] = true;
				}
			}

		Permutation res(n);
		for (size_t i = 0; i < n; i++) {
			size_t len_cur_cycle = cycles[ncycle[i]].size();
			res[i] = cycles[ncycle[i]][((pos_in_cycle[i] + p) % len_cur_cycle + len_cur_cycle) % len_cur_cycle] + 1;
		}
		return res;
	}

	friend std::istream& operator>>(std::istream& stream, Permutation& perm) {
		for (size_t x : perm.a)
			stream >> x;
		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const Permutation& perm) {
		for (size_t x : perm.a)
			stream << x << ' ';
		return stream;
	}
};