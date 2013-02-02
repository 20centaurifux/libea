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
   @file MersenneTwisterUniformIntDistribution.h
   @brief Random number generator based on c++11 uniform distribution and mersenne twister engine.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef MERSENNETWISTERUNIFORMINTDISTRIBUTION_H
#define MERSENNETWISTERUNIFORMINTDISTRIBUTION_H

#include <random>
#include <functional>
#include <ctime>
#include <assert.h>

#include "ARandomNumberGenerator.h"

using namespace std;

namespace ea
{	/**
	   @addtogroup Core
	   @{
	   	@addtogroup Random
	   	@{
	 */

	/**
	   @class MersenneTwisterUniformIntDistribution
	   @brief Random number generator based on c++11 uniform distribution and mersenne twister engine.
	  */
	class MersenneTwisterUniformIntDistribution : public ARandomNumberGenerator
	{
		public:
			MersenneTwisterUniformIntDistribution()
			{
				_engine.seed(time(nullptr));
			}

			int32_t get_number(const int32_t min, const int32_t max)
			{
				static int32_t last_min = 0;
				static int32_t last_max = 0;
				static std::uniform_int_distribution<int> distribution;

				if(last_min != min || last_max != max)
				{
					assert(min < max);
					distribution = std::uniform_int_distribution<int>(min, max);
				}

				return distribution(_engine);
			}

			int32_t random()
			{
				static std::uniform_int_distribution<int> distribution;

				return distribution(_engine);
			}

			int32_t get_max() const
			{
				return _engine.max();
			}

		private:
			std::mt19937 _engine; 
	};

	/**
	   	@}
	   @}
	 */
}
#endif
