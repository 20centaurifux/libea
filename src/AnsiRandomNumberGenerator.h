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
   @file AnsiRandomNumberGenerator.h
   @brief ANSI compatible random number generator.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef ANSIRANDOMNUMBERGENERATOR_H
#define ANSIRANDOMNUMBERGENERATOR_H

#include <ctime>

#include "ARandomNumberGenerator.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	   	@addtogroup Random
	   	@{
	 */

	/**
	   @class AnsiRandomNumberGenerator
	   @brief A random number generator using the Ansi C rand() function.
	 */
	class AnsiRandomNumberGenerator : public ARandomNumberGenerator
	{
		public:
			AnsiRandomNumberGenerator()
			{
				srand(time(NULL));
			}

			virtual ~AnsiRandomNumberGenerator() {}

			int32_t random()
			{
				return rand();
			}

			inline int32_t get_max() const
			{
				return RAND_MAX;
			}
	};

	/**
	   	@}
	   @}
	 */
}
#endif
