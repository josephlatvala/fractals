#include <stdio.h>
#include <complex.h>

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

char mandelbrot(double a, double b) {

	double complex previousPoints[MAX_ITERATIONS];
	int isInSet = iterate(0, a + b * I, 0, previousPoints);

	if (isInSet) return '#';

	else return '.';
}

int main() {
	for (double b = -0.85; b <= 0.9; b += 0.05) {
		for (double a = -1.5; a <= 1; a += 0.04) {
			char result = mandelbrot(a, b);
			printf("%c", result);
		}
		printf("\n");
	}
}

