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
   @file OnePointCrossover.h
   @brief Implementation of the one-point crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef ONEPOINTCROSSOVER_H
#define ONEPOINTCROSSOVER_H

#include <cassert>
#include <memory>
#include <random>
#include <limits>
#include "ACrossover.h"
#include "ARandomNumberGenerator.h"
#include "TR1UniformDistribution.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Crossover
		@{
	 */

	/**
	   @class OnePointCrossover
	   @tparam TGenomeBase a genome base class
	   @brief Implementation of the one-point crossover operator.
	 */
	template<typename TGenomeBase>
	class OnePointCrossover : ea::ACrossover<TGenomeBase>
	{
		public:
			OnePointCrossover()
			{
				_rnd = std::make_shared<TR1UniformDistribution<std::mt19937_64>>();
			}

			/**
			   @param rnd instance of a random number generator
			 */
			OnePointCrossover(std::shared_ptr<ARandomNumberGenerator> rnd) : _rnd(rnd) {}

			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			uint32_t crossover(const sequence_type& a, const sequence_type& b, ea::IOutputAdapter<sequence_type>& output) override
			{
				uint32_t separator;

				assert(_base.len(a) >= 4);
				assert(_base.len(a) < std::numeric_limits<int32_t>::max());

				separator = _rnd->get_int32(1, _base.len(a) - 3);

				output.push(create_child(b, a, separator));
				output.push(create_child(a, b, separator));

				return 2;
			}

		private:
			static TGenomeBase _base;
			std::shared_ptr<ARandomNumberGenerator> _rnd;

			sequence_type create_child(const sequence_type& a, const sequence_type& b , const uint32_t separator)
			{
				uint32_t i;
				sequence_type individual;

				individual = _base.create(_base.len(a));

				for(i = 0; i < separator; i++)
				{
					_base.set(individual, i, _base.get(a, i));
				}

				for(i = separator; i < _base.len(b); i++)
				{
					_base.set(individual, i, _base.get(b, i));
				}

				return individual;
			}
	};

	template<typename TGenomeBase>
	TGenomeBase OnePointCrossover<TGenomeBase>::_base;

	/**
		   @}
	   @}
	 */
}
#endif
