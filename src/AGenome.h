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
   @file AGenome.h
   @brief Genome base class.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
   @version 0.1.0
*/
#ifndef AGENOME_H
#define AGENOME_H

#include <stdint.h>
#include <string>
#include <algorithm>

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	*/

	template<typename TBase, typename TIterator>
	void dispose(TBase& base, const TIterator& first, const TIterator& last)
	{
		std::for_each(first, last, [&base](typename TBase::sequence_type seq)
		{
			base.dispose(seq);
		});
	}

	/**
	   @class AGenomeBase
	   @tparam TSequence type of the sequence this class provides access to
	   @tparam TGene type of genes stored in the sequence
	   @brief In libea individuals are stored in sequences. All operations
	          on sequences are provided by classes derived from AGenomeBase.

	  */
	template<typename TSequence, typename TGene>
	class AGenomeBase
	{
		public:
			/*! Sequence type this class can process. */
			typedef TSequence sequence_type;

			/*! Type of the genes stored in sequences of type sequence_type. */
			typedef TGene gene_type;

			virtual ~AGenomeBase() {}

			/*! Size of a gene in byte.*/
			virtual uint16_t gene_size() const = 0;

			/**
			   @param len length of the new sequence
			   @return a new sequence

			   Creates a new and empty sequence.
			 */
			virtual TSequence create(const uint16_t len) = 0;

			/**
			   @param sequence sequence to destroy

			   Destroys a sequence.
			 */
			virtual void dispose(TSequence& sequence) = 0;

			/**
			   @param sequence a sequence
			   @param offset position of a gene
			   @param gene gene to set

			   Copies a gene to the given position.
			 */
			virtual void set(TSequence& sequence, const uint16_t offset, const TGene& gene) const = 0;

			/**
			   @param sequence a sequence
			   @param offset position of a geme
			   @return a gene

			   Gets a gene.
			 */
			virtual TGene get(const TSequence& sequence, const uint16_t offset) const = 0;

			/**
			   @param sequence a sequence
			   @return length of the sequence

			   Gets the length of a sequence.
			 */

			virtual uint16_t len(const TSequence& sequence) const = 0;
			/**
			   @param sequence a sequence
			   @return fitness of the given sequence

			   Gets fitness of a sequence.
			 */

			virtual float fitness(const TSequence& sequence) = 0;

			/**
			   @param sequence a sequence
			   @return hash of the sequence

			   Gets hash of a sequence.
			 */
			virtual size_t hash(const TSequence& sequence) = 0;

			/**
			   @param a a sequence
			   @param b another sequence
			   @return This method returns zero if the two sequences are identical, otherwise
			           it returns the difference between the first two differing bytes (if both
			           sequences have the same length). Zero-length sequences are always equal.
				   If a's length is less than b's the result is -1, otherwise 1.

			   Compares two sequences.
			 */
			virtual int cmp(const TSequence& a, const TSequence& b) const = 0;
	};

	/**
	   @}
	 */
}
#endif
