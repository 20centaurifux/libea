/***************************************************************************
    begin........: November 2012
    copyright....: Sebastian Fedrau
    email........: lord-kefir@arcor.de
 ***************************************************************************/

/***************************************************************************
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.
 ***************************************************************************/
/*!
 * \file RandomDeviceNumberGenerator.h
 * \brief Random number generator reading from "/dev/random".
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef RANDOMDEVICENUMBERGENERATOR_H
#define RANDOMDEVICENUMBERGENERATOR_H

#include "AFileBasedRandomNumberGenerator.h"

namespace ea
{
	class RandomDeviceNumberGenerator : public AFileBasedRandomNumberGenerator
	{
		public:
			RandomDeviceNumberGenerator() : AFileBasedRandomNumberGenerator("/dev/random", AFileBasedRandomNumberGenerator::DEFAULT_BUFFER_SIZE) {};
			virtual ~RandomDeviceNumberGenerator() {};
			inline int32_t get_max() const { return 2147483647; };
	};
}
#endif
