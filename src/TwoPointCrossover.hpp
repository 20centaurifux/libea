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
   @file TwoPointCrossover.hpp
   @brief Cuts two chromosomes into three parts and links the substrings.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_TWOPOINT_CROSSOVER_HPP
#define EA_TWOPOINT_CROSSOVER_HPP

#include <iterator>
#include <tuple>
#include <algorithm>
#include <stdexcept>

#include "Random.hpp"

namespace ea::crossover
{
	/**
	   @class TwoPoint
	   @tparam Chromosome must meet the requirements of LegacyRandomAccessIterator
	   @brief Cuts two chromosomes into three parts and links the substrings.
	*/
	template<typename Chromosome>
	class TwoPoint
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

			   Combines two parents and generates two offsprings.

			   Throws std::length_error if the length of at least one chromosome is less
			   than three.
			*/
			template<typename InputIterator, typename OutputIterator>
			size_t operator()(InputIterator first1,
			                  InputIterator last1,
			                  InputIterator first2,
			                  InputIterator last2,
			                  OutputIterator result) const
			{
				const auto length1 = std::distance(first1, last1);
				const auto length2 = std::distance(first2, last2);

				append(first1, last1, length1, first2, last2, length2, result);
				append(first2, last2, length2, first1, last1, length1, result);

				return 2;
			}

		private:
			template<typename InputIterator, typename Distance, typename OutputIterator>
			static void append(InputIterator first1,
			                   InputIterator last1,
			                   const Distance length1,
			                   InputIterator first2,
			                   InputIterator last2,
			                   const Distance length2,
			                   OutputIterator result)
			{
				const auto min = std::min(length1, length2);

				if (min < 3)
				{
					throw std::length_error("Chromosome too short.");
				}

				random::RandomEngine eng = random::default_engine();
				std::uniform_int_distribution<typename std::remove_const<decltype(min)>::type> dist1(0, min - 2);
				const auto separator1 = dist1(eng);

				std::uniform_int_distribution<typename std::remove_const<decltype(min)>::type> dist2(separator1 + 1, length2 - 1);
				const auto separator2 = dist2(eng);

				Chromosome offspring(length2);

				std::copy_n(first1, separator1, begin(offspring));
				std::copy_n(first2 + separator1, separator2 - separator1, begin(offspring) + separator1);
				std::copy_n(first1 + separator2, length2  - separator2, begin(offspring) + separator2);

				*result++ = offspring;
			}
	};
}

#endif

