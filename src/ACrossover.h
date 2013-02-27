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

#include "VectorAdapter.h"
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
			   @param inserter an inserter to append generated children
			   @return number of generated children
			   
			   Combine two genomes.
			 */
			virtual uint32_t crossover(const AGenome<T>* a, const AGenome<T>* b, IInserter<AGenome<T>*>* inserter) = 0;

			/**
			   @param a a genome
			   @param b a genome
			   @param children vector to store generated children
			   @tparam Vector type of the children vector
			   @return number of generated children
			   
			   Combine two genomes.
			 */
			template<class Vector>
			uint32_t crossover(const AGenome<T>* a, const AGenome<T>* b, Vector& children)
			{
				IInserter<AGenome<T>*>* adapter = new VectorAdapter<Vector, AGenome<T>*>(children);
				uint32_t result;

				result = crossover(a, b, adapter);

				delete adapter;

				return result;
			}

			/**
			   @param begin begin of a population
			   @param end end of a population
			   @param children vector to store generated children
			   @tparam Iterator type of the iterator
			   @tparam Vector type of the children vector
			   @return number of generated children
			   
			   Combine two genomes.
			 */
			template<class Iterator, class Vector>
			uint32_t multi_crossover(const Iterator& begin, const Iterator& end, Vector& children)
			{
				uint32_t size = end - begin;
				uint32_t result = 0;

				for(uint32_t i = 0; i < size - 1; ++i)
				{
					for(uint32_t j = i + 1; j < size; ++j)
					{
						result += crossover(*(begin + i), *(begin + j), children);
					}
				}

				return result;
			}

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
