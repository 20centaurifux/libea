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
 * \file OrderedCrossover.h
 * \brief Implementation of ordered crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef ORDEREDCROSSOVER_H
#define ORDEREDCROSSOVER_H

#include "ACrossover.h"
#include "ARandomNumberGenerator.h"

namespace ea
{
	class OrderedCrossover : public ACrossover
	{
		public:
			OrderedCrossover(ARandomNumberGenerator* rnd_generator) : ACrossover(rnd_generator) {}
			virtual ~OrderedCrossover() {};
			uint32_t crossover(const ea::Individual* a, const ea::Individual* b, std::vector<Individual*>& children);

		private:
			inline bool gene_exists(Gene* gene, Individual* individual, const uint32_t len)
			{
				for(uint32_t i = 0; i < len; i++)
				{
					if(gene->equals(individual->gene_at(i)))
					{
						return true;
					}
				}

				return false;
			}
	};
}
#endif

