/***************************************************************************
    begin........: November 2012
    copyright....: Sebastian Fedrau
    email........: lord-kefir@arcor.de
 ***************************************************************************/

/***************************************************************************
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.
 ***************************************************************************/
/**
   @file StochasticUniversalSampling.h
   @brief Implementation of stochastic universal sampling selection operator.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef STOCHASTICUNIVERSALSAMPLING_H
#define STOCHASTICUNIVERSALSAMPLING_H

#include "AIndexSelection.h"

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
	   @tparam T Datatype of genes stored in the Genome.
	   @brief Implementation of stochastic universal sampling selection operator.
	 */
	template<class T>
	class StochasticUniversalSampling : AIndexSelection<T>
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			StochasticUniversalSampling(ARandomNumberGenerator* rnd_generator)
				: AIndexSelection<T>(rnd_generator) {}

			virtual ~StochasticUniversalSampling() {};

			void select(const std::vector<AGenome<T>*>& population, const uint32_t count, std::vector<uint32_t>& selection)
			{
				float* sums = new float[population.size()];
				uint32_t i;
				uint32_t j = 0;
				uint32_t u;
				float interval;
				uint32_t range[2];

				sums[0] = population.at(0)->fitness();

				for(i = 1; i < population.size(); ++i)
				{
					sums[i] = sums[i - 1] + population.at(i)->fitness();
				}

				interval = sums[population.size() - 1] / count;
				u = AIndexSelection<T>::generator->get_number(0, interval);

				for(i = 0; i < count; ++i)
				{
					range[0] = j;
					range[1] = population.size() - 1;

					j = find_index(sums, range, u);

					selection.push_back(j);

					u += interval;
				}

				delete sums;
			}

		private:
			inline uint32_t find_index(const float* sums, uint32_t range[2], const uint32_t n) const
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

	/**
		   @}
	   @}
	 */
}
#endif
