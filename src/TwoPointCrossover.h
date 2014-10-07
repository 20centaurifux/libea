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
   @file TwoPointCrossover.h
   @brief Implementation of the two-point crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef TWOPOINTCROSSOVER_H
#define TWOPOINTCROSSOVER_H

#include <cassert>
#include <memory>
#include <random>
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
	   @class TwoPointCrossover
	   @tparam TGenomeBase a genome base class
	   @brief Implementation of the two-point crossover operator.
	 */
	template<typename TGenomeBase>
	class TwoPointCrossover : ea::ACrossover<TGenomeBase>
	{
		public:
			TwoPointCrossover()
			{
				_rnd = std::make_shared<TR1UniformDistribution<std::mt19937_64>>();
			}

			TwoPointCrossover(std::shared_ptr<ARandomNumberGenerator> rnd) : _rnd(rnd) {}

			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			uint32_t crossover(const sequence_type& a, const sequence_type& b, ea::IOutputAdapter<sequence_type>& output)
			{
				uint32_t offset0;
				uint32_t offset1;

				assert(_base.len(a) >= 5);
				offset0 = (uint32_t)_rnd->get_int32(1, _base.len(a) - 3);

				assert(offset0 < _base.len(b) - 2);
				offset1 = (uint32_t)_rnd->get_int32(offset0 + 1, _base.len(b) - 1);

				output.push(create_child(b, a, offset0, offset1));
				output.push(create_child(a, b, offset0, offset1));

				return 2;
			}

		private:
			static TGenomeBase _base;
			std::shared_ptr<ARandomNumberGenerator> _rnd;

			sequence_type create_child(const sequence_type& a, const sequence_type& b , const uint32_t offset1, const uint32_t offset2) const
			{
				uint32_t i;
				sequence_type individual;

				individual = _base.create(_base.len(a));

				for(i = 0; i < offset1; i++)
				{
					_base.set(individual, i, _base.get(a, i));
				}

				for(i = offset1; i < offset2; i++)
				{
					_base.set(individual, i, _base.get(b, i));
				}

				for(i = offset2; i < _base.len(a); i++)
				{
					_base.set(individual, i, _base.get(a, i));
				}

				return individual;
			}
	};

	template<typename TGenomeBase>
	TGenomeBase TwoPointCrossover<TGenomeBase>::_base;

	/**
		   @}
	   @}
	 */
}
#endif
