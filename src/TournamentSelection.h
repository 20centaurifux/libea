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
   @file TournamentSelection.h
   @brief Implementation of the tournament selection.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef TOURNAMENTSELECTION_H
#define TOURNAMENTSELECTION_H

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
	   @class TournamentSelection
	   @tparam TGenome type of the genome class
	   @tparam Compare a compare function
	   @tparam Q number of enemies
	   @tparam P prohability (0..100) that the fitter individual gets selected
	   @brief Implementation of tournament selection.
	 */
	template<typename TGenome, typename Compare = std::greater<double>, const uint32_t Q = 3, const uint32_t P = 65>
	class TournamentSelection : public ASelection<TGenome>
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			TournamentSelection(std::shared_ptr<ARandomNumberGenerator> rnd_generator)
				: ASelection<TGenome>(rnd_generator)
			{
				assert(P >= 0);
				assert(P <= 100);
				assert(Q >= 1);
			}

			~TournamentSelection() {}

		protected:
			void select_impl(IInputAdapter<std::shared_ptr<TGenome>> &input, const uint32_t count, IOutputAdapter<std::shared_ptr<TGenome>> &output) override
			{
				uint32_t index;
				uint32_t size = 0;
				int32_t enemies[Q];
				int32_t prohability[Q];
				static Compare compare;

				assert(input.size() > Q);
				assert(input.size() > 2);

				while(size != count)
				{
					index = this->generator->get_int32(0, input.size() - 1);
					this->generator->get_int32_seq(0, input.size() - 1, enemies, Q);
					this->generator->get_int32_seq(0, 100, prohability, Q);

					for(uint32_t i = 0; i < Q; i++)
					{
						if(prohability[i] >= (int32_t)P && compare(input.at(enemies[i])->fitness(), input.at(index)->fitness()))
						{
							index = enemies[i];
						}
					}

					output.append(input.at(index));
					size++;
				}
			}
	};
}
#endif
