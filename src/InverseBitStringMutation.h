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
   @file InverseBitStringMutation.h
   @brief Mutation operator flipping all genes.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef INVERSEBITSTRINGMUTATION_H
#define INVERSEBITSTRINGMUTATION_H

#include "AMutation.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Mutation
		@{
	 */

	/**
	   @class InverseBitStringMutation
	   @tparam TGenomeBase a genome base class
	   @brief A mutation operator flipping all genes.
	 */
	template<class TGenomeBase>
	class InverseBitStringMutation : public AMutation<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			virtual ~InverseBitStringMutation() {}

			void mutate(sequence_type& sequence) override
			{
				static TGenomeBase base;

				for(uint32_t i = 0; i < base.len(sequence); i++)
				{
					base.set(sequence, i, !base.get(sequence, i));
				}
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
