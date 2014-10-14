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
   @file AFactory.h
   @brief Factory base class.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
   @version 0.1.0
*/
#ifndef AFACTORY_H
#define AFACTORY_H

#include <stdint.h>
#include "OutputAdapter.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	*/

	/**
	   @class AFactory
	   @brief Base class for factories.
	   @tparam TSequence Type of sequences this factory creates.
	 */
	template<typename TSequence>
	class AFactory
	{
		public:
			/**
			   @param count number of sequences to create
			   @param output an output operator
			 */
			void create_population(uint32_t count, IOutputAdapter<TSequence>& output)
			{
				for(uint32_t i = 0; i < count; i++)
				{
					output.push(create_sequence());
				}
			}

			/**
			   @return a new sequence

			   Creates a new sequence.
			 */
			virtual TSequence create_sequence() = 0;
	};

	/**
	   @}
	 */
}
#endif
