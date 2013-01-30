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
/**
   @file URandomDeviceNumberGenerator.h
   @brief Random number generator reading from "/dev/urandom".
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef URANDOMDEVICENUMBERGENERATOR_H
#define URANDOMDEVICENUMBERGENERATOR_H

#include "AFileBasedRandomNumberGenerator.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	   	@addtogroup Random
	   	@{
	 */

	/**
	   @class URandomDeviceNumberGenerator
	   @brief Random number generator reading vom /dev/urandom.
	 */
	class URandomDeviceNumberGenerator : public AFileBasedRandomNumberGenerator
	{
		public:
			URandomDeviceNumberGenerator() : AFileBasedRandomNumberGenerator("/dev/urandom", AFileBasedRandomNumberGenerator::DEFAULT_BUFFER_SIZE) {};
			virtual ~URandomDeviceNumberGenerator() {};
			inline int32_t get_max() const { return 2147483647; };
	};

	/**
	   	@}
	   @}
	 */
}
#endif
