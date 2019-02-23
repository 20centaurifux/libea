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
   @file CycleCrossover.hpp
   @brief Identifies cycles between parent chromosomes & copies them to offsprings.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_CYCLE_CROSSOVER_HPP
#define EA_CYCLE_CROSSOVER_HPP

#include <iterator>
#include <vector>
#include <set>
#include <algorithm>
#include <stdexcept>

namespace ea::crossover
{
	/**
	   @addtogroup Crossover
	   @{
	 */

	/**
	   @class Cycle
	   @tparam Chromosome must meet the requirements of LegacyRandomAccessIterator
	   @brief Identifies cycles between parent chromosomes & copies them to two offsprings.
	 */
	template<typename Chromosome>
	class Cycle
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

			   Throws std::length_error or std::logic_error if the genes of the parent
			   chromosomes aren't the same.
			 */
			template<typename InputIterator, typename OutputIterator>
			size_t operator()(InputIterator first1,
			                  InputIterator last1,
			                  InputIterator first2,
			                  InputIterator last2,
			                  OutputIterator result)
			{
				using difference_type = typename std::iterator_traits<InputIterator>::difference_type;

				const difference_type length = std::distance(first1, last1);

				if(length != std::distance(first2, last2))
				{
					throw std::length_error("Chromosome lengths have to be equal.");
				}

				using Genes = std::set<typename std::iterator_traits<InputIterator>::value_type>;

				Genes assigned;
				difference_type start = 0;
				Chromosome offsprings[2] = { Chromosome(length), Chromosome(length) };
				int cycle = 0;

				while(assigned.size() != static_cast<typename std::make_unsigned<difference_type>::type>(length))
				{
					while(std::find(begin(assigned), end(assigned), *(first1 + start)) != end(assigned))
					{
						++start;
					}

					difference_type offset1 = start;

					while(std::find(begin(assigned), end(assigned), *(first1 + offset1)) == end(assigned))
					{
						offsprings[cycle][offset1] = *(first1 + offset1);

						assigned.insert(*(first1 + offset1));

						auto match = std::find(first2, last2, *(first1 + offset1));

						if(match == last2)
						{
							throw std::logic_error("Chromosomes aren't equal.");
						}

						difference_type offset2 = std::distance(first2, match);

						offsprings[!cycle][offset2] = *(first2 + offset2);

						auto next = std::find(first1, last1, *(first2 + offset1));

						offset1 = std::distance(first1, next);
					}

					cycle = !cycle;
				}

				std::move(begin(offsprings), end(offsprings), result);

				return 2;
			}
	};

	/*! @} */
}

#endif

