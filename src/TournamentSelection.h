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
   @file TournamentSelection.h
   @brief Implementation of tournament selection.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef TOURNAMENTSELECTION_H
#define TOURNAMENTSELECTION_H

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
	   @class TournamentSelection
	   @tparam T datatype of genes stored in the genome.
	   @tparam Q number of enemies
	   @brief Implementation of tournament selection.
	 */
	template<class T, uint32_t Q = 5>
	class TournamentSelection : AIndexSelection<T>
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			TournamentSelection(ARandomNumberGenerator* rnd_generator)
				: AIndexSelection<T>(rnd_generator) {}

			virtual ~TournamentSelection() {};

			void select(IIterator *iter, const uint32_t count, std::vector<uint32_t>& selection)
			{
				uint32_t index;
				uint32_t enemy;
				AGenome<T> *a;
				AGenome<T> *b;

				assert(iter->size() > Q);

				while(selection.size() != count)
				{
					index = generator->get_number(0, iter->size() - 1);
					iter->at(index, b);

					for(uint32_t i = 0; i < Q; ++i)
					{
						enemy = generator->get_number(0, iter->size() - 1);
						iter->at(enemy, a);

						if(a->fitness() > b->fitness())
						{
							index = enemy;
						}
					}

					selection.push_back(index);
				}
			}

		protected:
			using AIndexSelection<T>::generator;
	};

	/**
		   @}
	   @}
	 */
}
#endif
