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
   @file EdgeRecombinationCrossover.hpp
   @brief Cuts two chromosomes and links both substrings.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
*/
#ifndef EA_ONEPOINT_CROSSOVER_HPP
#define EA_ONEPOINT_CROSSOVER_HPP

#include <iterator>
#include <set>
#include <map>
#include <tuple>
#include <algorithm>
#include <stdexcept>

#include "Random.hpp"
#include "Utils.hpp"

namespace ea::crossover
{
	/**
	   @addtogroup Crossover
	   @{
	*/

	/**
	   @class OnePoint
	   @tparam Chromosome must meet the requirements of LegacyRandomAccessIterator
	   @brief Cuts two chromosomes and links both substrings.
	*/
	template<typename Chromosome>
	class OnePoint
	{
		public:
			/**
			   @tparam InputIterator must meet the requirements of LegacyRandomAccessIterator
			   @tparam OutputIterator must meet the requirements of LegacyOutputIterator
			   @param first1 points to the first element of the first chromosome
			   @param last1 points to the end of the first chromosome
			   @param first2 points to the first element of the second chromosome
			   @param last2 points to the end of the second chromosome
			   @param result beginning of the destination range
			   @return number of offsprings written to \p result

			   Combines two parents and generates a two offsprings.

			   Throws std::length_error if at least a single chromosome is empty.
			*/
			template<typename InputIterator, typename OutputIterator>
			size_t operator()(InputIterator first1,
					  InputIterator last1,
					  InputIterator first2,
					  InputIterator last2,
					  OutputIterator result)
			{
				const auto length1 = std::distance(first1, last1);
				const auto length2 = std::distance(first2, last2);
				const auto min = std::min(length1, length2);

				if (min <= 0)
				{
					throw std::length_error("Chromosome is empty.");
				}

				std::uniform_int_distribution<typename std::remove_const<decltype(min)>::type> dist(0, min - 1);
				random::RandomEngine eng = random::default_engine();
				auto const separator = dist(eng);

				Chromosome offspring1(length2);

				std::copy_n(first1, separator, begin(offspring1));
				std::copy(first2 + separator, last2, begin(offspring1) + separator);

				*result++ = offspring1;

				Chromosome offspring2(length1);

				std::copy_n(first2, separator, begin(offspring2));
				std::copy(first1 + separator, last1, begin(offspring2) + separator);

				*result++ = offspring2;

				return 2;
			}
	};

	/*! @} */
}

#endif

