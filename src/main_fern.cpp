//
// Created by ktulhu on 28/11/2018.
//

#include <iostream>
#include <Shapes.h>
#include <Canvas.h>
#include <Benchmark.h>
#include "transform/IFSio.h"
#include "transform/IFSystem.h"

int main() {
	BWrap::Canvas c{1000, 1000};
	IFSystem ifs = IFio::readIFSystem("../configs/transforms/chaos_attractors/barnsley_fern.txt");

	// special cases, i.e. barnsley fern
	ifs.set_shift_special(c, 'b');
	{
		Timeit t;
		ifs.n_point_transform(c, 1e8, BMShape::Coord{0.5, 0});
	}
	c.set_detail_factor(20);
	c.save_image("../imgs/output/chaos/fern_image.bmp");
	return 0;
}