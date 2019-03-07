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
   @file OrderedCrossover.hpp
   @brief Copies a swath of consecutive genes from one parent and places
          remaining genes in the child in the order in which they appear in
	  the other parent.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_ORDERED_CROSSOVER_HPP
#define EA_ORDERED_CROSSOVER_HPP

#include <iterator>
#include <tuple>
#include <stdexcept>

#include "Random.hpp"

namespace ea::crossover
{
	/**
	   @class Ordered
	   @tparam Chromosome chromosome sequence type
	   @brief Copies a swath of consecutive genes from one parent and places
	          remaining genes in the child in the order in which they appear in
	          the other parent.
	 */
	template<typename Chromosome>
	class Ordered
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
			                  OutputIterator result) const
			{
				const difference_type<InputIterator> length = std::distance(first1, last1);

				if(length != std::distance(first2, last2))
				{
					throw std::length_error("Chromosome lengths have to be equal.");
				}

				append(first1, last1, first2, last2, length, result);
				append(first2, last2, first1, last1, length, result);

				return 2;
			}

		private:
			template<typename InputIterator>
			using difference_type = typename std::iterator_traits<InputIterator>::difference_type;

			template<typename InputIterator, typename OutputIterator>
			static void append(InputIterator first1,
			                   InputIterator last1,
			                   InputIterator first2,
			                   InputIterator last2,
			                   const difference_type<InputIterator> length,
			                   OutputIterator result)
			{
				difference_type<InputIterator> from, to;

				std::tie(from, to) = get_range(length);

				Chromosome offspring(length);

				std::copy(first1 + from, first1 + to, begin(offspring) + from);

				difference_type<InputIterator> offset = 0;

				std::for_each(first2, last2, [&](const auto &g2)
				{
					if(std::none_of(first1 + from, first1 + to, [&g2](const auto &g1) { return g1 == g2; }))
					{
						if(offset == from)
						{
							offset = to;
						}

						if(offset == length)
						{
							throw std::logic_error("Chromosomes aren't equal.");
						}

						offspring[offset] = g2;

						++offset;
					}
				});

				*result++ = offspring;
			}

			template <typename Difference>
			static std::tuple<Difference, Difference> get_range(const Difference length)
			{
				Difference range[2];

				random::fill_distinct_n_int(range, 2, static_cast<Difference>(0), length - 1);

				return std::make_tuple(std::min(range[0], range[1]), std::max(range[0], range[1]));
			}
	};
}

#endif

