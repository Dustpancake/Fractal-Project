//
// Created by ktulhu on 29/11/2018.
//

#ifndef PROJECT_IFSIO_H
#define PROJECT_IFSIO_H

#include <string>
#include <vector>
#include <functional>
#include "AffineTransform.h"
#include "IFSystem.h"

namespace IFio {

	struct ATReader {
		ATReader() noexcept ;
		virtual ~ATReader() noexcept ;

		void read(const std::string& path) noexcept ;
		std::vector<AffineTransform> transforms;
		std::vector<double> weights;

	};

	IFSystem readIFSystem(const std::string &path) noexcept ;

}


#endif //PROJECT_IFSIO_H
