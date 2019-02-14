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
   @file Random.hpp
   @brief Random number generation utilities.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_RANDOM_HPP
#define EA_RANDOM_HPP

#include <random>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <limits>

namespace ea::random
{
	/**
	   @addtogroup Utils
	   @{
	 */

	/*! Default random engine. */
	using RandomEngine = std::mt19937;

	/**
	   @return a new instance of the default random engine

	   Creates and seeds an instance of the default random engine.
	 */
	RandomEngine default_engine();

	/**
	   @tparam OutputIterator must meet the requirements of LegacyOutputIterator
	   @tparam T the type of numbers generated
	   @param first iterator pointing to the first element of the range
	   @param count number of values to generate
	   @param min minimum potentially generated value
	   @param max maximum potentially generated value

	   Writes \p count random integer values to a destination range.
	 */
	template<typename OutputIterator, typename T>
	void fill_n_int(OutputIterator first, const size_t count, const T min, const T max)
	{
		RandomEngine eng = default_engine();
		std::uniform_int_distribution<T> dist(min, max);

		std::generate_n(first, count, [&dist, &eng]()
		{
			return dist(eng);
		});
	};

	/**
	   @tparam OutputIterator must meet the requirements of LegacyOutputIterator
	   @tparam T the type of numbers generated
	   @param first iterator pointing to the first element of the range
	   @param count number of values to generate
	   @param min minimum potentially generated value
	   @param max maximum potentially generated value

	   Writes \p count random floating-point values to a destination range.
	 */
	template<typename OutputIterator, typename T>
	void fill_n_real(OutputIterator first, const size_t count, const T min, const T max)
	{
		RandomEngine eng = default_engine();
		std::uniform_real_distribution<T> dist(min, max);

		std::generate_n(first, count, [&dist, &eng]()
		{
			return dist(eng);
		});
	};

	/**
	   @tparam OutputIterator must meet the requirements of LegacyOutputIterator
	   @tparam T the type of numbers generated
	   @param first iterator pointing to the first element of the range
	   @param count number of values to generate
	   @param min minimum potentially generated value
	   @param max maximum potentially generated value

	   Writes a distinct collection of \p count random integer values to a destination range.

	   Throws std::invalid_argument or std::overflow_error if the specified range is invalid.
	 */
	template<typename OutputIterator, typename T>
	void fill_distinct_n_int(OutputIterator first, const size_t count, const T min, const T max)
	{
		if(count > 0)
		{
			if(min >= max)
			{
				throw std::invalid_argument("Invalid interval endpoints.");
			}

			// difference may overflow if min is less than zero:
			if(std::numeric_limits<T>::is_signed
			   && min < 0
			   && max > std::numeric_limits<T>::max() + min)
			{
				throw std::overflow_error("Interval endpoints out of range.");
			}

			const T distance = max - min;

			if(static_cast<typename std::make_unsigned<T>::type>(distance) < count - 1) // interval is closed
			{
				throw std::invalid_argument("Number range too small.");
			}

			RandomEngine eng = default_engine();
			std::uniform_int_distribution<T> dist(min, max);
			std::vector<T> numbers;

			while(numbers.size() != count)
			{
				int n = dist(eng);

				if(std::find(begin(numbers), end(numbers), n) == end(numbers))
				{
					numbers.push_back(n);
				}
			}

			std::copy(begin(numbers), end(numbers), first);
		}
	};

	/*! @} */
}

#endif

