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
   @file ARandomNumberGenerator.cpp
   @brief Base class for random number generators.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#include <assert.h>
#include <cmath>
#include "ARandomNumberGenerator.hpp"
#include "algorithms.hpp"

using namespace std;

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	   	@addtogroup Random
	   	@{
	 */

	int32_t ARandomNumberGenerator::get_int32(const int32_t min, const int32_t max)
	{
		uint32_t range;
		uint32_t scale;
		uint32_t limit;
		uint32_t rnd;
		int32_t result;

		assert(max >= min);

		/* this distribution algorithm assumes that the minimum number returned by
		   get_int32() is zero */
		assert(get_min_int32() == 0 && get_max_int32() > 0);

		assert_subtraction(max, min);
		range = max - min + 1; /* [min, max] is a closed interval, the length of the corresponding
		                          set is max - min + 1 (the interval [-1, 1] has the set {-1, 0, 1},
		                          for example) */

		scale = (get_max_int32() + 1) / range; /* get_int32() can return get_max_int32() + 1
		                                          different numbers */
		assert(scale > 0);

		limit = scale * range;

		// test if range exceeds the limit:
		assert(range <= limit);

		// find random number within range:
		do
		{
			rnd = get_int32();
		} while(rnd >= limit);

		result = (int32_t)rnd / scale + min;

		// additional boundary check:
		assert(result >= min && result <= max);

		return result;
	}

	double ARandomNumberGenerator::get_double(const double min, const double max)
	{
		double range;
		double result;

		assert(max > min);

		// assume that the minimum number returned by get_double() is zero:
		assert(get_min_double() == 0.0 && get_max_double() > 0.0);

		range = max - min;
		result = get_double() / get_max_double() * range + min;

		// additional boundary check:
		assert(result >= min && result <= max);

		return result;
	}

	void ARandomNumberGenerator::get_int32_seq(const int32_t min, const int32_t max, int32_t* numbers, const std::size_t length)
	{
		get_seq<int32_t, int32_t (ARandomNumberGenerator::*)(const int32_t, const int32_t)>(min, max, numbers, length, &ARandomNumberGenerator::get_int32);
	}

	void ARandomNumberGenerator::get_double_seq(const double min, const double max, double* numbers, const std::size_t length)
	{
		get_seq<double, double (ARandomNumberGenerator::*)(const double, const double)>(min, max, numbers, length, &ARandomNumberGenerator::get_double);
	}

	void ARandomNumberGenerator::get_unique_int32_seq(const int32_t min, const int32_t max, int32_t* numbers, const std::size_t length)
	{
		get_unique_seq<int32_t, int32_t (ARandomNumberGenerator::*)(const int32_t, const int32_t)>(min, max, numbers, length, &ARandomNumberGenerator::get_int32);
	}

	void ARandomNumberGenerator::get_unique_double_seq(const double min, const double max, double* numbers, const std::size_t length)
	{
		get_unique_seq<double, double (ARandomNumberGenerator::*)(const double, const double)>(min, max, numbers, length, &ARandomNumberGenerator::get_double);
	}

	/**
		   @}
	   @}
	 */
}
