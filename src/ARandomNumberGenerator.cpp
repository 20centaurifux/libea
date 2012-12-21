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
 * \file ARandomNumberGenerator.cpp
 * \brief Base class for random number generators.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include <assert.h>
#include "ARandomNumberGenerator.h"

using namespace std;

namespace ea
{
	int32_t ARandomNumberGenerator::get_number(const int32_t min, const int32_t max)
	{
		uint32_t range = max - min + 1;
		uint32_t scale = (get_max() + 1u) / range;
		uint32_t limit = scale * range;
		uint32_t rnd;

		do
		{
			rnd = abs(random());
		} while(rnd > limit);

		return (int32_t)rnd / scale + min;
	}

	void ARandomNumberGenerator::get_numbers(const int32_t min, const int32_t max, int32_t* numbers, const int32_t length)
	{
		assert(min < max);
		assert(numbers != NULL);
		assert(length > 0);

		for(int32_t i = 0; i < length; i++)
		{
			numbers[i] = get_number(min, max);
		}
	}

	void ARandomNumberGenerator::get_unique_numbers(const int32_t min, const int32_t max, int32_t* numbers, const int32_t length)
	{
		int32_t count = 0;
		int32_t i;
		int32_t rnd;
		bool found;

		assert(min < max);
		assert(numbers != NULL);
		assert(length > 0);
		assert(length - 1 <= max - min);

		// test if we've found enough numbers:
		while(count != length)
		{
			// get next number:
			rnd = get_number(min, max);

			// test if number is already stored in array:
			found = false;

			for(i = 0; i < count; i++)
			{
				if(numbers[i] == rnd)
				{
					found = true;
					break;
				}
			}

			if(!found)
			{
				// and random number to array:
				numbers[count++] = rnd;
			}
		}
	}
}
