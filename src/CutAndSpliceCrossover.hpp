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
   @file CutAndSpliceCrossover.hpp
   @brief Cuts two chromosomes and links both substrings.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_CUT_AND_SPLICE_CROSSOVER_HPP
#define EA_CUT_AND_SPLICE_CROSSOVER_HPP

#include <tuple>
#include <iterator>
#include <limits>
#include <stdexcept>

#include "Random.hpp"

namespace ea::crossover
{
	/**
	   @addtogroup Crossover
	   @{
	 */

	/**
	   @class CutAndSplice
	   @tparam Chromosome chromosome sequence type
	   @brief Cuts two chromosomes and links both substrings. Two offstrings are generated.
	 */
	template<typename Chromosome>
	class CutAndSplice
	{
		public:
			/**
			   @tparam InputIterator must meet the requirements of LegacyInputIterator
			   @tparam OutputIterator must meet the requirements of LegacyOutputIterator
			   @param first1 points to the first element of the first chromosome
			   @param last1 points to the end of the first chromosome
			   @param first2 points to the first element of the second chromosome
			   @param last2 points to the end of the second chromosome
			   @param result beginning of the destination range
			   @return number of offsprings written to \p result
			   
			   Combines two parents and generates new offspring.

			   Throws std::length_error if the length of at least one chromosome is below 3
			   and std::overflow_error if an overflow occurs.
			 */
			template<typename InputIterator, typename OutputIterator>
			size_t operator()(InputIterator first1,
			                  InputIterator last1,
			                  InputIterator first2,
			                  InputIterator last2,
			                  OutputIterator result)
			{
				difference_type<InputIterator> length1, sep1;

				std::tie(length1, sep1) = separate(first1, last1);

				difference_type<InputIterator> length2, sep2;

				std::tie(length2, sep2) = separate(first2, last2);

				append(first1, sep1, first2, sep2, length2, result);
				append(first2, sep2, first1, sep1, length1, result);

				return 2;
			}

		private:
			template<typename InputIterator>
			using difference_type = typename std::iterator_traits<InputIterator>::difference_type;

			random::RandomEngine eng = random::default_engine();

			template<typename InputIterator>
			std::tuple<difference_type<InputIterator>, difference_type<InputIterator>>
			separate(InputIterator first, InputIterator last)
			{
				const difference_type<InputIterator> length = std::distance(first, last);

				if(length <= 2)
				{
					throw std::length_error("Population too small.");
				}

				std::uniform_int_distribution<difference_type<InputIterator>> dist(1, length - 2);

				return std::make_tuple(length, dist(eng));
			}

			template<typename InputIterator, typename OutputIterator>
			static void append(InputIterator first1,
			                   const difference_type<InputIterator> sep1,
			                   InputIterator first2,
			                   const difference_type<InputIterator> sep2,
			                   const difference_type<InputIterator> length2,
			                   OutputIterator result)
			{
				if(std::numeric_limits<difference_type<InputIterator>>::max() - sep1 < length2 - sep2)
				{
					throw std::overflow_error("Arithmetic overflow.");
				}

				const difference_type<InputIterator> length = sep1 + (length2 - sep2);

				Chromosome chromosome(length);

				auto iter = begin(chromosome);

				std::copy_n(first1, sep1, iter);
				std::advance(iter, sep1);

				std::advance(first2, sep2);
				std::copy_n(first2, length2 - sep2, iter);

				*result++ = chromosome;
			}
	};

	/*! @} */
}

#endif

