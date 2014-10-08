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
#include "IIndexSelection.h"

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
	   @tparam TGenomeBase genome base class
	   @tparam Compare a compare function
	   @brief Selects the fittest genomes of a population.
	 */
	template<typename TGenomeBase, typename Compare = std::greater<double>>
	class FittestSelection : public IIndexSelection<TGenomeBase>
	{
		public:
			void select(IInputAdapter<typename TGenomeBase::sequence_type>& input, const uint32_t count, IOutputAdapter<uint32_t>& output)
			{
				std::multiset<_Individual, _CompareIndividuals> individuals;
				_Individual individual;
				uint32_t index = 0;

				assert(count <= input.size());

				while(!input.end())
				{
					individual.index = index++;
					individual.fitness = _base.fitness(input.current());
					individuals.insert(individual);
					input.next();
				}

				auto it = individuals.begin();

				for(uint32_t i = 0; i < count; i++, it++)
				{
					output.push(it->index);
				}
			}

			~FittestSelection() {}

		private:
			static TGenomeBase _base;

			typedef struct
			{
				uint32_t index;
				double fitness;
			} _Individual;

			typedef struct
			{
				bool operator()(const _Individual& a, const _Individual& b) const
				{
					static Compare cmp;

					return cmp(a.fitness, b.fitness);
				}
			} _CompareIndividuals;
	};

	template<typename TGenomeBase, typename Compare>
	TGenomeBase FittestSelection<TGenomeBase, Compare>::_base;
}
#endif
