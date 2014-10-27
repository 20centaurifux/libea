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
   @file PrimitiveGenome.hpp
   @brief Default genome base classes.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
   @version 0.1.0
 */
#ifndef GENOME_H
#define GENOME_H

#include <cstring>
#include "AGenome.hpp"
#include "SDBMHash.hpp"
#include "memory.hpp"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	*/

	/**
	   @class PrimitiveGenomeHashFunc
	   @tparam TSequence sequence datatype
	   @tparam H an hash algorithm
	   @brief Default functor calculating an hash for sequences derived
	          from Sequence.
	 */
	template<typename TSequence, typename H = SDBMHash>
	class PrimitiveGenomeHashFunc
	{
		public:
			/**
			   @param sequence a sequence
			   @return hash of a sequence

			   Default functor calculating an hash for sequences derived
			   from ea::Sequence.
			 */
			size_t operator()(const TSequence* const sequence) const
			{
				assert(sequence != nullptr);

				hash_func.reset();

				for(auto i = 0; i < sequence->len; i++)
				{
					hash_func << sequence->genes[i];
				}

				return hash_func.hash();
			}

		private:
			static H hash_func;
	};

	template<typename TSequence, typename H>
	H PrimitiveGenomeHashFunc<TSequence, H>::hash_func;

	/**
	   @class PrimitiveGenomeStringHashFunc
	   @tparam TSequence sequence datatype
	   @tparam H an hash algorithm
	   @brief Functor calculating an hash for sequences derived
	          from ea::Sequence having genes of type std::string.
	 */
	template<typename TSequence, typename H = SDBMHash>
	class PrimitiveGenomeStringHashFunc
	{
		public:
			/**
			   @param sequence a sequence
			   @return hash of a sequence

			   Compares each gene with std::string::compare method.
			 */
			size_t operator()(const TSequence* const sequence) const
			{
				assert(sequence != nullptr);

				hash_func.reset();

				for(auto i = 0; i < sequence->len; i++)
				{
					hash_func << sequence->genes[i].c_str();
				}

				return hash_func.hash();
			}

		private:
			static H hash_func;
	};

	template<typename TSequence, typename H>
	H PrimitiveGenomeStringHashFunc<TSequence, H>::hash_func;

	/**
	   @struct Sequence
	   @tparam TGene gene datatype
	   @brief A sequence holding data length.
	  */
	template<typename TGene>
	struct Sequence
	{
		/*! Datatype of stored genes. */
		typedef TGene gene_type;

		/*! Number of stored genes. */
		uint16_t len;
		/*! Dynamic array holding genes. */
		TGene* genes;
	};

	/**
	   @class DirectSequenceCmp
	   @tparam TSequence sequence datatype
	   @brief Functor comparing two sequences. Genes are compared
	          using std::memcmp.
	 */
	template<typename TSequence>
	class DirectSequenceCmp
	{
		public:
			/**
			   @param a a sequence
			   @param b a sequence
			   @return This method returns zero if the two sequences are identical, otherwise
			           it returns the difference between the first two differing genes (if both
			           sequences have the same length). Zero-length sequences are always equal.
				   If a's length is less than b's the result is -1, otherwise 1.

			   Compares genes using std::memcmp.
			 */
			int operator()(TSequence* const& a, TSequence* const& b) const
			{
				assert(a != nullptr && b != nullptr);

				if(a->len < b->len)
				{
					return -1;
				}

				if(a->len > b->len)
				{
					return 1;
				}

				return std::memcmp(a->genes, b->genes, sizeof(typename TSequence::gene_type) * a->len);
			}
	};

	/**
	   @class StringSequenceCmp
	   @tparam TSequence sequence datatype
	   @brief Functor comparing two sequences. Genes are compared
	          using std::string::compare.
	 */
	template<typename TSequence>
	class StringSequenceCmp
	{
		public:
			/**
			   @param a a sequence
			   @param b a sequence
			   @return This method returns zero if the two sequences are identical, otherwise
			           it returns the difference between the first two differing genes (if both
			           sequences have the same length). Zero-length sequences are always equal.
				   If a's length is less than b's the result is -1, otherwise 1.

			   Compares genes using std::string::compare.
			 */
			int operator()(TSequence* const& a, TSequence* const& b) const
			{
				assert(a != nullptr && b != nullptr);

				if(a->len < b->len)
				{
					return -1;
				}

				if(a->len > b->len)
				{
					return 1;
				}

				for(uint32_t i = 0; i < a->len; i++)
				{
					int result = a->genes[i].compare(b->genes[i]);

					if(result)
					{
						return result;
					}
				}

				return 0;
			}
	};

	/**
	   @class PrimitiveGenomeBase
	   @tparam TGene type of genes
	   @tparam F a fitness function
	   @tparam H an hash function
	   @tparam Cmp functor to compare sequences
	   @tparam TSequence sequence type (this parameter is only used internally)
	   @brief A genome base class providing access to sequences of datatype Sequence.
	 */
	template<typename TGene,
	         typename F,
	         typename H = PrimitiveGenomeHashFunc<Sequence<TGene>>,
	         typename Cmp = DirectSequenceCmp<Sequence<TGene>>,
	         typename TSequence = Sequence<TGene>>
	class PrimitiveGenomeBase : public AGenomeBase<TSequence*, TGene>
	{
		public:
			PrimitiveGenomeBase() : allocator(StdAllocator::create_shared()) {}

			/**
			   @param allocator a memory allocator

			   Creates a new PrimitiveGenomeBase instance with an assigned allocator.
			 */
			PrimitiveGenomeBase(std::shared_ptr<IAllocator> allocator)
			{
				assert(allocator != nullptr);
				this.allocator = allocator;
			}

			virtual ~PrimitiveGenomeBase() {}

			inline uint16_t gene_size() const override { return sizeof(TSequence); }

			TSequence* create(const uint16_t len) override
			{
				auto genes = (TGene*)allocator->alloc(sizeof(typename TSequence::gene_type) * len);
				auto sequence = (TSequence*)allocator->alloc(sizeof(TSequence));

				new(genes) typename TSequence::gene_type[len];
				new(sequence) TSequence;

				//std::memset(sequence, 0, sizeof(TSequence));
				//std::memset(genes, 0, len * sizeof(typename TSequence::gene_type));

				sequence->len = len;
				sequence->genes = genes;

				return sequence;
			}

			void dispose(TSequence*& sequence) override
			{
				if(sequence == nullptr)
				{
					return;
				}

				if(sequence->genes)
				{
					allocator->free(sequence->genes);
				}

				allocator->free(sequence);

				// @TODO call destructor
			}

			virtual inline void set(TSequence*& sequence, const uint16_t offset, const typename TSequence::gene_type& gene) const override
			{
				assert(sequence != nullptr && offset < sequence->len);

				sequence->genes[offset] = gene;
			}

			typename TSequence::gene_type get(TSequence* const& sequence, const uint16_t offset) const override
			{
				assert(sequence != nullptr && offset < sequence->len);

				return sequence->genes[offset];
			}

			uint16_t len(TSequence* const& sequence) const override
			{
				return sequence->len;
			}

			virtual float fitness(TSequence* const& sequence) override
			{
				return fitness_func(sequence);
			}

			virtual size_t hash(TSequence* const& sequence) override
			{
				return hash_func(sequence);
			}

			int cmp(TSequence* const& a, TSequence* const& b) const override
			{
				static Cmp cmp;

				return cmp(a, b);
			}

			int32_t index_of(TSequence* const& seq, const typename TSequence::gene_type& search) const override
			{
				for(uint32_t i = 0; i < seq->len; i++)
				{
					if(seq->genes[i] == search)
					{
						return i;
					}
				}

				return -1;
			}

		protected:
			/*! Fitness function. */
			static F fitness_func;

			/*! Hash function. */
			static H hash_func;

			/*! An allocator. */
			std::shared_ptr<IAllocator> allocator;
	};

	template<typename TGene, typename F, typename H, typename Cmp, typename TSequence>
	F PrimitiveGenomeBase<TGene, F, H, Cmp, TSequence>::fitness_func;

	template<typename TGene, typename F, typename H, typename Cmp, typename TSequence>
	H PrimitiveGenomeBase<TGene, F, H, Cmp, TSequence>::hash_func;

	enum
	{
		/*! This flag is set when the fitness has been cached. */
		PSEQ_FLAG_FITNESS_SET = 1,
		/*! This flag is set when the hash value has been cached. */
		PSEQ_FLAG_HASH_SET = 2
	};

	/**
	   @tparam TGene gene datatype
	   @brief This sequence can additionally cache fitness and hash values.
	  */
	template<typename TGene>
	struct CachedSequence : Sequence<TGene>
	{
		/*! Flags. */
		uint8_t flags;
		/*! Cached fitness value. */
		float fitness;
		/*! Cached hash value. */
		size_t hash;
	};

	/**
	   @class CachedPrimitiveGenomeBase
	   @tparam TGene type of genes
	   @tparam F a fitness function
	   @tparam H an hash function
	   @tparam Cmp functor to compare sequences
	   @brief A genome base class providing access to sequences of datatype CachedSequence.
	 */
	template<typename TGene,
	         typename F,
	         typename H = PrimitiveGenomeHashFunc<CachedSequence<TGene>>,
	         typename Cmp = DirectSequenceCmp<CachedSequence<TGene>>>
	class CachedPrimitiveGenomeBase : public PrimitiveGenomeBase<TGene, F, H, Cmp, CachedSequence<TGene>>
	{
		public:
			virtual ~CachedPrimitiveGenomeBase() {}

			size_t hash(CachedSequence<TGene>* const& sequence) override
			{
				// test if hash has already been calculated:
				if(sequence->flags & PSEQ_FLAG_HASH_SET)
				{
					// return cached hash:
					return sequence->hash;
				}

				// calculate & cache hash:
				sequence->hash = hash_func(sequence);
				sequence->flags |= PSEQ_FLAG_HASH_SET;

				return sequence->hash;
			}

			float fitness(CachedSequence<TGene>* const& sequence) override
			{
				// test if fitness has already been calculated:
				if(sequence->flags & PSEQ_FLAG_FITNESS_SET)
				{
					// return cached fitness:
					return sequence->fitness;
				}

				// calculate & cache fitness:
				sequence->fitness = fitness_func(sequence);
				sequence->flags |= PSEQ_FLAG_FITNESS_SET;

				return sequence->fitness;
			}

			inline void set(CachedSequence<TGene>*& sequence, const uint16_t offset, const TGene& gene) const override
			{
				assert(sequence != nullptr && offset < sequence->len);

				sequence->flags = 0;
				sequence->genes[offset] = gene;
			}

		protected:
			using PrimitiveGenomeBase<TGene, F, H, Cmp, CachedSequence<TGene>>::fitness_func;
			using PrimitiveGenomeBase<TGene, F, H, Cmp, CachedSequence<TGene>>::hash_func;
	};

	template<typename TSequence>
	uint16_t sequence_len(const TSequence* seq)
	{
		assert(seq != nullptr);

		return seq->len;
	}

	template<typename TSequence>
	typename TSequence::gene_type sequence_get(const TSequence* seq, const uint16_t offset)
	{
		assert(seq != nullptr);
		assert(offset < seq->len);

		return seq->genes[offset];
	}

	/**
	   @}
	 */
}
#endif
