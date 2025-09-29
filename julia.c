#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include <string.h>

static const size_t MAX_ITERATIONS = 3000;

int iterate(double complex z, double complex c, size_t iterations, double complex previousPoints[]) {

	if (iterations >= MAX_ITERATIONS)
		return 0;

	z = z*z + c;

	for (size_t i = 0; i < iterations; ++i)
		if (z == previousPoints[i])
			return 1;

	previousPoints[iterations] = z;

	return iterate(z, c, iterations + 1, previousPoints);
}

char julia(double ca, double cb, double a, double b) {

	double complex previousPoints[MAX_ITERATIONS];
	int isInSet = iterate(a + b * I, ca + cb * I, 0, previousPoints);

	if (isInSet) return '#';

	else return '.';
}

int main(int argc, char** argv) {

	if (argc <= 2) {
		printf("usage: %s [c_a] [c_b]\n", argv[0]);
		return 0;
	}

	double ca = strtod(argv[1], NULL);
	double cb = strtod(argv[2], NULL);

	for (double a = -1.2; a <= 1.2; a += 0.0825) {
		for (double b = -2; b <= 2; b += 0.0625) {
			char result = julia(ca, cb, a, b);
			printf("%c", result);
		}
		printf("\n");
	}
}

