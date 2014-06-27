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
   @file DoubleTournamentSelection.h
   @brief Implementation of the double-tournament selection.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef DOUBLETOURNAMENTSELECTION_H
#define DOUBLETOURNAMENTSELECTION_H

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
	   @class DoubleTournamentSelection
	   @tparam TGenome type of the genome class
	   @tparam Compare a compare function
	   @tparam Q number of enemies
	   @tparam P prohability (1..100) that the fitter individual gets selected
	   @brief Implementation of double-tournament selection.
	 */
	template<typename TGenome, typename Compare = std::greater<double>, const uint32_t Q = 3, const uint32_t P = 65>
	class DoubleTournamentSelection : public ASelection<TGenome>
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			DoubleTournamentSelection(std::shared_ptr<ARandomNumberGenerator> rnd_generator)
				: ASelection<TGenome>(rnd_generator)
			{
				assert(P >= 0);
				assert(P <= 100);
			}

			~DoubleTournamentSelection() {}

		protected:
			using ASelection<TGenome>::generator;

			void select_impl(IInputAdapter<std::shared_ptr<TGenome>> &input, const uint32_t count, IOutputAdapter<std::shared_ptr<TGenome>> &output) override
			{
				std::multiset<struct individual, struct compare_individuals> individuals;
				uint32_t i = 0;
				uint32_t j;
				struct individual individual;
				static Compare compare;
				int32_t numbers[Q];
				int32_t prohability[Q];

				assert(input.size() > Q);

				while(!input.end())
				{
					individual.index = i;
					individual.score = 0;

					generator->get_int32_seq(0, input.size() - 1, numbers, Q);
					generator->get_int32_seq(0, 100, prohability, Q);

					for(j = 0; j < Q; j++)
					{
						if(prohability[j] >= (int32_t)P && compare(input.at(i)->fitness(), input.at(numbers[j])->fitness()))
						{
							individual.score++;
						}
					}

					individuals.insert(individual);
					input.next(), i++;
				}

				auto it = individuals.begin();

				for(i = 0; i < count; i++, it++)
				{
					output.append(input.at(it->index));
				}
			}

		private:
			/// @cond INTERNAL
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
			/// @endcond
	};
}
#endif
