/***************************************************************************
    begin........: November 2012
    copyright....: Sebastian Fedrau
    email........: sebastian.fedrau@gmail.com
 ***************************************************************************/

/***************************************************************************
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License v3 as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License v3 for more details.
 ***************************************************************************/
/**
   @file ARandomNumberGenerator.hpp
   @brief Base class for random number generators.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef ARANDOMNUMBERGENERATOR_H
#define ARANDOMNUMBERGENERATOR_H

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

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
	   @brief Abstract base class for random number generators.
	 */
	class ARandomNumberGenerator
	{
		public:
			ARandomNumberGenerator() {}
			virtual ~ARandomNumberGenerator() {}

			/**
			   @return a random integer

			   Generates a random number.
			 */
			virtual int32_t get_int32() = 0;

			/**
			   @return a random double

			   Generates a random number.
			 */
			virtual double get_double() = 0;

			/**
			   @param min minimum value
			   @param max maximum value
			   @return a random integer

			   Generates a random number in the given range.
			 */
			virtual int32_t get_int32(const int32_t min, const int32_t max);

			/**
			   @param min minimum value
			   @param max maximum value
			   @return a random double

			   Generates a random number in the given range.
			 */
			virtual double get_double(const double min, const double max);

			/**
			   @param min minimum value
			   @param max maximum value
			   @param numbers location to store generated numbers
			   @param length length of the array

			   Generates multiple random integer values.
			 */
			virtual void get_int32_seq(const int32_t min, const int32_t max, int32_t* numbers, const size_t length);

			/**
			   @param min minimum value
			   @param max maximum value
			   @param numbers location to store generated numbers
			   @param length length of the array

			   Generates multiple random double values.
			 */
			virtual void get_double_seq(const double min, const double max, double* numbers, const size_t length);

			/**
			   @param min minimum value
			   @param max maximum value
			   @param numbers location to store generated numbers
			   @param length length of the array

			   Generates multiple unique integer values.
			 */
			virtual void get_unique_int32_seq(const int32_t min, const int32_t max, int32_t* numbers, const size_t length);

			/**
			   @param min minimum value
			   @param max maximum value
			   @param numbers location to store generated numbers
			   @param length length of the array

			   Generates multiple unique double values.
			 */
			virtual void get_unique_double_seq(const double min, const double max, double* numbers, const size_t length);

			/**
			   @return The maximum value the get_int32() method can return.
			 */
			virtual int32_t get_max_int32() const = 0;

			/**
			   @return The maximum value the get_double() method can return.
			 */
			virtual double get_max_double() const = 0;

			/**
			   @return The minimum value the get_int32() method can return.
			 */
			virtual int32_t get_min_int32() const = 0;

			/**
			   @return The minimum value the get_double() method can return.
			 */
			virtual double get_min_double() const = 0;

		private:
			template<typename T, typename F>
			void get_seq(const T min, const T max, T* numbers, const size_t length, F random)
			{
				assert(min <= max);
				assert(numbers != nullptr);
				assert(length > 0);

				for(size_t i = 0; i < length; ++i)
				{
					T x = (this->*random)(min, max);
					numbers[i] = x;
				}
			}

			template<typename T, typename F>
			void get_unique_seq(const T min, const T max, T* numbers, const size_t length, F random)
			{
				size_t count = 0;
				size_t i;
				T rnd;
				bool found;

				assert(min <= max);
				assert(numbers != nullptr);
				assert(length > 0);

				// test if we've found enough numbers:
				while(count != length)
				{
					// get next number:
					rnd = (this->*random)(min, max);

					// test if number is already stored in array:
					found = false;

					for(i = 0; i < count; ++i)
					{
						if(numbers[i] == rnd)
						{
							found = true;
							break;
						}
					}

					if(!found)
					{
						// add random number to array:
						numbers[count++] = rnd;
					}
				}
			}
	};

	/**
	   	@}
	   @}
	 */
}
#endif
