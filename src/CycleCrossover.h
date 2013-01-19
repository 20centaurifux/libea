/***************************************************************************
    begin........: December 2012
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
 * \file CycleCrossover.h
 * \brief Implementation of cycle crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef CYCLECROSSOVER_H
#define CYCLECROSSOVER_H

#include "ACrossover.h"
#include "ARandomNumberGenerator.h"

namespace ea
{
	class CycleCrossover : public ACrossover
	{
		public:
			CycleCrossover(ARandomNumberGenerator* rnd_generator) : ACrossover(rnd_generator) {}
			virtual ~CycleCrossover() {};
			uint32_t crossover(const ea::Individual* a, const ea::Individual* b, std::vector<Individual*>& children);

		private:
			inline Gene* next_gene(const ea::Individual *a, const ea::Individual *b, uint32_t& index)
			{
				if(a->find_gene(b->gene_at(index), index))
				{
					return a->gene_at(index);
				}

				return NULL;
			}

			inline bool gene_assigned(std::vector<std::vector<ea::Gene*>*> cycles, const ea::Gene *gene)
			{
				// search for given gene in all cycles:
				for(std::vector<std::vector<Gene*>*>::iterator iter = cycles.begin(); iter != cycles.end(); iter++)
				{
					if(IEquatable<Gene>::find(**iter, gene) != -1)
					{
						return true;
					}
				}

				return false;
			}
	};
}
#endif
