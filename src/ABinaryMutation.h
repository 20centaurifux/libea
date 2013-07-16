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
   @file ABinaryMutation.h
   @brief Binary mutation base class.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef ABINARYMUTATION_H
#define ABINARYMUTATION_H

#include "AMutation.h"
#include "PrimitiveGenome.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Mutation
		@{
	 */

	/**
	   @typedef ABinaryMutation
	   @brief Base class for binary mutation operators.
	 */
	typedef AMutation<PrimitiveGenome<bool>> ABinaryMutation;

	/**
		   @}
	   @}
	 */
}
#endif
