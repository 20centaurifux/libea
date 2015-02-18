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
   @file DoubleTournamentSelection.hpp
   @brief Implementation of the double-tournament selection.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef DOUBLETOURNAMENTSELECTION_H
#define DOUBLETOURNAMENTSELECTION_H

#include <assert.h>
#include <set>
#include <memory>
#include <limits>
#include "IIndexSelection.hpp"
#include "TR1UniformDistribution.hpp"
#include "algorithms.hpp"

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
	   @tparam TGenomeBase genome base class
	   @tparam Compare a compare function
	   @tparam Q number of enemies
	   @tparam P prohability (1..100) that the fitter individual gets selected
	   @brief Implementation of double-tournament selection.
	 */
	template<typename TGenomeBase, typename Compare = std::greater<double>, const int32_t Q = 3, const int32_t P = 65>
	class DoubleTournamentSelection : public IIndexSelection<TGenomeBase>
	{
		public:
			/**
			   @param rnd instance of a random number generator
			 */
			DoubleTournamentSelection(std::shared_ptr<ARandomNumberGenerator> rnd)
			{
				assert(rnd != nullptr);
				assert(P >= 1 && P <= 100);
				assert(Q >= 1);
				assert(Q < std::numeric_limits<int32_t>::max());
			}

			DoubleTournamentSelection()
			{
				_rnd = std::make_shared<TR1UniformDistribution<>>();

				assert(P >= 1 && P <= 100);
				assert(Q >= 1);
				assert(Q < std::numeric_limits<int32_t>::max());
			}

			~DoubleTournamentSelection() {}

			void select(IInputAdapter<typename TGenomeBase::sequence_type>& input, const size_t count, IOutputAdapter<size_t>& output) override
			{
				std::multiset<_Individual, _CompareIndividuals> individuals;
				size_t i;
				uint32_t j;
				_Individual individual;
				static Compare compare;
				int32_t* challengers;
				int32_t enemies[Q];
				int32_t prohability[Q];

				assert(input.size() > 1);
				assert(input.size() < (size_t)std::numeric_limits<int32_t>::max());
				assert(count < (size_t)std::numeric_limits<int32_t>::max());

				challengers = new int32_t[count];
				_rnd->get_int32_seq(0, input.size() - 1, challengers, count);

				for(i = 0; i < count; ++i)
				{
					individual.index = challengers[i];
					individual.score = 0;

					_rnd->get_int32_seq(0, input.size() - 1, enemies, Q);
					_rnd->get_int32_seq(1, 100, prohability, Q);

					float f0 = _base.fitness(input.at(challengers[i]));
					ASSERT_FP_NORMALITY(f0);

					for(j = 0; j < Q; ++j)
					{
						float f1 = _base.fitness(input.at(enemies[j]));
						ASSERT_FP_NORMALITY(f1);

						if(prohability[j] <= P && compare(f0, f1))
						{
							++individual.score;
						}
					}

					individuals.insert(individual);
				}

				std::for_each(begin(individuals), end(individuals), [&output](const _Individual& individual)
				{
					output.push(individual.index);
				});

				delete[] challengers;
			}

		private:
			static TGenomeBase _base;
			std::shared_ptr<ARandomNumberGenerator> _rnd;

			typedef struct
			{
				size_t index;
				uint32_t score;
			} _Individual;

			typedef struct
			{
				bool operator()(const _Individual& a, const _Individual& b) const
				{
					return a.score > b.score;
				}
			} _CompareIndividuals;
	};

	template<typename TGenomeBase, typename Compare, const int32_t Q, const int32_t P>
	TGenomeBase DoubleTournamentSelection<TGenomeBase, Compare, Q, P>::_base;

	/**
		   @}
	   @}
	 */
}
#endif
