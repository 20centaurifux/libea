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
   @file UniformCrossover.h
   @brief Implementation of the uniform crossover operator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef UNIFORMCROSSOVER_H
#define UNIFORMCROSSOVER_H

#include <cassert>
#include <algorithm>
#include <map>
#include <memory>
#include <random>
#include "ACrossover.h"
#include "ARandomNumberGenerator.h"
#include "TR1UniformDistribution.h"
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
	   @class UniformCrossover
	   @tparam TGenomeBase a genome base class
	   @tparam N Ratio between two parents
	   @brief Implementation of the uniform crossover operator.
	 */
	template<typename TGenomeBase, const int32_t N = 2>
	class UniformCrossover : ea::ACrossover<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			UniformCrossover()
			{
				_rnd = std::make_shared<TR1UniformDistribution<std::mt19937_64>>();
			}

			UniformCrossover(std::shared_ptr<ARandomNumberGenerator> rnd) : _rnd(rnd) {}

			uint32_t crossover(const sequence_type& a, const sequence_type& b, ea::IOutputAdapter<sequence_type>& output)
			{
				sequence_type child0;
				sequence_type child1;
				int32_t rnd;
				uint32_t len = _base.len(a);

				assert(len > N);
				assert(len == _base.len(b));

				child0 = _base.create(len);
				child1 = _base.create(len);

				for(uint32_t i = 0; i < len; i++)
				{
					do
					{
						rnd = _rnd->get_int32();
					} while(!rnd);

					if(rnd % N)
					{
						_base.set(child0, i, _base.get(a, i));
						_base.set(child1, i, _base.get(b, i));
					}
					else
					{
						_base.set(child0, i, _base.get(b, i));
						_base.set(child1, i, _base.get(a, i));
					}
				}

				output.push(child0);
				output.push(child1);

				return 2;
			}

		private:
			static TGenomeBase _base;
			std::shared_ptr<ARandomNumberGenerator> _rnd;
	};

	template<typename TGenomeBase, const int32_t N>
	TGenomeBase UniformCrossover<TGenomeBase, N>::_base;

	/**
		   @}
	   @}
	 */
}
#endif
