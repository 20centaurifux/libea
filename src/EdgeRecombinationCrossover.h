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
 * \file EdgeRecombinationCrossover.h
 * \brief Implementation of edge recombination crossover operator.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef EDGERECOMBINATIONCROSSOVER_H
#define EDGERECOMBINATIONCROSSOVER_H

#include "ACrossover.h"
#include "ARandomNumberGenerator.h"

namespace ea
{
	class EdgeRecombinationCrossover : public ACrossover
	{
		public:
			EdgeRecombinationCrossover(ARandomNumberGenerator* rnd_generator) : ACrossover(rnd_generator) {}
			virtual ~EdgeRecombinationCrossover() {};
			uint32_t crossover(const ea::Individual* a, const ea::Individual* b, std::vector<Individual*>& children);

		private:
			typedef struct
			{
				Gene* genes[4];
				uint32_t count;
			} Neighbors;

			void add_neighbors(const ea::Individual* individual, const uint32_t index, Gene* neighbors[4], uint32_t& count) const;
			void add_neighbor(Gene* neighbors[4], uint32_t& count, Gene* neighbor) const;
			void remove_neighbor(Neighbors* neighbors, const Gene* gene) const;
			bool gene_exists(const Genome* genome, const uint32_t size, const Gene* gene) const;
	};
}
#endif
