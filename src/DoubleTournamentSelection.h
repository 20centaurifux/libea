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
   @file DoubleTournamentSelection.h
   @brief Implementation of double-tournament selection.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef DOUBLETOURNAMENTSELECTION_H
#define DOUBLETOURNAMENTSELECTION_H

#include <set>
#include <assert.h>

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
	   @class DoubleTournamentSelection
	   @tparam T datatype of genes stored in the genome.
	   @tparam Q number of enemies
	   @brief Implementation of double-tournament selection.
	 */
	template<class T, uint32_t Q = 5>
	class DoubleTournamentSelection : AIndexSelection<T>
	{
		public:
			using AIndexSelection<T>::select;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			DoubleTournamentSelection(ARandomNumberGenerator* rnd_generator)
				: AIndexSelection<T>(rnd_generator) {}

			virtual ~DoubleTournamentSelection() {};

			void select(IIterator<AGenome<T>*> *iter, const uint32_t count, std::vector<uint32_t>& selection)
			{
				std::multiset<struct individual, struct compare_individuals> individuals;
				uint32_t i;
				uint32_t j;
				struct individual individual;

				assert(iter->size() > Q);

				for(i = 0; i < iter->size(); ++i)
				{
					individual.index = i;
					individual.score = 0;

					for(j = 0; j < Q; ++j)
					{
						if(iter->at(i)->fitness() > iter->at(generator->get_number(0, iter->size() - 1))->fitness())
						{
							++individual.score;
						}
					}

					individuals.insert(individual);
				}

				auto it = individuals.begin();

				for(i = 0; i < count; ++i, ++it)
				{
					selection.push_back(it->index);
				}
			}

		protected:
			using AIndexSelection<T>::generator;

		private:
			struct individual
			{
				uint32_t index;
				uint32_t score;
			};

			struct compare_individuals
			{
				bool operator()(const struct individual& a, const struct individual& b) const
				{
					return a.score > b.score;
				}
			};
	};

	/**
		   @}
	   @}
	 */
}
#endif
