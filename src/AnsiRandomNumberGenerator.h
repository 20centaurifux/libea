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
   @file AnsiRandomNumberGenerator.h
   @brief ANSI compatible random number generator.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef ANSIRANDOMNUMBERGENERATOR_H
#define ANSIRANDOMNUMBERGENERATOR_H

#include <ctime>
#include "ARandomNumberGenerator.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	   	@addtogroup Random
	   	@{
	 */

	/**
	   @class AnsiRandomNumberGenerator
	   @brief ANSI compatible random number generator.
	  */
	class AnsiRandomNumberGenerator : public ARandomNumberGenerator
	{
		public:
			using ARandomNumberGenerator::get_int32;
			using ARandomNumberGenerator::get_double;

			AnsiRandomNumberGenerator()
			{
				if(!_seeded)
				{
					srand(time(nullptr));
					_seeded = true;
				}
			}

			~AnsiRandomNumberGenerator() {}

			int32_t get_int32() override
			{
				return rand();
			}

			double get_double() override
			{
				return (double)rand();
			}

			inline int32_t get_max_int32() const override
			{
				return RAND_MAX;
			}

			inline int32_t get_min_int32() const override
			{
				return 0;
			}

			double get_max_double() const override
			{
				return (double)RAND_MAX;
			}

			double get_min_double() const override
			{
				return 0.0;
			}

		private:
			static bool _seeded;
	};

	bool AnsiRandomNumberGenerator::_seeded = false;

	/**
	   	@}
	   @}
	 */
}
#endif
