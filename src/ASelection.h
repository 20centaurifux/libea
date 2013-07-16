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

#ifndef ASELECTION_H
#define ASELECTION_H

#include <memory>
#include <vector>
#include "ARandomNumberGenerator.h"
#include "InputAdapter.h"
#include "OutputAdapter.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Selection
		@{
	 */

	/**
	   @class ASelection
	   @tparam TGenome type of the genome class
	   @brief Abstract base class for selection operators.
	 */
	template<typename TGenome> 
	class ASelection
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			ASelection(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : generator(rnd_generator) {}

			~ASelection() {}

			/**
			   @param input source to select genomes from
			   @param size number of genomes to select
			   @param output destination to write selected genomes to
			   @tparam TInputContainer type of the input container
			   @tparam TInputContainer type of the output container

			   Selects genomes from a population.
			 */
			template<typename TInputContainer, typename TOutputContainer>
			void select(TInputContainer &input, const uint32_t size, TOutputContainer &output)
			{
				auto adapter_in = make_input_adapter<typename TInputContainer::value_type>(input);
				auto adapter_out = make_output_adapter<typename TOutputContainer::value_type>(output);

				select_impl(adapter_in, size, adapter_out);
			}

		protected:
			/*! A random number generator. */
			std::shared_ptr<ARandomNumberGenerator> generator;

			/**
			   @param input adapter used for getting genomes to select
			   @param count number of genomes to select
			   @param output destination to write selected genomes to
			   
			   Combines two genomes.
			 */
			virtual void select_impl(IInputAdapter<std::shared_ptr<TGenome>> &input, const uint32_t count, IOutputAdapter<std::shared_ptr<TGenome>> &output) = 0;
	};
}
#endif
