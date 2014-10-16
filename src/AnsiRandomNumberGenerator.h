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
#include <mutex>
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
				#ifdef THREAD_SAFE
				std::lock_guard<std::mutex> lock(_mutex);

				if(!_seeded)
				{
					srand(time(nullptr));
					_seeded = true;
				}
				#else
				srand(time(nullptr));
				#endif
			}

			~AnsiRandomNumberGenerator() {}

			int32_t get_int32() override
			{
				return rand_re();
			}

			double get_double() override
			{
				return (double)rand_re();
			}

			inline int32_t get_max_int32() const override
			{
				return RAND_MAX;
			}

			double get_max_double() const override
			{
				return (double)RAND_MAX;
			}

		private:
			#ifdef THREAD_SAFE
			static std::mutex _mutex;
			static bool _seeded;

			inline int32_t rand_re()
			{
				std::lock_guard<std::mutex> lock(_mutex);

				return rand();
			}
			#else
			inline int32_t rand_re()
			{
				return rand();
			}
			#endif
	};

	#ifdef THREAD_SAFE
	std::mutex AnsiRandomNumberGenerator::_mutex;
	bool AnsiRandomNumberGenerator::_seeded = false;
	#endif

	/**
	   	@}
	   @}
	 */
}
#endif
