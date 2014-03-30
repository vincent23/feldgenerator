#include "params.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void print_help(void);

bool parse_args(const int argc, const char* const argv[], param_t* const out_parameters) {
	if(argc < 3) {
		print_help();
		return false;
	}
	out_parameters->in = NULL;
	out_parameters->out = NULL;
	out_parameters->scale = DEFAULT_SCALE;
	out_parameters->spread = DEFAULT_SPREAD;
	for(int i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			// optional argument
			if(strcmp(argv[i], "--scale") == 0) {
				i++;
				out_parameters->scale = atoi(argv[i]);
				if(out_parameters->scale <= 0) {
					fprintf(stderr, "Invalid scale set\n");
					print_help();
					return false;
				}
			} else if(strcmp(argv[i], "--spread") == 0) {
				i++;
				out_parameters->spread = atoi(argv[i]);
				if(out_parameters->spread <= 0) {
					fprintf(stderr, "Invalid spread set\n");
					print_help();
					return false;
				}
			} else {
				fprintf(stderr, "Invalid option!\n");
				print_help();
				return false;
			}
		} else {
			// required argument
			if(out_parameters->in == NULL) {
				out_parameters->in = argv[i];
			} else if(out_parameters->out == NULL) {
				out_parameters->out = argv[i];
			} else {
				// ignore additional arguments
				break;
			}
		}
	}
	return out_parameters->in != NULL && out_parameters->out != NULL;
}

static void print_help(void) {
	puts("Usage: feldgenerator [--spread spread] [--scale scale] infile outfile");
}
