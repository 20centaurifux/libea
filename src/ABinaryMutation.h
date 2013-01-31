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
   @file AMutation.h
   @brief Base class for mutation operators.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef ABINARYMUTATION_H
#define ABINARYMUTATION_H

#include "ABinaryMutation.h"
#include "ARandomNumberGenerator.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Mutation
		@{
	 */

	/**
	   @class ABinaryMutation
	   @brief Abstract base class for all binary mutation operators.
	 */
	class ABinaryMutation
	{
		public:
			/**
			   @param rnd_generator instance of a random number generator
			 */
			ABinaryMutation(ARandomNumberGenerator* rnd_generator) : generator(rnd_generator) {}

			virtual ~ABinaryMutation() {};

			/**
			   @param genome genome to mutate

			   Mutates a genome.
			 */
			virtual void mutate(BinaryGenome* genome) = 0;

		protected:
			/*! A random number generator. */
			ARandomNumberGenerator* generator;
	};

	/**
		   @}
	   @}
	 */
}
#endif
