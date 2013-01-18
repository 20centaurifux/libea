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
/*!
 * \file PMXCrossover.h
 * \brief Implementation of PMX crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef PMXCROSSOVER_H
#define PMXCROSSOVER_H

#include "ACrossover.h"
#include "ARandomNumberGenerator.h"

namespace ea
{
	class PMXCrossover : public ACrossover
	{
		public:
			PMXCrossover(ARandomNumberGenerator* rnd_generator) : ACrossover(rnd_generator) {}
			virtual ~PMXCrossover() {};
			uint32_t crossover(const ea::Individual* a, const ea::Individual* b, std::vector<Individual*>& children);

		private:
			ea::Individual* crossover(const ea::Individual* parent1, const ea::Individual* parent2, const uint32_t offset1, const uint32_t offset2) const;

			void next_index(const ea::Individual* individual, int32_t& index) const
			{
				while(individual->gene_at(index))
				{
					++index;
				}
			}
	};
}
#endif
