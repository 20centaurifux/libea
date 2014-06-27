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

#include <memory>
#include <vector>
#include "ARandomNumberGenerator.h"
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
	template<typename TGenome> 
	class ACrossover
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			ACrossover(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : generator(rnd_generator) {}

			~ACrossover() {}

			/**
			   @param a a genome
			   @param b a genome
			   @param container a container used to store the generated genomes
			   @tparam TOutputContainer type of the specified container
			   @return number of generated children
			   
			   Combines two genomes.
			 */
			template<typename TOutputContainer>
			uint32_t crossover(const std::shared_ptr<TGenome> &a, const std::shared_ptr<TGenome> &b, TOutputContainer &container)
			{
				auto adapter = make_output_adapter<std::shared_ptr<TGenome>>(container);

				return crossover_impl(a, b, adapter);
			}

			/**
			   @param begin begin of a range
			   @param end end of a range
			   @param container a container used to store the generated genomes
			   @tparam TIterator iterator type
			   @tparam TOutputContainer type of the specified container
			   @return number of generated children
			   
			   Combines multiple genomes.
			 */
			template<typename TIterator, typename TOutputContainer>
			uint32_t crossover(const TIterator &begin, const TIterator &end, TOutputContainer &container)
			{
				auto adapter =  make_output_adapter<std::shared_ptr<TGenome>>(container);
				uint32_t size = end - begin;
				uint32_t result = 0;

				for(uint32_t i = 0; i < size - 1; i++)
				{
					for(uint32_t j = i + 1; j < size; j++)
					{
						result += crossover_impl(*(begin + i), *(begin + j), adapter);
					}
				}

				return result;
			}

		protected:
			/*! A random number generator. */
			std::shared_ptr<ARandomNumberGenerator> generator;

			/**
			   @param a a genome
			   @param b a genome
			   @param output an adapter wrapping a container used to store the generated genomes
			   @return number of generated children
			   
			   Combines two genomes.
			 */
			virtual uint32_t crossover_impl(const std::shared_ptr<TGenome> &a, const std::shared_ptr<TGenome> &b, IOutputAdapter<std::shared_ptr<TGenome>> &output) = 0;
	};
}
#endif
