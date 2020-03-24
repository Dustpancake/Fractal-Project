//
// Created by ktulhu on 26/11/2018.
//

#include "Frame.h"
#include "bmplib/EasyBMP.h"
#include <memory>
#include <iostream>

BWrap::Frame::Frame() noexcept
        :   xsize{0},   ysize{0},	detail_factor{1},
        	delta_maximum{0.9}	{
}

BWrap::Frame::Frame(int x, int y) noexcept
        :   xsize{x},   ysize{y},	detail_factor{1},
			delta_maximum{0.9}	{
	allocate_memory();
    clear();
}

BWrap::Frame::~Frame() {
    free_memory();
    delete image;
}

void BWrap::Frame::clear() noexcept {
    // set all cells to 0
    for (int i = 0, j; i < xsize; ++i) {
        for (j = 0; j < ysize; ++j) {
            image[i][j] = 0;
        }
    }
}

void BWrap::Frame::save(std::string name, bool regular) const noexcept {
    double min = 9e9;
    double max = -9e9;
    for(int j, i=0; i < xsize ; ++i)
    {
        for(j=0; j <ysize; ++j)
        {
            if(image[i][j] < min) {
                min = image[i][j];
            }
            if(image[i][j] > max) {
                max = image[i][j];
            }
        }
    }

    std::cout << "Determined max = " << max << "\nDetermined min = " << min << std::endl;

    BMP Output;
    Output.SetSize(xsize, ysize);
    Output.SetBitDepth(32);

    // pixel change multiplier if needed
    double delta;

    // paint lambdas
	std::function<void(int, int)> paint;
	if (!regular) {
		paint = [&](int i, int j) {
			double scaled_value = 1 - (image[i][j] - min )/( max-min + 1e-16) * detail_factor;
			auto pixel_value = (ebmpBYTE) ( scaled_value * 220.0 );
			if (pixel_value != 220) {
				delta = delta_maximum;
			} else delta = 1;
			Output(i,j)->Red 	= 	220 - pixel_value * delta;
			Output(i,j)->Green 	= 	220 - pixel_value * delta;
			Output(i,j)->Blue 	= 	220 - pixel_value * delta;
		};
	} else {
		paint = [&](int i, int j) {
			double scaled_value = 1 - ( image[i][j] - min )/( max-min + 1e-16 );
			auto pixel_value = (ebmpBYTE) ( scaled_value * 255.0 );
			// std::cout << (int) pixel_value << std::endl;
			Output(i,j)->Red = pixel_value;
			Output(i,j)->Green = pixel_value;
			Output(i,j)->Blue = pixel_value;
		};
	}

    for(int j, i=0; i < xsize; ++i)
    {
        for(j=0; j < ysize; ++j)
        {
			paint(i, j);
        }
    }

    Output.WriteToFile(name.c_str());
    std::cout << "Saved file '" << name << "'." << std::endl;
}

int BWrap::Frame::getX() const noexcept {
    return xsize;
}

int BWrap::Frame::getY() const noexcept {
    return ysize;
}

double& BWrap::Frame::index(int x, int y) noexcept {
    return image[x][y];
}

BWrap::Frame::Frame(BWrap::Frame &&f) noexcept {
    // implement move semantics
	xsize = f.xsize;
	ysize = f.ysize;

	// move new frame
	image = f.image;

	// when f's destructor is called, ensure doesn't delete image values
	f.image = nullptr;
    f.xsize = 0;
    f.ysize = 0;

    delta_maximum = f.delta_maximum;
    detail_factor = f.detail_factor;

}

void BWrap::Frame::free_memory() noexcept {
    // delete rows
    for (int i = 0; i < xsize; ++i) {
        delete image[i];
    }

    // delete columns
    free(image);
    image = nullptr;
}

void BWrap::Frame::allocate_memory() noexcept {
	image = new double*[xsize];

	// create rows
	for (int i = 0; i < xsize; ++i) {
		image[i] = new double[ysize];
	}
}

void BWrap::Frame::set_detail_factor(double factor) noexcept {
	detail_factor = factor;
}

void BWrap::Frame::set_delta_max(double d_max) noexcept {
	delta_maximum = d_max;
}
