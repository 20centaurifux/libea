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
   @file AMutation.h
   @brief Base class for mutation operators.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef AMUTATION_H
#define AMUTATION_H

#include <memory>

#include "AGenome.h"
#include "ARandomNumberGenerator.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Mutation
		@{
	 */

	/**
	   @class AMutation
	   @tparam T Datatype of genes stored in the Genome.
	   @brief Abstract base class for all mutation operators.
	 */
	template<class T>
	class AMutation
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			AMutation(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : generator(rnd_generator) {}

			virtual ~AMutation() {};

			/**
			   @param genome genome to mutate

			   Mutates a genome.
			 */
			virtual void mutate(AGenome<T>* genome) = 0;

			/**
			   @param begin begin of a range of genomes
			   @param end end of a range of genomes
			   @param rate mutation probability
			   @tparam Iterator iterator type
			   
			   Mutates genomes of a population with the given probability.
			 */
			template<class Iterator>
			void multi_mutate(const Iterator& begin, const Iterator& end, uint32_t rate = 5)
			{
				for(Iterator iter = begin; iter != end; ++iter)
				{
					if(!generator->get_number(0, rate))
					{
						mutate(*iter);
					}
				}
			}

		protected:
			/*! A random number generator. */
			std::shared_ptr<ARandomNumberGenerator> generator;
	};

	/**
		   @}
	   @}
	 */
}
#endif
