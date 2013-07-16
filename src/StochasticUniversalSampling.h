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
   @file StochasticUniversalSampling.h
   @brief Implementation of stochastic universal sampling.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef STOCHASTICUNIVERSALSAMPLING_H
#define STOCHASTICUNIVERSALSAMPLING_H

#include <cassert>
#include "ASelection.h"

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
	   @brief Implementation of stochastic universal sampling. This operator can only be
	          used with positive fitness values.
	 */
	template<typename TGenome>
	class StochasticUniversalSampling : public ASelection<TGenome>
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			StochasticUniversalSampling(std::shared_ptr<ARandomNumberGenerator> rnd_generator)
				: ASelection<TGenome>(rnd_generator) {}

			~StochasticUniversalSampling() {}

		protected:
			using ASelection<TGenome>::generator;

			void select_impl(IInputAdapter<std::shared_ptr<TGenome>> &input, const uint32_t count, IOutputAdapter<std::shared_ptr<TGenome>> &output) override
			{
				double* sums;
				uint32_t i = 0;
				uint32_t j = 0;
				double u;
				double interval;
				uint32_t range[2];

				sums = new double[input.size()];
				sums[0] = input.current()->fitness();

				for(i = 1; i < input.size(); i++)
				{
					sums[i] = sums[i - 1] + input.at(i)->fitness();
				}

				interval = sums[input.size() - 1] / count;
				u = generator->get_double(0, interval);

				for(i = 0; i < count; ++i)
				{
					range[0] = j;
					range[1] = input.size() - 1;

					j = find_index(sums, range, u);

					output.append(input.at(j));

					u += interval;
				}

				delete[] sums;
			}

		private:
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
}
#endif
