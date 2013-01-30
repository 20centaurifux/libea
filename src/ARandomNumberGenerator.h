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
   @file ARandomNumberGenerator.h
   @brief Base class for random number generators.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef ARANDOMNUMBERGENERATOR_H
#define ARANDOMNUMBERGENERATOR_H

#include <stdint.h>
#include <stdlib.h>

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	   	@addtogroup Random
	   	@{
	 */

	/**
	   @class ARandomNumberGenerator
	   @brief Abstract base class for all random number generators.
	 */
	class ARandomNumberGenerator
	{
		public:
			ARandomNumberGenerator() {}
			virtual ~ARandomNumberGenerator() {}

			/**
			   @param min minimum value
			   @param max maximum value
			   @return a random number

			   Generates a random number in the given range.
			 */
			virtual int32_t get_number(const int32_t min, const int32_t max);

			/**
			   @param min minimum value
			   @param max maximum value
			   @param numbers location to store generated numbers
			   @param length length of the array

			   Generates multiple random numbers.
			 */
			virtual void get_numbers(const int32_t min, const int32_t max, int32_t* numbers, const int32_t length);

			/**
			   @param min minimum value
			   @param max maximum value
			   @param numbers location to store generated numbers
			   @param length length of the array

			   Generates multiple unique random numbers.
			 */
			virtual void get_unique_numbers(const int32_t min, const int32_t max, int32_t* numbers, const int32_t length);

			/**
			   @return a random number

			   Generates a random number.
			 */
			virtual int32_t random() = 0;

			/**
			   @return The maximum number the random() method will return.
			 */
			virtual int32_t get_max() const = 0;
	};

	/**
	   	@}
	   @}
	 */
}
#endif
