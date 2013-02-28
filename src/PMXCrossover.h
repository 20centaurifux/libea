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
   @file PMXCrossover.h
   @brief Implementation of PMX crossover operator.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef PMXCROSSOVER_H
#define PMXCROSSOVER_H

#include <cstring>
#include "ACrossover.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Crossover
		@{
	 */

	/**
	   @class PMXCrossover
	   @tparam T Datatype of genes stored in the Genome.
	   @brief Implementation of the PMX crossover operator.
	 */
	template<class T>
	class PMXCrossover : public ACrossover<T>
	{
		public:
			using ACrossover<T>::crossover;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			PMXCrossover(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : ACrossover<T>(rnd_generator) {}

			virtual ~PMXCrossover() {};

			uint32_t crossover(const AGenome<T>* a, const AGenome<T>* b, IInserter<AGenome<T>*>* inserter)
			{
				uint32_t offset1;
				uint32_t offset2;
				AGenome<T>* child;

				offset1 = generator->get_number(0, a->size() - 2);
				offset2 = generator->get_number(offset1 + 1, a->size() - 1);

				child = crossover(a, b, offset1, offset2);
				inserter->insert(child);

				child = crossover(b, a, offset1, offset2);
				inserter->insert(child);

				return 2;
			}

		protected:
			using ACrossover<T>::generator;

		private:
			AGenome<T>* crossover(const AGenome<T>* parent1, const AGenome<T>* parent2, const uint32_t offset1, const uint32_t offset2) const
			{
				AGenome<T>* child;
				uint32_t i;
				uint32_t index;
				std::vector<bool> indices(parent1->size());
				std::vector<bool>::iterator first;
				std::vector<bool>::iterator last;
				std::vector<bool>::iterator iter;
				T gene;
				T gene_p1;
				T gene_p2;

				first = indices.begin();
				last = indices.end();

				std::fill(first, last, false);
				std::fill(first + offset1, first + offset2 + 1, true);

				child = parent1->instance();

				for(i = offset1; i <= offset2; ++i)
				{
					child->copy_to(i, parent1->at(i));
				}

				for(i = offset1; i <= offset2; ++i)
				{
					gene = parent2->at(i);

					if(!child->contains(gene))
					{
						gene_p2 = gene;

						while(1)
						{
							index = parent2->index_of(gene_p2);
							gene_p1 = parent1->at(index);
							index = parent2->index_of(gene_p1);

							if(index >= offset1 && index <= offset2)
							{
								gene_p2 = gene_p1;
							}
							else
							{
								child->copy_to(index, gene);
								indices[index] = true;
								break;
							}
						}
					}
				}

				index = ((iter = std::find(first, last, false))) - first, ++iter;

				for(i = 0; i < offset1; ++i)
				{
					if(!child->contains((gene = parent1->at(i))))
					{
						child->copy_to(index, gene);
						index = ((iter = std::find(iter, last, false))) - first, ++iter;
					}
				}

				for(i = offset2 + 1; i < child->size(); ++i)
				{
					if(!child->contains((gene = parent1->at(i))))
					{
						child->copy_to(index, gene);
						index = ((iter = std::find(iter, last, false))) - first, ++iter;
					}
				}

				return child;
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
