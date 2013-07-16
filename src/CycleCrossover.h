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
   @file CycleCrossover.h
   @brief Implementation of the cycle crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef CYCLECROSSOVER_H
#define CYCLECROSSOVER_H

#include <cassert>
#include <vector>
#include <map>
#include <algorithm>
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
	   @class CycleCrossover
	   @tparam TGenome type of the genome class
	   @tparam LessThan optional functor to test if a gene is smaller than another one
	   @brief Implementation of the uniform crossover operator.
	 */
	template<typename TGenome, typename LessThan = std::less<typename TGenome::value_type>>
	class CycleCrossover : public ACrossover<TGenome>
	{
		public:
			using ACrossover<TGenome>::crossover;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			CycleCrossover(std::shared_ptr<ARandomNumberGenerator> rnd_generator) : ACrossover<TGenome>(rnd_generator) {}
			virtual ~CycleCrossover() {};

		protected:
			uint32_t crossover_impl(const std::shared_ptr<TGenome> &a, const std::shared_ptr<TGenome> &b, IOutputAdapter<std::shared_ptr<TGenome>> &output) override
			{
				std::vector<T>* cycle;
				std::vector<std::vector<T>*> cycles;
				uint32_t index = 0;
				T gene;
				uint32_t count = 1;
				uint32_t offset = 1;
				std::shared_ptr<TGenome> child0;
				std::shared_ptr<TGenome> child1;
				std::map<T, bool, LessThan> assigned;
				bool flag = true;

				assert(a->size() >= 2);
				assert(a->size() == b->size());

				// initialize map to mark genes assigned:
				for(uint32_t i = 0; i < a->size(); i++)
				{
					assigned[a->at(i)] = false;
				}

				// create initial cycle:
				cycle = new std::vector<T>();
				cycles.push_back(cycle);

				// insert first gene of the first individual into the initial cycle:
				cycle->push_back(a->at(0));
				assigned[a->at(0)] = true;

				// find next gene:
				next_gene(a, b, index, gene);

				// check if we have inserted all exisiting genes:
				while(count != a->size())
				{
					// has current gene already been assigned?
					while(!assigned[gene])
					{
						// insert current gene into current cycle & update counter:
						cycle->push_back(gene), count++;
						assigned[gene] = true;

						// find next gene:
						next_gene(a, b, index, gene);
					}

					// check if we have inserted all exisiting genes:
					if(count != a->size())
					{
						// find next unassigned gene:
						while(offset < a->size() - 1 && assigned[a->at(offset)])
						{
							offset++;
						}

						gene = a->at((index = offset++));

						// create new cycle:
						cycles.push_back((cycle = new std::vector<T>()));
					}
				}

				// create children:
				child0 = std::make_shared<TGenome>(a->size(), a->get_fitness_func());
				child1 = std::make_shared<TGenome>(a->size(), a->get_fitness_func());

				for(auto iter = cycles.begin(); iter != cycles.end(); iter++)
				{
					for(uint32_t m = 0; m < a->size(); m++)
					{
						if(contains(*iter, a->at(m)))
						{
							(flag ? child0 : child1)->copy_to(m, a->at(m));
						}

						if(contains(*iter, b->at(m)))
						{
							(flag ? child1 : child0)->copy_to(m, b->at(m));
						}
					}

					flag = !flag;
				}

				output.append(child0);
				output.append(child1);

				// free memory:
				std::for_each(cycles.begin(), cycles.end(), [](std::vector<T>* cycle) { delete cycle; });

				return 2;
			}

		protected:
			using ACrossover<TGenome>::generator;

		private:
			/// @cond INTERNAL
			typedef typename TGenome::value_type T;

			struct
			{
				LessThan _lessthan;

				bool operator()(const T &a, const T &b)
				{
					return !_lessthan(a, b) && !_lessthan(b, a);
				}
			} _equals;

			inline bool next_gene(const std::shared_ptr<TGenome> &a, const std::shared_ptr<TGenome> &b, uint32_t &index, T &gene)
			{
				if(a->index_of(b->at(index), index))
				{
					gene = a->at(index);

					return true;
				}

				return false;
			}

			inline bool contains(typename std::vector<T>* cycle, const T gene)
			{
				return std::search_n(cycle->begin(), cycle->end(), 1, gene, _equals) != cycle->end();
			}
			/// @endcond
	};

	/**
		   @}
	   @}
	 */
}
#endif
