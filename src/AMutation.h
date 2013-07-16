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
   @file AMutation.h
   @brief Base class for mutation operators.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef AMUTATION_H
#define AMUTATION_H

#include <memory>
#include <cassert>
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
	   @tparam TGenome type of the genome class
	   @brief Abstract base class for mutation operators.
	 */
	template<class TGenome>
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
			virtual void mutate(std::shared_ptr<TGenome> genome) = 0;

			/**
			   @param src genome to copy genes from
			   @param dst genome to copy genes to & mutate

			   Creates a copy of a genome & mutates it.
			 */
			void mutate(const std::shared_ptr<TGenome> src, std::shared_ptr<TGenome> dst)
			{
				copy(src, dst);
				mutate(dst);
			}

			/**
			   @param begin begin of a range
			   @param end end of a range
			   @param prohability that an individual will be mutated
			   @tparam TIterator iterator type
			   @return number of generated children
			   
			   Mutates multiple genomes.
			 */
			template<typename TIterator>
			void mutate(const TIterator &begin, const TIterator &end, const double prohability = 0.1)
			{
				TIterator iter;
				uint32_t index = 0;
				uint32_t size;
				double* numbers;

				size = end - begin;
				numbers = new double[size];
				generator->get_double_seq(0, 1, numbers, size);

				for(iter = begin; iter != end; iter++, index++)
				{
					if(numbers[index] >= prohability)
					{
						mutate(*iter);
					}
				}

				delete[] numbers;
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
