//
// Created by ktulhu on 26/11/2018.
//

#ifndef PROJECT_CANVAS_H
#define PROJECT_CANVAS_H

#include <iostream>
#include <vector>
#include "Shapes.h"
#include "Frame.h"

namespace BWrap {

    class Canvas {
    public:
        Canvas() = delete;
        // move canvas
        explicit Canvas(Frame&& f) noexcept ;

        // copy canvas
        // excplicit Canvas(const Frame& f) noexcept ;

        Canvas(int xsize, int ysize) noexcept ;

        ~Canvas();

        // adds shape using [i][j] += 1
        void trace_shape(const BMShape::Shape &s) noexcept ;
        void trace_point(int x, int y) noexcept ;
        void trace_point(const BMShape::Coord &c) noexcept ;
        void trace_points(const std::vector<BMShape::Coord> &v) noexcept ;

        // adds shape using [i][j] = 1
        void fill_point(const BMShape::Coord &c) noexcept ;

        // get dimensions
        int getXsize() const noexcept ;
        int getYsize() const noexcept ;

        // config methods
        void set_detail_factor(double fact) noexcept ;
        void set_delta_max(double d_max) noexcept ;

        // save the frame
        void save_image(const std::string& name, bool regular = false) const noexcept ;

        // clear the contained frame
        void clear() noexcept ;

    private:
        Frame frame;

    };

    // read in image, return Frame object
    Frame image_to_frame(const std::string &path, int tolerance = 255) noexcept ;

}

#endif //PROJECT_CANVAS_H
