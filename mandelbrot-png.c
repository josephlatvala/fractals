#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <complex.h>

const int IMAGE_WIDTH = 256;
const int IMAGE_HEIGHT = 256;

const char* IMAGE_NAME = "test.png";

static const size_t MAX_ITERATIONS = 1500;

typedef struct{
	int red;
	int green;
	int blue;
} pixel;

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

pixel mandelbrot(double a, double b) {

	double complex previousPoints[MAX_ITERATIONS];
	int isInSet = iterate(0, a + b * I, 0, previousPoints);

	const pixel WHITE = {.red = 255, .green=255, .blue=255};
	const pixel BLACK = {.red = 0, .green=0, .blue=0};

	if (isInSet) return BLACK;

	else return WHITE;
}

int main(int argc, char** argv) {
	FILE* fp = fopen(IMAGE_NAME, "wb");
	if (!fp) {
		perror("file init");
		exit(1);
	}
	
	png_structp png_ptr = png_create_write_struct(
			PNG_LIBPNG_VER_STRING,
			NULL, NULL, NULL);
	if (!png_ptr) {
		perror("png write init");
		exit(1);
	}

	png_infop png_info_ptr = png_create_info_struct(png_ptr);
	if (!png_info_ptr) {
		perror("png info init");
		exit(1);
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &png_info_ptr);
		fclose(fp);
		exit(1);
	}

	png_set_IHDR(png_ptr, png_info_ptr,
		IMAGE_WIDTH, IMAGE_HEIGHT,
		8, PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);

	printf("Rendering to %s\n", IMAGE_NAME);

	png_byte** rows = png_malloc(png_ptr, IMAGE_HEIGHT * sizeof(png_byte*));
	for (int y = 0; y < IMAGE_HEIGHT; ++y) {
		printf("Rendering row %d\r", y);
		fflush(stdout);
		rows[y] = png_malloc(png_ptr, IMAGE_WIDTH * sizeof(png_byte) * 3);
		for (int x = 0; x < IMAGE_WIDTH; ++x) {

			double a = ((double) x / IMAGE_WIDTH  * 3) - 2;
			double b = ((double) y / IMAGE_HEIGHT * 3) - 1.5;

			pixel pixel_data = mandelbrot(a, b);

			rows[y][x*3]   = pixel_data.red;
			rows[y][x*3+1] = pixel_data.green;
			rows[y][x*3+2] = pixel_data.blue;
		}
	}
	printf("\n");

	png_set_rows(png_ptr, png_info_ptr, rows);

	png_init_io(png_ptr, fp);

	png_write_png(png_ptr, png_info_ptr, 0, NULL);
}
