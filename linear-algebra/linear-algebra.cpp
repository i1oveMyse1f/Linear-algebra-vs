#include <iostream>
#include "polynomial.h"
#include "permutation.h"
#include "math_vector.h"
#include "rational.h"
#include "matrix.h"
#include "container_math_vectors.h"

using namespace std;

int main() {
	Matrix<Rational<int>> a(3, 3);
	cin >> a;
	ContainerMathVectors<Rational<int>> mv(a);
	cout << mv.basis();
}
