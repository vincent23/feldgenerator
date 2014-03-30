#ifndef PARAMS_H
#define PARAMS_H

#include <stdbool.h>

typedef struct {
	int spread;
	int scale;
	const char* in;
	const char* out;
} param_t;

bool parse_args(int argc, const char* const argv[], param_t* out_parameters);

static const int DEFAULT_SPREAD = 10;
static const int DEFAULT_SCALE = 10;

#endif
