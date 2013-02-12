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
   @file FittestSelection.h
   @brief Select fittest genomes of a population.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef FITTESTSELECTION_H
#define FITTESTSELECTION_H

#include <set>

#include "AIndexSelection.h"

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
	   @tparam T datatype of genes stored in the genome.
	   @brief Select fittest genomes of a population.
	 */
	template<class T>
	class FittestSelection : AIndexSelection<T>
	{
		public:
			using AIndexSelection<T>::select;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			FittestSelection(ARandomNumberGenerator* rnd_generator)
				: AIndexSelection<T>(rnd_generator) {}

			virtual ~FittestSelection() {};

			void select(IIterator<AGenome<T>*> *iter, const uint32_t count, std::vector<uint32_t>& selection)
			{
				std::multiset<struct individual, compare_individuals> individuals;
				struct individual individual;
				uint32_t index = 0;

				while(!iter->end())
				{
					individual.index = index++;
					individual.fitness = iter->current()->fitness();
					individuals.insert(individual);
					iter->next();
				}

				auto it = individuals.begin();

				for(uint32_t i = 0; i < count; ++i, ++it)
				{
					selection.push_back(it->index);
				}
			}

		private:
			struct individual
			{
				uint32_t index;
				float fitness;
			};

			struct compare_individuals
			{
				bool operator()(const struct individual& a, const struct individual& b) const
				{
					return a.fitness > b.fitness;
				}
			};

	};

	/**
		   @}
	   @}
	 */
}
#endif
