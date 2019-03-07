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
   @file DoubleSwapMutation.hpp
   @brief Mutation operator swapping three genes.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_DOUBLE_SWAP_MUTATION_HPP
#define EA_DOUBLE_SWAP_MUTATION_HPP

#include <iterator>
#include <vector>
#include <stdexcept>

#include "Random.hpp"

namespace ea::mutation
{
	/**
	   @class DoubleSwap
	   @brief Swaps three genes.
	 */
	class DoubleSwap
	{
		public:
			/**
			   @tparam InputIterator must meet the requirements of LegacyRandomAccessIterator
			   @param first points to the first element of a chromosome
			   @param last points to the end of a chromosome

			   Swaps three genes.

			   Throws std::length_error if the chromosome contains less than three genes.
			 */
			template<typename InputIterator>
			void operator()(InputIterator first, InputIterator last) const
			{
				using difference_type = typename std::iterator_traits<InputIterator>::difference_type;

				const difference_type length = std::distance(first, last);

				if(length < 3)
				{
					throw std::length_error("Chromosome too short.");
				}

				std::vector<difference_type> indeces(3);

				random::fill_distinct_n_int(begin(indeces), 3, static_cast<difference_type>(0), length - 1);

				auto chromosome = *(first + indeces[0]);

				*(first + indeces[0]) = *(first + indeces[1]);
				*(first + indeces[1]) = *(first + indeces[2]);
				*(first + indeces[2]) = chromosome;
			}
	};
}

#endif

