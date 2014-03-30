#include <math.h>

#include "convert.h"
#include "util.h"

void convert(const image_t in, const image_t out, const param_t parameters) {
	const int scale = parameters.scale;
	const int spread = parameters.spread;

	for(int out_y = 0; out_y < out.height; out_y++) {
		const int orig_y = out_y * scale + scale / 2;
		for(int out_x = 0; out_x < out.width; out_x++) {
			const int orig_x = out_x * scale + scale / 2;
			// only check blue channel
			const bool inside = in.data[3 * (in.width * orig_y + orig_x)] == 0xFF;
			int mindist2 = spread * spread;
			for(int y = imax(orig_y - spread, 0); y < imin(orig_y + spread, in.height); y++) {
				for(int x = imax(orig_x - spread, 0); x < imin(orig_x + spread, in.width); x++) {
					if(inside != (in.data[3 * (in.width * y + x)] == 0xFF)) {
						const int dx = orig_x - x;
						const int dy = orig_y - y;
						const int dist2 = dx * dx + dy * dy;
						mindist2 = imin(dist2, mindist2);
					}
				}
			}

			const double dist = sqrt(mindist2);
			out.data[ (out.width * out_y + out_x)] = (inside ? -1 : 1) * dist / spread * 0x7F + 0x7F;
		}
	}
}
