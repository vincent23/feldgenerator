#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <IL/il.h>

#include "params.h"

static int imin(int a, int b);
static int imax(int a, int b);

int main(const int argc, const char* const argv[]) {
	param_t parameters;
	if(!parse_args(argc, argv, &parameters)) {
		return EXIT_FAILURE;
	}

	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);
	const ILboolean load_success = ilLoadImage(parameters.in);
	if(load_success== IL_FALSE) {
		// handle error TODO
		ilDeleteImages(1, &image);
		return EXIT_FAILURE;
	}
	const ILboolean convert_success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	if(convert_success == IL_FALSE) {
		// handle error TODO
		ilDeleteImages(1, &image);
		return EXIT_FAILURE;
	}

	const ILint width = ilGetInteger(IL_IMAGE_WIDTH);
	const ILint height = ilGetInteger(IL_IMAGE_HEIGHT);


	const int scale = parameters.scale;
	const int spread = parameters.spread;
	const ILint out_width = width / scale;
	const ILint out_height = height / scale;

	ILubyte out_data[out_width * out_height];


	const ILubyte* const data = ilGetData();

	for(int out_y = 0; out_y < out_height; out_y++) {
		const int orig_y = out_y * scale + scale / 2;
		for(int out_x = 0; out_x < out_width; out_x++) {
			const int orig_x = out_x * scale + scale / 2;

			// only check blue channel
			const bool inside = data[3 * (width * orig_y + orig_x)] == 0xFF;
			int mindist2 = spread * spread;
			for(int y = imax(orig_y - spread, 0); y < imin(orig_y + spread, height); y++) {
				for(int x = imax(orig_x - spread, 0); x < imin(orig_x + spread, width); x++) {
					if(inside != (data[3 * (width * y + x)] == 0xFF)) {
						const int dx = orig_x - x;
						const int dy = orig_y - y;
						const int dist2 = dx * dx + dy * dy;
						mindist2 = imin(dist2, mindist2);
					}
				}
			}

			const double dist = sqrt(mindist2);
			out_data[ (out_width * out_y + out_x)] = (inside ? -1 : 1) * dist / spread * 0x7F + 0x7F;
		}
	}
	ilDeleteImages(1, &image);

	ILuint out_image;
	ilGenImages(1, &out_image);
	ilBindImage(out_image);

	ilTexImage(out_width, out_height, 1, 1, IL_LUMINANCE, IL_UNSIGNED_BYTE, &out_data);
	ilEnable(IL_FILE_OVERWRITE);
	ilSaveImage(parameters.out);
	ilDeleteImages(1, &image);
	return EXIT_SUCCESS;
}

static int imin(const int a, const int b) {
	return (a < b) ? a : b;
}

static int imax(const int a, const int b) {
	return (a > b) ? a : b;
}
