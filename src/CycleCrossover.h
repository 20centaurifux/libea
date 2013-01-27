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

#include <algorithm>
#include <functional>
#include "ACrossover.h"
#include "foreach.h"

namespace ea
{
	template<class T, class Equals = std::equal_to<T> >
	class CycleCrossover : public ACrossover<T>
	{
		public:
			CycleCrossover(ARandomNumberGenerator* rnd_generator) : ACrossover<T>(rnd_generator) {}
			virtual ~CycleCrossover() {};

			uint32_t crossover(const AGenome<T>* a, const AGenome<T>* b, std::vector<AGenome<T>*>& children)
			{
				uint32_t index = 0;
				uint32_t offset = 1;
				T gene;
				std::vector<T> *cycle;
				int32_t count = 1;
				std::vector<std::vector<T>*> cycles;
				bool flag = true;
				AGenome<T>* child1;
				AGenome<T>* child2;
				AGenome<T>* p1;
				AGenome<T>* p2;

				// create initial cycle:
				cycles.push_back((cycle = new std::vector<T>()));

				// insert first gene of individual "a" into initial cycle:
				cycle->push_back(a->at(0));

				// find next gene:
				next_gene(a, b, index, gene);

				// check if we have inserted all exisiting genes:
				while(count != (int32_t)a->size())
				{
					// has current gene already been assigned?
					while(!gene_assigned(cycles, gene))
					{
						// insert current gene into current cycle & update counter:
						cycle->push_back(gene);
						count++;

						// find next gene:
						next_gene(a, b, index, gene);
					}

					// check if we have inserted all exisiting genes:
					if(count != (int32_t)a->size())
					{
						// find next unassigned gene:
						while(offset < a->size() - 1 && gene_assigned(cycles, a->at(offset)))
						{
							offset++;
						}
			
						gene = a->at((index = offset++));

						// create new cycle:
						cycles.push_back((cycle = new std::vector<T>()));
					}
				}

				// create children:
				child1 = a->instance();
				child2 = a->instance();

				for(auto iter = cycles.begin(); iter != cycles.end(); ++iter)
				{
					if(flag)
					{
						p1 = child2;
						p2 = child1;
					}
					else
					{
						p1 = child1;
						p2 = child2;
					}

					for(uint32_t m = 0; m < a->size(); m++)
					{
						if(contains(*iter, a->at(m)))
						{
							p1->copy_to(m, a->at(m));
						}

						if(contains(*iter, b->at(m)))
						{
							p2->copy_to(m, b->at(m));
						}
					}

					flag = !flag;

				}

				children.push_back(child1);
				children.push_back(child2);

				// free memory:
				std::for_each(cycles.begin(), cycles.end(), [] (std::vector<T>* cycle) { delete cycle; });

				return 2;
			}

		private:
			Equals _equals;

			inline bool next_gene(const AGenome<T> *a, const AGenome<T> *b, uint32_t& index, T& gene)
			{
				if(a->find(b->at(index), index))
				{
					gene = a->at(index);
					return true;
				}

				return false;
			}

			bool gene_assigned(std::vector<std::vector<T>*> cycles, const T gene)
			{
				// search for given gene in all cycles:
				for(auto cycle = cycles.begin(); cycle != cycles.end(); ++cycle)
				{
					if(contains(*cycle, gene))
					{
						return true;
					}
				}

				return false;
			}

			inline bool contains(typename std::vector<T>* cycle, const T gene)
			{
				return std::search_n(cycle->begin(), cycle->end(), 1, gene, _equals) != cycle->end();
			}
	};
}
#endif
