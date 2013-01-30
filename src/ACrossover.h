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
   @file ACrossover.h
   @brief Crossover operator base class.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef ACROSSOVER_H
#define ACROSSOVER_H

#include <vector>

#include "ACrossover.h"
#include "ARandomNumberGenerator.h"

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
	   @tparam T Datatype of genes stored in the Genome.
	   @brief Abstract base class for all crossover operators.
	 */
	template<class T>
	class ACrossover
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			ACrossover(ARandomNumberGenerator* rnd_generator) : generator(rnd_generator) {}

			virtual ~ACrossover() {};

			/**
			   @param a a genome
			   @param b a genome
			   @param children reference to a vector for storing generated child genomes
			   @return number of generated children
			   
			   Combine two genomes.
			 */
			virtual uint32_t crossover(const AGenome<T>* a, const AGenome<T>* b, std::vector<AGenome<T>*>& children) = 0;

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
