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
   @file AMutation.hpp
   @brief Base class for mutation operators.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef AMUTATION_H
#define AMUTATION_H

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Mutation
		@{
	 */

	/**
	   @class AMutation
	   @tparam TGenomeBase a genome base class
	   @brief Abstract base class for mutation operators.
	 */
	template<class TGenomeBase>
	class AMutation
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			virtual ~AMutation() {};

			/**
			   @param sequence sequence to mutate
			 
			   Mutates a sequence.
			 */
			virtual void mutate(sequence_type& sequence) = 0;

			/**
			   @param sequence sequence to copy genes from
			   @return a mutated copy of the specified sequence

			   Creates a copy of a sequence & mutates it.
			 */
			sequence_type create_child(const sequence_type& sequence)
			{
				static TGenomeBase base;

				sequence_type m = base.copy(sequence);
				mutate(m);

				return m;
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
