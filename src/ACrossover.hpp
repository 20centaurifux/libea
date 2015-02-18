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
   @file ACrossover.hpp
   @brief Crossover operator base class.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef ACROSSOVER_H
#define ACROSSOVER_H

#include <stdlib.h>
#include "OutputAdapter.hpp"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Crossover
		@{
	 */

	/**
	   @class ACrossover
	   @tparam TGenomeBase type of a genome base class
	   @brief Abstract base class for crossover operators.
	 */
	template<typename TGenomeBase>
	class ACrossover
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			virtual ~ACrossover() {}

			/**
			   @param a a sequence
			   @param b a sequence
			   @param output an output adapter
			   @return number of generated children

			   Combines two sequences.
			 */
			virtual size_t crossover(const sequence_type& a, const sequence_type& b, IOutputAdapter<sequence_type>& output) = 0;
	};

	/**
		   @}
	   @}
	 */
}
#endif
