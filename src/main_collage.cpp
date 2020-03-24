//
// Created by ktulhu on 2018-12-02.
//

#include <Canvas.h>
#include <Benchmark.h>
#include "transform/IFSystem.h"
#include "transform/AffineTransform.h"
#include "collage/Collage.h"
#include "collage/VTio.h"
#include "collage/ConfigParse.h"
#include "transform/IFSio.h"


// init the config parser
CData ConfigParse::config = CData();

void preview(IFSystem& ifs, const CData& config) {
	BWrap::Frame frame = BWrap::image_to_frame(config.in, 240);

	ifs.preview_transforms(frame, false);
	{
		// scoped canvas to save the preview
		BWrap::Canvas preview{std::move(frame)};
		preview.save_image(config.pout, true);
	}

}

void vertices() {
	CData config = ConfigParse::config;

	std::vector<AffineTransform> afs_vec;

	if (config.triangles) {
		TCollage col(config.transforms);
		afs_vec = col.find_transform();
	} else {
		LCollage col(config.transforms);
		afs_vec = col.find_transform();
	}

	IFSystem ifs{std::move(afs_vec), std::move(config.weights)};

	BWrap::Canvas c{config.width, config.height};
	ifs.set_shift_all(c, config.shifts);
	{
		Timeit it;
		ifs.n_point_transform(c, config.n, BMShape::Coord{0, 0});
	}
	c.save_image(config.out);

	if (config.preview && config.in != "" && config.pout != "") {
		preview(ifs, config);
	}

	if (config.save && config.savefile != "") {
		ifs.save_system(config.savefile);
	}
}

void ifsystem() {
	CData config = ConfigParse::config;

	IFSystem ifs = IFio::readIFSystem(config.transforms);

	BWrap::Canvas c{config.width, config.height};
	ifs.set_shift_all(c, config.shifts);
	{
		Timeit it;
		ifs.n_point_transform(c, config.n, BMShape::Coord{0, 0});
	}
	c.save_image(config.out);

	if (config.preview && config.in != "" && config.pout != "") {
		preview(ifs, config);
	}

	if (config.save && config.savefile != "") {
		ifs.save_system(config.savefile);
	}
}

void execute() {
	if (ConfigParse::config.triangles || ConfigParse::config.lines) {
		vertices();
	} else {
		ifsystem();
	}
}

int main(int argc, char* argv[]) {
	ConfigParse cp("../configs/systems/current.ini");
	cp.parse();
	execute();
	return 0;
}