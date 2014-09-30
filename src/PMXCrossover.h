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
   @file PMXCrossover.h
   @brief Implementation of the two-point crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef PMXCROSSOVER_H
#define PMXCROSSOVER_H

#include <cassert>
#include <algorithm>
#include <map>
#include "ACrossover.h"
#include "ARandomNumberGenerator.h"
#include "algorithms.h"

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
	   @tparam TGenomeBase a genome base class
	   @tparam TRandom random number generator inherited from ARandomNumberGenerator
	   @tparam LessThan optional functor to test if a gene is smaller than another one
	   @brief Implementation of the two-point crossover operator.
	 */
	template<typename TGenomeBase, typename TRandom, typename LessThan = std::less<typename TGenomeBase::gene_type>>
	class PMXCrossover : ea::ACrossover<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			/*! Gene datatype. */
			typedef typename TGenomeBase::gene_type gene_type;

			uint32_t crossover(const sequence_type& a, const sequence_type& b, ea::IOutputAdapter<sequence_type>& output)
			{
				uint32_t offset0;
				uint32_t offset1;

				assert(_base.len(a) >= 4);
				assert(set_equals<TGenomeBase>(a, b));

				offset0 = _rnd.get_int32(0, _base.len(a) - 3);
				offset1 = _rnd.get_int32(offset0 + 1, _base.len(a) - 1);

				output.push(crossover(a, b, offset0, offset1));
				output.push(crossover(b, a, offset0, offset1));

				return 2;
			}

		private:
			static TGenomeBase _base;
			static TRandom _rnd;

			sequence_type crossover(const sequence_type& a, const sequence_type& b, const uint32_t offset0, const uint32_t offset1) const
			{
				bool* indices;                                // array used to store the information if an index is in use or not
				std::map<gene_type, bool, LessThan> assigned; // a cache to test if a gene is assigned to the child genome
				uint32_t size;
				uint32_t i;
				gene_type gene;
				int32_t index;

				// initialize index array:
				size = _base.len(a);
				indices = new bool[size];

				std::fill(indices, indices + size, false);

				// initialize cache:
				for(i = 0; i < size; i++)
				{
					assigned[_base.get(a, i)] = false;
				}

				// create child genome:
				sequence_type child = _base.create(size);

				// copy initial genes to child genome:
				for(i = offset0; i <= offset1; i++)
				{
					gene = _base.get(a, i);
					_base.set(child, i, gene);
					assigned[gene] = true;
				}

				for(i = offset0; i <= offset1; i++)
				{
					gene = _base.get(b, i);

					if(!assigned[gene])
					{
						auto g1 = gene;

						for(;;)
						{
							index = _base.index_of(b, g1);
							auto g0 = _base.get(a, index);
							index = _base.index_of(b, g0);

							if(index >= offset0 && index <= offset1)
							{
								g1 = g0;
							}
							else
							{
								_base.set(child, index, gene);
								assigned[gene] = indices[index] = true;
								break;
							}
						}
					}
				}

				uint32_t start = 0;
				index = next_unassigned(indices, 0, size), start++;

				for(i = 0; i < offset0; i++)
				{
					if(!assigned[(gene = _base.get(a, i))])
					{
						_base.set(child, index, gene);
						assigned[gene] = indices[index] = true;
						index = next_unassigned(indices, start++, size);
					}
				}

				for(i = offset1 + 1; i < size; i++)
				{
					if(!assigned[(gene = _base.get(a, i))])
					{
						_base.set(child, index, gene);
						assigned[gene] = indices[index] = true;
						index = next_unassigned(indices, start++, size);
					}
				}

				delete[] indices;

				return child;
			}

			inline uint32_t next_unassigned(const bool* indices, const uint32_t first, const uint32_t last) const
			{
				for(uint32_t i = first; i < last; i++)
				{
					if(!indices[i])
					{
						return i;
					}
				}

				return 0;
			}
	};

	template<typename TGenomeBase, typename TRandom, typename LessThan>
	TGenomeBase PMXCrossover<TGenomeBase, TRandom, LessThan>::_base;

	template<typename TGenomeBase, typename TRandom, typename LessThan>
	TRandom PMXCrossover<TGenomeBase, TRandom, LessThan>::_rnd;

	/**
		   @}
	   @}
	 */
}
#endif

