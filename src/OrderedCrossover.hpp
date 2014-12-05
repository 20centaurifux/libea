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
   @file OrderedCrossover.hpp
   @brief Implementation of the ordered crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef ORDEREDCROSSOVER_H
#define ORDEREDCROSSOVER_H

#include <cassert>
#include <memory>
#include <random>
#include <limits>
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
	   @class OrderedCrossover
	   @tparam TGenomeBase a genome base class
	   @tparam LessThan optional functor to test if a gene is smaller than another one
	   @brief Implementation of the ordered crossover operator.
	 */
	template<typename TGenomeBase, typename LessThan = std::less<typename TGenomeBase::gene_type>>
	class OrderedCrossover : public ea::ACrossover<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			/*! Gene datatype. */
			typedef typename TGenomeBase::gene_type gene_type;

			OrderedCrossover()
			{
				_rnd = std::make_shared<TR1UniformDistribution<>>();
			}

			/**
			   @param rnd instance of a random number generator
			 */
			OrderedCrossover(std::shared_ptr<ARandomNumberGenerator> rnd) : _rnd(rnd) {}

			uint32_t crossover(const sequence_type& a, const sequence_type& b, ea::IOutputAdapter<sequence_type>& output) override
			{
				uint32_t len;
				uint32_t separator;
				uint32_t i;
				uint32_t m = 0;

				assert(set_equals<TGenomeBase>(a, b));
				assert(_base.len(a) > 2);
				assert(_base.len(a) < std::numeric_limits<int32_t>::max());

				len = _base.len(a);
				separator = _rnd->get_int32(1, len - 2);

				auto individual = _base.create(len);

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
			static LessThan _less_than;

			std::shared_ptr<ARandomNumberGenerator> _rnd;

			inline bool gene_exists(const gene_type& gene, const sequence_type& individual, const uint32_t len)
			{
				for(uint32_t i = 0; i < len; i++)
				{
					if(!_less_than(gene, _base.get(individual, i)) && !_less_than(_base.get(individual, i), gene))
					{
						return true;
					}
				}

				return false;
			}
	};

	template<typename TGenomeBase, typename LessThan>
	TGenomeBase OrderedCrossover<TGenomeBase, LessThan>::_base;

	template<typename TGenomeBase, typename LessThan>
	LessThan OrderedCrossover<TGenomeBase, LessThan>::_less_than;

	/**
		   @}
	   @}
	 */
}
#endif
