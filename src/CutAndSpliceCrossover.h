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
   @file CutAndSpliceCrossover.h
   @brief Implementation of the cut-and-splice crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef CUTANDSPLICECROSSOVER_H
#define CUTANDSPLICECROSSOVER_H

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
	   @class CutAndSpliceCrossover
	   @tparam TGenomeBase a genome base class
	   @brief Implementation of the cut-and-splice crossover operator.
	 */
	template<typename TGenomeBase>
	class CutAndSpliceCrossover : ea::ACrossover<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			CutAndSpliceCrossover()
			{
				_rnd = std::make_shared<TR1UniformDistribution<std::mt19937_64>>();
			}

			CutAndSpliceCrossover(std::shared_ptr<ARandomNumberGenerator> rnd) : _rnd(rnd) {}

			uint32_t crossover(const sequence_type& a, const sequence_type& b, ea::IOutputAdapter<sequence_type>& output)
			{
				uint32_t m, i, sep1, sep2;

				assert(_base.len(a) > 2);
				assert(_base.len(b) > 2);

				m = sep1 = (uint32_t)_rnd->get_int32(1, _base.len(a) - 2);
				sep2 = (uint32_t)_rnd->get_int32(1, _base.len(b) - 2);

				// create first individual:
				auto individual = _base.create(sep1 + _base.len(b) - sep2);

				for(i = 0; i < sep1; i++)
				{
					_base.set(individual, i, _base.get(a, i));
				}

				for(i = sep2; i < _base.len(b); i++)
				{
					_base.set(individual, m++, _base.get(b, i));
				}

				output.push(individual);

				// create second individual:
				individual = _base.create(sep2 + _base.len(a) - sep1);

				for(i = 0; i < sep2; i++)
				{
					_base.set(individual, i, _base.get(b, i));
				}

				m = sep2;

				for(i = sep1; i < _base.len(a); i++)
				{
					_base.set(individual, m++, _base.get(a, i));
				}

				output.push(individual);

				return 2;
			}

		private:
			static TGenomeBase _base;
			std::shared_ptr<ARandomNumberGenerator> _rnd;
	};

	template<typename TGenomeBase>
	TGenomeBase CutAndSpliceCrossover<TGenomeBase>::_base;

	/**
		   @}
	   @}
	 */
}
#endif
