#include <stdio.h>
#include <complex.h>

static const size_t MAX_ITERATIONS = 1500;

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
	for (int i = 0; i <= 34; ++i) {
		for (int j = 0; j <= 70; ++j) {

			double a = -1.4 + 0.026 * j;
			double b = -0.85 + 0.05 * i;

			char result = mandelbrot(a, b);
			printf("%c", result);
		}
		printf("\n");
	}
}

