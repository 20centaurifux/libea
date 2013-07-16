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
   @file FittestSelection.h
   @brief Selects fittest genomes of a population.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef FITTESTSELECTION_H
#define FITTESTSELECTION_H

#include <functional>
#include <set>
#include <cassert>
#include "ASelection.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Selection
		@{
	 */

	/**
	   @class FittestSelection
	   @tparam TGenome type of the genome class
	   @tparam Compare a compare function 
	   @brief Selects the fittest genomes of a population.
	 */
	template<typename TGenome, typename Compare = std::greater<double>>
	class FittestSelection : public ASelection<TGenome>
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			FittestSelection(std::shared_ptr<ARandomNumberGenerator> rnd_generator)
				: ASelection<TGenome>(rnd_generator) {}

			~FittestSelection() {}

		protected:
			void select_impl(IInputAdapter<std::shared_ptr<TGenome>> &input, const uint32_t count, IOutputAdapter<std::shared_ptr<TGenome>> &output) override
			{
				std::multiset<Individual, CompareIndividuals> individuals;
				Individual individual;
				uint32_t index = 0;

				assert(count <= input.size());

				while(!input.end())
				{
					individual.index = index++;
					individual.fitness = input.current()->fitness();
					individuals.insert(individual);
					input.next();
				}

				auto it = individuals.begin();

				for(uint32_t i = 0; i < count; i++, it++)
				{
					output.append(input.at(it->index));
				}
			}

		private:
			/// @cond INTERNAL
			typedef struct
			{
				uint32_t index;
				double fitness;
			} Individual;

			typedef struct
			{
				bool operator()(const Individual& a, const Individual& b) const
				{
					static Compare cmp;

					return cmp(a.fitness, b.fitness);
				}
			} CompareIndividuals;
			/// @endcond
	};
}
#endif
