#include <stdio.h>
#include <stdlib.h>

#include <IL/il.h>

#include "params.h"
#include "convert.h"

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
	const ILint out_width = width / parameters.scale;
	const ILint out_height = height / parameters.scale;

	ILubyte out_data[out_width * out_height];

	const image_t in = {
		.width = width,
		.height = height,
		.data = ilGetData(),
	};
	const image_t out = {
		.width = out_width,
		.height = out_height,
		.data = out_data,
	};
	convert(in, out, parameters);

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
