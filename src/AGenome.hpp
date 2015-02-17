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
   @file AGenome.hpp
   @brief Genome base class.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
   @version 0.1.0
*/
#ifndef AGENOME_H
#define AGENOME_H

#include <stdint.h>
#include <string>
#include <algorithm>
#include <limits>
#include "AHash.hpp"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	*/

	/**
	   @tparam TBase a genome base class
	   @tparam TIterator an iterator
	   @param base genome base class
	   @param first head of a list of sequences to dispose
	   @param last tail of a list of sequences to dispose
	   @brief Disposes all genomes.
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
	   @tparam TBase a genome base class
	   @param base genome base class
	   @param seqs list of sequences to dispose
	   @brief Disposes all genomes.
	 */
	template<typename TBase>
	void dispose(TBase& base, std::initializer_list<typename TBase::sequence_type> seqs)
	{
		for(auto seq : seqs)
		{
			base.dispose(seq);
		}
	}

	/*! Type of sequence length. */
	typedef uint16_t sequence_len_t;

	/*! Maximium sequence length. */
	#define MAX_SEQUENCE_LEN std::numeric_limits<uint16_t>::max()

	/*! Type of genome size. */
	typedef uint16_t gene_size_t;

	/**
	   @class AGenomeBase
	   @tparam TSequence the genome base class provides access to sequences of this type
	   @tparam TGene datatype of genes
	   @brief In libea individuals are stored in sequences. Any operation
	          on sequences is provided by a class derived from AGenomeBase.
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
			virtual gene_size_t gene_size() const = 0;

			/**
			   @param len length of the new sequence
			   @return a new sequence

			   Creates a new and empty sequence.
			 */
			virtual TSequence create(const sequence_len_t len) = 0;

			/**
			   @param sequence sequence to destroy

			   Destroys a sequence.
			 */
			virtual void dispose(TSequence& sequence) = 0;

			/**
			   @param sequence a sequence
			   @param offset position of a gene
			   @param gene gene to set

			   Writes a gene to a sequence.
			 */
			virtual void set(TSequence& sequence, const sequence_len_t offset, const TGene& gene) const = 0;

			/**
			   @param sequence a sequence
			   @param offset position of the gene to get
			   @return a gene

			   Reads a gene from a sequence.
			 */
			virtual TGene get(const TSequence& sequence, const sequence_len_t offset) const = 0;

			/**
			   @param sequence a sequence
			   @return length of the sequence

			   Gets the length of a sequence.
			 */
			virtual sequence_len_t len(const TSequence& sequence) const = 0;

			/**
			   @param sequence a sequence
			   @return fitness of the sequence

			   Calculates the fitness of a sequence.
			 */

			virtual float fitness(const TSequence& sequence) = 0;

			/**
			   @param sequence a sequence
			   @return hash of the sequence

			   Gets the hash of a sequence.
			 */
			virtual hash_t hash(const TSequence& sequence) = 0;

			/**
			   @param a a sequence
			   @param b another sequence
			   @return This method returns zero if the two sequences are identical, otherwise
			           it returns the difference between the first two differing genes (if both
			           sequences have the same length). Zero-length sequences are always equal.
				   If a's length is less than b's the result is -1, otherwise 1.

			   Compares two sequences.
			 */
			virtual int cmp(const TSequence& a, const TSequence& b) const = 0;

			/**
			   @param seq a sequence
			   @param search a gene to search
			   @return index of the found gene or -1

			   Searches for a gene in a sequence.
			 */
			virtual int32_t index_of(const TSequence& seq, const TGene& search) const = 0;

			/**
			   @param sequence a sequence
			   @return a new sequence

			   Copies an existing sequence.
			 */
			virtual TSequence copy(const TSequence& sequence)
			{
				TSequence dst;
				auto l = len(sequence);

				dst = create(l);

				for(auto i = 0; i < l; ++i)
				{
					set(dst, i, get(sequence, i));
				}

				return dst;
			}
	};

	/**
	   @}
	 */
}
#endif
