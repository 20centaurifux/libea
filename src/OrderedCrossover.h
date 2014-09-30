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
   @file OrderedCrossover.h
   @brief Implementation of the ordered crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef ORDEREDCROSSOVER_H
#define ORDEREDCROSSOVER_H

#include <cassert>
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
	   @class OrderedCrossover
	   @tparam TGenomeBase a genome base class
	   @tparam TRandom random number generator inherited from ARandomNumberGenerator
	   @tparam LessThan optional functor to test if a gene is smaller than another one
	   @brief Implementation of the ordered crossover operator.
	 */
	template<typename TGenomeBase, typename TRandom, typename LessThan = std::less<typename TGenomeBase::gene_type>>
	class OrderedCrossover : ea::ACrossover<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			/*! Gene datatype. */
			typedef typename TGenomeBase::gene_type gene_type;

			uint32_t crossover(const sequence_type& a, const sequence_type& b, ea::IOutputAdapter<sequence_type>& output)
			{
				assert(set_equals<TGenomeBase>(a, b));

				uint32_t len = _base.len(a);
				uint32_t separator = (uint32_t)_rnd.get_int32(1, len - 2);
				auto individual = _base.create(len);

				uint32_t i;
				uint32_t m = 0;

				for(i = 0; i < separator; i++)
				{
					_base.set(individual, i, _base.get(a, i));
				}

				for(i = separator; i < len; i++)
				{
					while(gene_exists(_base.get(b, m), individual, i))
					{
						m++;
					}

					assert(m < _base.len(b));

					_base.set(individual, i, _base.get(b, m));
				}

				output.push(individual);

				return 1;
			}

		private:
			static TGenomeBase _base;
			static TRandom _rnd;

			LessThan less_than;

			inline bool gene_exists(const gene_type& gene, const sequence_type& individual, const uint32_t len)
			{
				for(uint32_t i = 0; i < len; i++)
				{
					if(!less_than(gene, _base.get(individual, i)) && !less_than(_base.get(individual, i), gene))
					{
						return true;
					}
				}

				return false;
			}
	};

	template<typename TGenomeBase, typename TRandom, typename LessThan>
	TGenomeBase OrderedCrossover<TGenomeBase, TRandom, LessThan>::_base;

	template<typename TGenomeBase, typename TRandom, typename LessThan>
	TRandom OrderedCrossover<TGenomeBase, TRandom, LessThan>::_rnd;

	/**
		   @}
	   @}
	 */
}
#endif
