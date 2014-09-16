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
   @file ACrossover.h
   @brief Crossover operator base class.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef ACROSSOVER_H
#define ACROSSOVER_H

#include "OutputAdapter.h"

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
	   @tparam TGenome type of the genome class
	   @brief Abstract base class for crossover operators.
	 */
	template<typename TGenomeBase>
	class ACrossover
	{
		public:
			typedef typename TGenomeBase::sequence_type sequence_type;

			~ACrossover() {}

			/**
			   @param a a genome
			   @param b a genome
			   @param container a container used to store the generated genomes
			   @tparam TOutputContainer type of the specified container
			   @return number of generated children

			   Combines two genomes.
			 */
			virtual uint32_t crossover(const sequence_type& a, const sequence_type& b, IOutputAdapter<sequence_type>& output) = 0;
	};
}
#endif
