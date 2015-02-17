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
   @file CycleCrossover.hpp
   @brief Implementation of the cycle crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef CYCLECROSSOVER_H
#define CYCLECROSSOVER_H

#include <cassert>
#include <vector>
#include <map>
#include <algorithm>
#include "ACrossover.hpp"
#include "algorithms.hpp"

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
	   @tparam TGenomeBase a genome base class
	   @tparam LessThan optional functor to test if a gene is smaller than another one
	   @brief Implementation of the cycle crossover operator.
	 */
	template<typename TGenomeBase, typename LessThan = std::less<typename TGenomeBase::gene_type>>
	class CycleCrossover : public ACrossover<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			/*! Gene datatype. */
			typedef typename TGenomeBase::gene_type gene_type;

			std::size_t crossover(const sequence_type& a, const sequence_type& b, ea::IOutputAdapter<sequence_type>& output) override
			{
				std::map<gene_type, bool, LessThan> assigned; /* dictionary used to test if a gene
				                                                 is assigned */
				std::vector<std::vector<gene_type>*> cycles;  // vector containing the generated cycles
				std::size_t count = 1;                        // number of inserted genes
				sequence_len_t len = _base.len(a);            // genome length
				sequence_len_t offset = 1;
				sequence_len_t index = 0;
				gene_type gene;
				std::vector<gene_type>* cycle;

				assert(_base.len(a) >= 2);
				assert(set_equals<TGenomeBase>(a, b));

				// initialize dictionary:
				for(sequence_len_t i = 0; i < len; ++i)
				{
					// mark all genes from individual a as not assigned:
					assigned[_base.get(a, i)] = false;
				}

				// create initial cycle:
				cycle = new std::vector<gene_type>();
				cycles.push_back(cycle);

				// insert first gene from the first individual into the initial cycle:
				cycle->push_back(_base.get(a, 0));
				assigned[_base.get(a, 0)] = true;

				// find next gene:
				next_gene(a, b, index, gene);

				// check if we have inserted all exisiting genes:
				while(count != len)
				{
					// is current gene already assigned?
					while(!assigned[gene])
					{
						// insert current gene into current cycle & update counter:
						cycle->push_back(gene), ++count;
						assigned[gene] = true;

						// find next gene:
						next_gene(a, b, index, gene);
					}

					// check if we have inserted all existing genes:
					if(count != len)
					{
						// find next unassigned gene:
						while(offset < len - 1 && assigned[_base.get(a, offset)])
						{
							++offset;
						}

						gene = _base.get(a, ((index = offset++)));

						// create new cycle:
						cycles.push_back((cycle = new std::vector<gene_type>()));
					}
				}

				// create children:
				bool flag = true;

				sequence_type child0 = _base.create(len);
				sequence_type child1 = _base.create(len);

				for(auto iter = begin(cycles); iter != end(cycles); ++iter)
				{
					for(sequence_len_t m = 0; m < len; ++m)
					{
						if(contains(*iter, _base.get(a, m)))
						{
							_base.set(flag ? child0 : child1, m, _base.get(a, m));
						}

						if(contains(*iter, _base.get(b, m)))
						{
							_base.set(flag ? child1 : child0, m, _base.get(b, m));
						}
					}

					flag = !flag;
				}

				// write created children to adapter:
				output.push(child0);
				output.push(child1);

				// cleanup:
				std::for_each(cycles.begin(), cycles.end(), [](std::vector<gene_type>* cycle)
				{
					delete cycle;
				});

				return 2;
			}

		private:
			TGenomeBase _base;

			inline bool next_gene(const sequence_type& a, const sequence_type& b, sequence_len_t& index, gene_type& gene) const
			{
				int32_t i = _base.index_of(a, _base.get(b, index));

				if(i != -1)
				{
					index = i;
					gene = _base.get(a, index);

					return true;
				}

				return false;
			}

			/// @cond INTERNAL
			struct
			{
				bool operator()(const gene_type& a, const gene_type& b)
				{
					return !_lessthan(a, b) && !_lessthan(b, a);
				}

				private:
					LessThan _lessthan;

			} _equals;
			/// @endcond

			inline bool contains(typename std::vector<gene_type>* cycle, const gene_type& gene)
			{
				return std::search_n(cycle->begin(), cycle->end(), 1, gene, _equals) != cycle->end();
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
