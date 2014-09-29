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
#include "ACrossover.h"
#include "ARandomNumberGenerator.h"

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
	   @tparam TRandom random number generator inherited from ARandomNumberGenerator
	   @brief Implementation of the one-point crossover operator.
	 */
	template<typename TGenomeBase, typename TRandom>
	class OnePointCrossover : ea::ACrossover<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			uint32_t crossover(const sequence_type& a, const sequence_type& b, ea::IOutputAdapter<sequence_type>& output)
			{
				uint32_t separator;

				assert(_base.len(a) >= 4);
				separator = (uint32_t)_rnd.get_int32(1, _base.len(a) - 3);

				output.push(create_child(b, a, separator));
				output.push(create_child(a, b, separator));

				return 2;
			}

		private:
			static TGenomeBase _base;
			static TRandom _rnd;

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

	template<typename TGenomeBase, typename TRandom>
	TGenomeBase OnePointCrossover<TGenomeBase, TRandom>::_base;

	template<typename TGenomeBase, typename TRandom>
	TRandom OnePointCrossover<TGenomeBase, TRandom>::_rnd;

	/**
		   @}
	   @}
	 */
}
#endif
