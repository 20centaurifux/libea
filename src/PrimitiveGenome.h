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
   @file PrimitiveGenome.h
   @brief Default genome base classes.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
   @version 0.1.0
 */
#ifndef GENOME_H
#define GENOME_H

#include "AGenome.h"
#include "SDBMHash.h"
#include "memory.h"

namespace ea
{
	/**
	   @class PrimitiveGenomeHashFunc
	   @tparam TGene gene datatype
	   @brief Default functor calculating a hash for sequences derived
			   from Sequence.
	 */
	template<typename TSequence, typename H = SDBMHash>
	class PrimitiveGenomeHashFunc
	{
		public:
			/**
			   @param sequence a sequence
			   @return hash of a sequence

			   Default functor calculating a hash for sequences derived
			   from Sequence.
			 */
			size_t operator()(const TSequence* const sequence) const
			{
				assert(sequence != nullptr);

				hash_funcer.reset();

				for(auto i = 0; i < sequence->len; i++)
				{
					hash_funcer << sequence->genes[i];
				}

				return hash_funcer.hash();
			}

		private:
			static H hash_funcer;
	};

	template<typename TSequence, typename H>
	H PrimitiveGenomeHashFunc<TSequence, H>::hash_funcer;

	/**
	   @struct Sequence
	   @tparam TGene gene datatype
	   @brief A simple sequence holding data length.
	  */
	template<typename TGene>
	struct Sequence
	{
		/*! Datatype of the stored genes. */
		typedef TGene gene_type;

		/*! Number of stored genes. */
		uint16_t len;
		/*! Dynamic array holding genes. */
		TGene *genes;
	};

	/**
	   @class PrimitiveGenomeBase
	   @tparam TGene type of the genes
	   @tparam F a fitness function
	   @tparam H an hash function
	   @tparam TSequence sequence type (only used internally)
	   @brief A genome base class providing access to sequences of datatype Sequence.
	 */
	template<typename TGene, typename F, typename H = PrimitiveGenomeHashFunc<Sequence<TGene>>, typename TSequence = Sequence<TGene>>
	class PrimitiveGenomeBase : public AGenomeBase<TSequence*, TGene>
	{
		public:
			PrimitiveGenomeBase() : allocator(StdAllocator::create_shared()) {}

			PrimitiveGenomeBase(std::shared_ptr<IAllocator> allocator) : allocator(allocator) {}

			virtual ~PrimitiveGenomeBase() {}

			inline uint16_t gene_size() const { return sizeof(TSequence); }

			TSequence* create(const uint16_t len)
			{
				auto genes = (uint32_t*)allocator->alloc(sizeof(typename TSequence::gene_type) * len);
				auto sequence = (TSequence*)allocator->alloc(sizeof(TSequence));

				memset(sequence, 0, sizeof(TSequence));
				memset(genes, 0, len * sizeof(typename TSequence::gene_type));

				sequence->len = len;
				sequence->genes = genes;

				return sequence;
			}

			void dispose(TSequence*& sequence)
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
			}

			virtual inline void set(TSequence*& sequence, const uint16_t offset, const typename TSequence::gene_type& gene) const
			{
				assert(sequence != nullptr && offset < sequence->len);

				sequence->genes[offset] = gene;
			}

			typename TSequence::gene_type get(TSequence* const& sequence, const uint16_t offset) const
			{
				assert(sequence != nullptr && offset < sequence->len);

				return sequence->genes[offset];
			}

			uint16_t len(TSequence* const& sequence) const
			{
				return sequence->len;
			}

			virtual float fitness(TSequence* const& sequence)
			{
				return fitness_func(sequence);
			}

			virtual size_t hash(TSequence* const& sequence)
			{
				return hash_func(sequence);
			}

			int cmp(TSequence* const& a, TSequence* const& b) const
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

				return memcmp(a->genes, b->genes, sizeof(typename TSequence::gene_type) * a->len);
			}

		protected:
			/*! Fitness function. */
			static F fitness_func;
			/*! Hash function. */
			static H hash_func;
			/*! An allocator. */
			std::shared_ptr<IAllocator> allocator;
	};

	template<typename TGene, typename F, typename H, typename TSequence>
	F PrimitiveGenomeBase<TGene, F, H, TSequence>::fitness_func;

	template<typename TGene, typename F, typename H, typename TSequence>
	H PrimitiveGenomeBase<TGene, F, H, TSequence>::hash_func;

	enum
	{
		/*! This flag is set when the fitness has been cached. */
		PSEQ_FLAG_FITNESS_SET = 1,
		/*! This flag is set when the hash value has been cached. */
		PSEQ_FLAG_HASH_SET = 2
	};


	/**
	   @tparam TGene gene datatype
	   @brief This sequence can cache fitness and hash.
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
	   @tparam TGene type of the genes
	   @tparam F a fitness function
	   @tparam H an hash function
	   @brief A genome base class providing access to sequences of datatype CachedSequence.
	 */
	template<typename TGene, typename F, typename H = PrimitiveGenomeHashFunc<CachedSequence<TGene>>>
	class CachedPrimitiveGenomeBase : public PrimitiveGenomeBase<TGene, F, H, CachedSequence<TGene>>
	{
		public:
			virtual ~CachedPrimitiveGenomeBase() {}

			size_t hash(CachedSequence<TGene>* const& sequence) const
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

			float fitness(CachedSequence<TGene>* const& sequence)
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

		protected:
			using PrimitiveGenomeBase<TGene, F, H, CachedSequence<TGene>>::fitness_func;
			using PrimitiveGenomeBase<TGene, F, H, CachedSequence<TGene>>::hash_func;
	};
}
#endif
