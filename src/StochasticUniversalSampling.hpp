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
   @file StochasticUniversalSampling.hpp
   @brief Implementation of stochastic universal sampling.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef STOCHASTICUNIVERSALSAMPLING_H
#define STOCHASTICUNIVERSALSAMPLING_H

#include <cassert>
#include <memory>
#include "IIndexSelection.hpp"
#include "ARandomNumberGenerator.hpp"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Selection
		@{
	 */

	/**
	   @class StochasticUniversalSampling
	   @tparam TGenomeBase genome base class
	   @brief Implementation of stochastic universal sampling. This operator can only be
	          used with positive fitness values.
	 */
	template<typename TGenomeBase>
	class StochasticUniversalSampling : public IIndexSelection<TGenomeBase>
	{
		public:
			/**
			   @param rnd instance of a random number generator
			 */
			StochasticUniversalSampling(std::shared_ptr<ARandomNumberGenerator> rnd)
			{
				assert(rnd != nullptr);
				_rnd = rnd;
			}

			~StochasticUniversalSampling() {}

			void select(IInputAdapter<typename TGenomeBase::sequence_type>& input, const uint32_t count, IOutputAdapter<uint32_t>& output) override
			{
				double* sums;
				uint32_t i = 0;
				uint32_t j = 0;
				double u;
				double interval;
				uint32_t range[2];

				sums = new double[input.size()];
				sums[0] = _base.fitness(input.current());

				for(i = 1; i < input.size(); i++)
				{
					sums[i] = sums[i - 1] + _base.fitness(input.at(i));
				}

				interval = sums[input.size() - 1] / count;
				u = _rnd->get_double(0, interval);

				for(i = 0; i < count; ++i)
				{
					range[0] = j;
					range[1] = input.size() - 1;

					j = find_index(sums, range, u);

					output.push(j);

					u += interval;
				}

				delete[] sums;
			}

		private:
			static TGenomeBase _base;
			std::shared_ptr<ARandomNumberGenerator> _rnd;

			inline uint32_t find_index(const double* sums, uint32_t range[2], const double n) const
			{
				uint32_t mid;

				while(range[1] - range[0] > 1)
				{
					mid = (range[1] - range[0]) / 2 + range[0];

					if(sums[mid] > n)
					{
						range[1] = mid;
					}
					else
					{
						range[0] = mid;
					}
				}

				return range[1];
			}
	};

	template<typename TGenomeBase>
	TGenomeBase StochasticUniversalSampling<TGenomeBase>::_base;

	/**
		   @}
	   @}
	 */
}
#endif
