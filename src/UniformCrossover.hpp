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
   @file UniformCrossover.hpp
   @brief Genes are independently chosen from the two parents according to
          a 50/50 distribution.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_UNIFORM_CROSSOVER_HPP
#define EA_UNIFORM_CROSSOVER_HPP

#include <iterator>
#include <algorithm>
#include <stdexcept>

#include "Random.hpp"

namespace ea::crossover
{
	/**
	   @addtogroup Crossover
	   @{
	*/

	/**
	   @class Uniform
	   @tparam Chromosome must meet the requirements of LegacyRandomAccessIterator
	   @brief Genes are independently chosen from the two parents according to
                  a 50/50 distribution.
	*/
	template<typename Chromosome>
	class Uniform
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

		   Throws std::length_error if length of both chromosomes isn't the same.
		*/
		template<typename InputIterator, typename OutputIterator>
		size_t operator()(InputIterator first1,
			InputIterator last1,
			InputIterator first2,
			InputIterator last2,
			OutputIterator result)
		{
			const auto length = std::distance(first1, last1);

			if (length != std::distance(first2, last2))
			{
				throw std::length_error("Chromosome lengths have to be equal.");
			}

			random::RandomEngine eng = random::default_engine();
			std::uniform_int_distribution<int> dist(0, 1);

			Chromosome offsprings[2] = { Chromosome(length), Chromosome(length) };

			for (auto m = 0; m < length; ++m)
			{
				const int i = dist(eng);

				offsprings[i][m] = *(first1 + m);
				offsprings[!i][m] = *(first2 + m);
			}

			std::move(std::begin(offsprings), std::end(offsprings), result);

			return 2;
		}
	};

	/*! @} */
}

#endif

