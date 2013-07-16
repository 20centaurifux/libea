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
#include "ARandomNumberGenerator.h"

using namespace std;

namespace ea
{
	int32_t ARandomNumberGenerator::get_int32(const int32_t min, const int32_t max)
	{
		uint32_t range = max - min + 1;
		uint32_t scale = (get_max_int32() + 1u) / range;
		uint32_t limit = scale * range;
		uint32_t rnd;
		int32_t result;

		assert(scale != 0);

		do
		{
			rnd = abs(get_int32());
		} while(rnd >= limit);

		result = (int32_t)rnd / scale + min;
		assert(result >= min && result <= max);

		return result;
	}

	double ARandomNumberGenerator::get_double(const double min, const double max)
	{
		double rnd_max = get_max_double() + 1.0;
		double range = max - min;
		double result;

		result = abs(get_double()) / rnd_max * range + min;
		assert(result >= min && result <= max);

		return result;
	}

	void ARandomNumberGenerator::get_int32_seq(const int32_t min, const int32_t max, int32_t* numbers, const int32_t length)
	{
		get_seq<int32_t, int32_t (ARandomNumberGenerator::*)(const int32_t, const int32_t)>(min, max, numbers, length, &ARandomNumberGenerator::get_int32);
	}

	void ARandomNumberGenerator::get_double_seq(const double min, const double max, double* numbers, const int32_t length)
	{
		get_seq<double, double (ARandomNumberGenerator::*)(const double, const double)>(min, max, numbers, length, &ARandomNumberGenerator::get_double);
	}

	void ARandomNumberGenerator::get_unique_int32_seq(const int32_t min, const int32_t max, int32_t* numbers, const int32_t length)
	{
		get_unique_seq<int32_t, int32_t (ARandomNumberGenerator::*)(const int32_t, const int32_t)>(min, max, numbers, length, &ARandomNumberGenerator::get_int32);
	}

	void ARandomNumberGenerator::get_unique_double_seq(const double min, const double max, double* numbers, const int32_t length)
	{
		get_unique_seq<double, double (ARandomNumberGenerator::*)(const double, const double)>(min, max, numbers, length, &ARandomNumberGenerator::get_double);
	}
}