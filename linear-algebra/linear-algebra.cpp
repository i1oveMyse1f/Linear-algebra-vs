#include <iostream>
#include "polynomial.h"
#include "permutation.h"
#include "math_vector.h"
#include "rational.h"
#include "matrix.h"
#include "container_math_vectors.h"

using namespace std;

struct A {
	int x;
	explicit A(int x) : x(x) {}
	bool operator<(A other) const { return x < other.x; }
	bool operator>(A other) const { return x > other.x; }
	bool operator<=(A other) const { return x <= other.x; }
	bool operator>=(A other) const { return x >= other.x; }
	bool operator==(A other) const { return x == other.x; }
	bool operator!=(A other) const { return x != other.x; }
	A operator+(A other) const { return A(x + other.x); }
	A operator-(A other) const { return A(x - other.x); }
	A operator*(A other) const { return A(x * other.x); }
	A operator/(A other) const { return A(x / other.x); }
	A operator%(A other) const { return A(x % other.x); }
	A operator-() const { return A(-x); }
	friend istream& operator>>(istream& in, A& a) {
		return in >> a.x;
	}
	friend ostream& operator<<(ostream& out, const A& a) {
		return out << a.x;
	}
};

int main() {
	/*Matrix<Rational<int>> a(3, 3);
	cin >> a;
	ContainerMathVectors tmp(a);
	cout << tmp;*/
	Rational<A> x;
	cin >> x;
	cout << x;
}
