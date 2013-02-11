/***************************************************************************
    begin........: November 2012
    copyright....: Sebastian Fedrau
    email........: lord-kefir@arcor.de
 ***************************************************************************/

/***************************************************************************
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.
 ***************************************************************************/
/**
   @file AIndexSelection.h
   @brief Base class for selection operators.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef AINDEXSELECTION_H
#define AINDEXSELECTION_H

#include "IIterator.h"
#include "AGenome.h"
#include "ARandomNumberGenerator.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Selection
		@{
	 */

	/**
	   @class AIndexSelection
	   @tparam T datatype of genes stored in the Genome.
	   @brief Abstract base class for selection operators.
	 */
	template<class T>
	class AIndexSelection
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			AIndexSelection(ARandomNumberGenerator* rnd_generator) : generator(rnd_generator) {}

			virtual ~AIndexSelection() {};

			/**
			   @param iter range of genomes
			   @param count number of genomes to select
			   @param selection vector to store selected genomes
			   
			   Select genomes from a population.
			 */
			virtual void select(IIterator *iter, const uint32_t count, std::vector<uint32_t>& selection) = 0;

		protected:
			/*! Instance of a random number generator. */
			ARandomNumberGenerator* generator;
	};

	/**
		   @}
	   @}
	 */
}
#endif
