#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <complex.h>

const int DEFAULT_IMAGE_WIDTH  = 256;
const int DEFAULT_IMAGE_HEIGHT = 256;

const char* DEFAULT_IMAGE_NAME = "mandelbrot.png";

static const size_t DEFAULT_MAX_ITERATIONS = 1500;

typedef struct{
	int red;
	int green;
	int blue;
} pixel;

int iterate(double complex z, double complex c, size_t iterations, double complex previousPoints[], size_t max_iterations) {

	if (iterations >= max_iterations)
		return 0;

	z = z*z + c;

	for (size_t i = 0; i < iterations; ++i)
		if (z == previousPoints[i])
			return 1;

	previousPoints[iterations] = z;

	return iterate(z, c, iterations + 1, previousPoints, max_iterations);
}

pixel mandelbrot(double a, double b, size_t max_iterations) {

	double complex previousPoints[max_iterations];
	int isInSet = iterate(0, a + b * I, 0, previousPoints, max_iterations);

	const pixel WHITE = {.red = 255, .green=255, .blue=255};
	const pixel BLACK = {.red = 0, .green=0, .blue=0};

	if (isInSet) return BLACK;

	else return WHITE;
}

int main(int argc, char** argv) {

	const char* image_name = DEFAULT_IMAGE_NAME;

	int image_width  = DEFAULT_IMAGE_WIDTH;
	int image_height = DEFAULT_IMAGE_HEIGHT;

	int max_iterations = DEFAULT_MAX_ITERATIONS;

	if (argc >= 2) image_name = argv[1];

	if (argc >= 4) {
		int arg_width  = atoi(argv[2]);
		int arg_height = atoi(argv[3]);

		if (arg_width <= 0 || arg_height <= 0) {
			perror("invalid argument");
		}
		else {
			image_width  = arg_width;
			image_height = arg_height;
		}
	}

	if (argc >= 5) {
		int arg_iterations = atoi(argv[4]);
		if (arg_iterations <= 0) {
			perror("invalid argument");
		}
		else {
			max_iterations = arg_iterations;
		}
	}

	FILE* fp = fopen(image_name, "wb");
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
		image_width, image_height,
		8, PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);

	printf("Rendering to %s\n", image_name);

	png_byte** rows = png_malloc(png_ptr, image_height * sizeof(png_byte*));
	for (int y = 0; y < image_height; ++y) {
		printf("Rendering row %d\r", y + 1);
		fflush(stdout);
		rows[y] = png_malloc(png_ptr, image_width * sizeof(png_byte) * 3);
		for (int x = 0; x < image_width; ++x) {

			double a = ((double) x / image_width  * 3) - 2;
			double b = ((double) y / image_height * 3) - 1.5;

			pixel pixel_data = mandelbrot(a, b, max_iterations);

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
