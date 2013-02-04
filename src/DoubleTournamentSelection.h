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
	   @tparam T Datatype of genes stored in the Genome.
	   @tparam Q number of enemies
	   @brief Implementation of double-tournament selection.
	 */
	template<class T, uint32_t Q = 5>
	class DoubleTournamentSelection : AIndexSelection<T>
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			DoubleTournamentSelection(ARandomNumberGenerator* rnd_generator)
				: AIndexSelection<T>(rnd_generator) {}

			virtual ~DoubleTournamentSelection() {};

			void select(const std::vector<AGenome<T>*>& population, const uint32_t count, std::vector<uint32_t>& selection)
			{
				std::multiset<struct individual, struct compare_individuals> individuals;
				uint32_t i;
				uint32_t j;
				struct individual ind;

				assert(population.size() > Q);

				for(i = 0; i < population.size(); ++i)
				{
					ind.index = i;
					ind.score = 0;

					for(j = 0; j < Q; ++j)
					{
						if(population.at(i) > population.at(AIndexSelection<T>::generator->get_number(0, population.size() - 1)))
						{
							++ind.score;
						}
					}

					individuals.insert(ind);
				}

				i = 0;

				for(auto ind : individuals)
				{
					if(i++ == count)
					{
						break;
					}

					selection.push_back(ind.index);
				}
			}

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
