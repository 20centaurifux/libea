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
   @file BitStringMutation.hpp
   @brief Flips at least one gene.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_BITSTRING_MUTATION_HPP
#define EA_BITSTRING_MUTATION_HPP

#include <iterator>
#include <vector>
#include <stdexcept>

#include "Random.hpp"

namespace ea::mutation
{
	/**
	   @addtogroup Mutation
	   @{
	 */

	/**
	   @class BitString
	   @brief Flips at least one gene.
	 */
	class BitString
	{
		public:
			/**
			   @param probability probability a gene is flipped

			   Throws std::invalid_argument if probability is out of range (0.0 <= p <= 1.0).
			 */
			explicit BitString(const double probability = 0.8)
				: probability(probability)
			{
				if(probability <= 0.0 || probability >= 1.0)
				{
					throw std::invalid_argument("Probability out of range.");
				}
			}

			/**
			   @tparam InputIterator must meet the requirements of LegacyInputIterator
			   @param first points to the first element of a chromosome
			   @param last points to the end of a chromosome
			 */
			template<typename InputIterator>
			void operator()(InputIterator first, InputIterator last) const
			{
				bool flipped = false;
				const auto length = std::distance(first, last);
				std::vector<double> probabilities(length);

				while(!flipped && length > 0)
				{
					random::fill_n_real(begin(probabilities), length, 0.0, 1.0);

					auto n = begin(probabilities);

					for(auto c = first; c != last; ++c, ++n)
					{
						if(*n <= probability)
						{
							*c = !*c;
							flipped = true;
						}
					}
				}
			}

		private:
			const double probability;
	};

	/*! @} */
}

#endif

