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
   @file IIndexSelection.h
   @brief Index selection interface.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef IINDEXSELECTION_H
#define IINDEXSELECTION_H

#include "InputAdapter.h"
#include "OutputAdapter.h"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Selection
		@{
	 */

	/**
	   @class IIndexxSelection
	   @tparam TGenome type of the genome class
	   @brief Abstract base class for selection operators.
	 */
	template<typename TGenomeBase>
	class IIndexxSelection
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			~IIndexxSelection() {}

			/**
			   @param input a population
			   @param count number of sequences to select
			   @param output destination to write selected sequences to

			   Combines two genomes.
			 */
			virtual void select(IInputAdapter<sequence_type>& input, const uint32_t count, IOutputAdapter<int32_t>& output) = 0;
	};
}
#endif