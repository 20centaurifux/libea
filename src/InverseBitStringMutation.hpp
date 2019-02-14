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
   @file InverseBitStringMutation.hpp
   @brief Mutation operator flipping all genes.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_INVERSE_BITSTRING_MUTATION_HPP
#define EA_INVERSE_BITSTRING_MUTATION_HPP

namespace ea::mutation
{
	/**
	   @addtogroup Mutation
	   @{
	 */

	/**
	   @class SingleSwap
	   @brief Flips all genes.
	 */
	class InverseBitString
	{
		public:
			/**
			   @tparam InputIterator must meet the requirements of LegacyInputIterator
			   @param first points to the first element of a chromosome
			   @param last points to the end of a chromosome

			   Flips all genes.
			 */
			template<typename InputIterator>
			void operator()(InputIterator first, InputIterator last) const
			{
				for(auto c = first; c != last; ++c)
				{
					*c = !*c;
				}
			}
	};

	/*! @} */
}

#endif

