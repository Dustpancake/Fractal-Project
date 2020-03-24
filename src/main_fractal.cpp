#include <Canvas.h>
#include <Shapes.h>
#include <memory>
#include "chaos/ChaosFractal.h"
#include "chaos/ChaosShapes.h"
#include <Benchmark.h>


#define XSIZE 900
#define YSIZE 1000


int main() {
	// constructors work x, y
	BWrap::Canvas canvas{XSIZE, YSIZE};

	// set custom save parameters
	canvas.set_detail_factor(1);
	canvas.set_delta_max(0.95);

	CShape::RegularPolygon p1{5, XSIZE, YSIZE};

	ChaosFractal CF{p1};
	CF.set_init_point(XSIZE/2, YSIZE/2);

	{
		Timeit t;	// benchmark
		std::cout << "Populating frame..." << std::endl;
		CF.trace_n_points(canvas, 1e7);
		std::cout << "Done!" << std::endl;
	}
	canvas.save_image("../imgs/output/chaos/fractal.bmp");

	canvas.clear();

	CF.set_init_point(XSIZE/2, YSIZE/2);
	CF.set_fraction(0.5);
	{
		Timeit t;	// benchmark
		std::cout << "Populating frame with weightings and factor 1/3" << std::endl;
		CF.trace_n_weighted_points(canvas, 1e7, std::vector<double>{0.15, 0.15, 0.2, 0.3, 0.2});
		//CF.trace_n_points(canvas, 1e7);
		std::cout << "Done!" << std::endl;
	}
	canvas.save_image("../imgs/output/chaos/fractal_weight_image.bmp");
}