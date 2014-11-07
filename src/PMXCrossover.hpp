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
   @file PMXCrossover.hpp
   @brief Implementation of the PMX crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef PMXCROSSOVER_H
#define PMXCROSSOVER_H

#include <cassert>
#include <algorithm>
#include <map>
#include <memory>
#include <random>
#include "ACrossover.hpp"
#include "ARandomNumberGenerator.hpp"
#include "TR1UniformDistribution.hpp"
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
	   @class PMXCrossover
	   @tparam TGenomeBase a genome base class
	   @tparam LessThan optional functor to test if a gene is smaller than another one
	   @brief Implementation of the PMX crossover operator.
	 */
	template<typename TGenomeBase, typename LessThan = std::less<typename TGenomeBase::gene_type>>
	class PMXCrossover : ea::ACrossover<TGenomeBase>
	{
		public:
			PMXCrossover()
			{
				_rnd = std::make_shared<TR1UniformDistribution<>>();
			}

			/**
			   @param rnd instance of a random number generator
			 */
			PMXCrossover(std::shared_ptr<ARandomNumberGenerator> rnd)
			{
				assert(rnd != nullptr);
				_rnd = rnd;
			}

			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			/*! Gene datatype. */
			typedef typename TGenomeBase::gene_type gene_type;

			uint32_t crossover(const sequence_type& a, const sequence_type& b, ea::IOutputAdapter<sequence_type>& output) override
			{
				uint32_t offset0;
				uint32_t offset1;

				assert(_base.len(a) > 3);
				assert(set_equals<TGenomeBase>(a, b));

				offset0 = _rnd->get_int32(0, _base.len(a) - 3);
				offset1 = _rnd->get_int32(offset0 + 1, _base.len(a) - 1);

				output.push(crossover(a, b, offset0, offset1));
				output.push(crossover(b, a, offset0, offset1));

				return 2;
			}

		private:
			static TGenomeBase _base;
			std::shared_ptr<ARandomNumberGenerator> _rnd;

			sequence_type crossover(const sequence_type& a, const sequence_type& b, const uint32_t offset0, const uint32_t offset1) const
			{
				std::map<gene_type, bool, LessThan> assigned; // a cache to test if a gene is assigned to the child genome
				uint32_t size;
				uint32_t i;
				gene_type gene;
				int32_t index;

				// initialize index array:
				size = _base.len(a);

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
								assigned[gene] = true;
								break;
							}
						}
					}
				}

				for(i = 0; i < offset0; i++)
				{
					if(!assigned[(gene = _base.get(b, i))])
					{
						_base.set(child, i, gene);
					}
				}

				for(i = offset1 + 1; i < size; i++)
				{
					if(!assigned[(gene = _base.get(b, i))])
					{
						_base.set(child, i, gene);
					}
				}

				return child;
			}
	};

	template<typename TGenomeBase, typename LessThan>
	TGenomeBase PMXCrossover<TGenomeBase, LessThan>::_base;

	/**
		   @}
	   @}
	 */
}
#endif
