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
   @file SingleBitStringMutation.hpp
   @brief Flips a random gene.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_SINGLE_BITSTRING_MUTATION_HPP
#define EA_SINGLE_BITSTRING_MUTATION_HPP

#include <iterator>
#include <stdexcept>

#include "Random.hpp"

namespace ea::mutation
{
	/**
	   @class SingleBitString
	   @brief Flips a random gene.
	 */
	class SingleBitString
	{
		public:
			/**
			   @tparam InputIterator must meet the requirements of LegacyInputIterator
			   @param first points to the first element of a chromosome
			   @param last points to the end of a chromosome

			   Flips a random gene.

			   Throws std::length_error if chromosome is empty.
			 */
			template<typename InputIterator>
			void operator()(InputIterator first, InputIterator last) const
			{
				const auto length = std::distance(first, last);
	
				if(length == 0)
				{
					throw std::length_error("Chromosome is empty.");
				}

				random::RandomEngine eng = random::default_engine();
				std::uniform_int_distribution<decltype(length)> dist(0, length - 1);

				const auto offset = dist(eng);

				std::advance(first, offset);

				*first = !*first;
			}
	};
}

#endif

