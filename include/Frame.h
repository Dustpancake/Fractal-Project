//
// Created by ktulhu on 26/11/2018.
//

#ifndef PROJECT_FRAME_H
#define PROJECT_FRAME_H

#include <string>
#include "Frame.h"

namespace BWrap {

    class Frame {
    public:
        Frame() noexcept ;
        Frame(int x, int y) noexcept ;

        // no copy, only move
        Frame(const Frame&) = delete;
        Frame(Frame&& f) noexcept ;

        virtual ~Frame();

        // clear the image
        void clear() noexcept ;

        // save image with desired name
		void save(std::string name, bool regular = false) const noexcept ;

        // accessor methods
        int getX() const noexcept ;
        int getY() const noexcept ;

        // access x, y
        double& index(int x, int y) noexcept ;

        // config methods
        void set_detail_factor(double factor) noexcept ;
        void set_delta_max(double d_max) noexcept ;

    private:
		double **image;
        int xsize, ysize;
        double detail_factor;
        double delta_maximum;

        inline void free_memory() noexcept ;
        inline void allocate_memory() noexcept ;
    };

}

#endif //PROJECT_FRAME_H
