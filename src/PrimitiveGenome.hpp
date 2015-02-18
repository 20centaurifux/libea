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
#ifndef PRIMITIVE_GENOME_H
#define PRIMITIVE_GENOME_H

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
	   @class PGenomeHashFunc
	   @tparam TSequence sequence datatype
	   @tparam H an hash algorithm
	   @brief Default functor calculating an hash value for sequences derived
	          from Sequence.
	 */
	template<typename TSequence, typename H = SDBMHash>
	class PGenomeHashFunc
	{
		public:
			/**
			   @param sequence a sequence
			   @return hash value of a sequence

			   Default functor calculating an hash value for sequences derived
			   from ea::Sequence. It writes the data of each gene to the stream of
		           the given hash algorithm.
			 */
			hash_t operator()(const TSequence* const sequence) const
			{
				assert(sequence != nullptr);

				hash_func.reset();

				for(sequence_len_t i = 0; i < sequence->len; ++i)
				{
					hash_func << sequence->genes[i];
				}

				return hash_func.hash();
			}

		private:
			static H hash_func;
	};

	template<typename TSequence, typename H>
	H PGenomeHashFunc<TSequence, H>::hash_func;

	/**
	   @class PGenomeIteratingHashFunc
	   @tparam TSequence sequence datatype
	   @tparam H an hash algorithm
	   @brief Functor calculating an hash for sequences derived from ea::Sequence.
	          This functor is recommended if genes are enumerable. It iterates
	          each gene and writes the elements to the stream of the given hash
	          algorithm.
	 */
	template<typename TSequence, typename H = SDBMHash>
	class PGenomeIteratingHashFunc
	{
		public:
			/**
			   @param sequence a sequence
			   @return hash value of a sequence

			   Functor calculating an hash value for sequences derived from
		           ea::Sequence. It iterates all genes of the given sequence
			   and writes the data to the stream of the given hash algorithm.
			 */
			hash_t operator()(const TSequence* const sequence) const
			{
				assert(sequence != nullptr);

				hash_func.reset();

				for(sequence_len_t i = 0; i < sequence->len; ++i)
				{
					for(auto it : sequence->genes[i])
					{
						hash_func << it;
					}
				}

				return hash_func.hash();
			}

		private:
			static H hash_func;
	};

	template<typename TSequence, typename H>
	H PGenomeIteratingHashFunc<TSequence, H>::hash_func;

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
		sequence_len_t len;
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
	   @class SequenceStrCmp
	   @tparam TSequence sequence datatype
	   @brief Functor comparing two sequences. Genes are compared
	          using std::string::compare.
	 */
	template<typename TSequence>
	class SequenceStrCmp
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

				for(uint32_t i = 0; i < a->len; ++i)
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
	   @class PGenomeBase
	   @tparam TGene type of genes
	   @tparam F a fitness function
	   @tparam H an hash function
	   @tparam Cmp functor to compare sequences
	   @tparam TSequence sequence type (this parameter is only used internally)
	   @brief A genome base class providing access to sequences of datatype Sequence.
	 */
	template<typename TGene,
	         typename F,
	         typename H = PGenomeHashFunc<Sequence<TGene>>,
	         typename Cmp = DirectSequenceCmp<Sequence<TGene>>,
	         typename TSequence = Sequence<TGene>>
	class PGenomeBase : public AGenomeBase<TSequence*, TGene>
	{
		public:
			PGenomeBase() : allocator(StdAllocator::create_shared()) {}

			/**
			   @param allocator a memory allocator

			   Creates a new PGenomeBase instance with an assigned allocator.
			 */
			PGenomeBase(std::shared_ptr<IAllocator> allocator)
			{
				assert(allocator != nullptr);
				this.allocator = allocator;
			}

			virtual ~PGenomeBase() {}

			inline gene_size_t gene_size() const override { return sizeof(TSequence); }

			TSequence* create(const sequence_len_t len) override
			{
				assert(sizeof(TSequence) == gene_size());

				auto genes = (TGene*)allocator->alloc(sizeof(typename TSequence::gene_type) * len);
				auto sequence = (TSequence*)allocator->alloc(sizeof(TSequence));

				new(genes) typename TSequence::gene_type[len];
				new(sequence) TSequence;

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
					sequence->genes->~TGene();
					allocator->free(sequence->genes);
				}

				sequence->~TSequence();
				allocator->free(sequence);
			}

			virtual inline void set(TSequence*& sequence, const sequence_len_t offset, const typename TSequence::gene_type& gene) const override
			{
				assert(sequence != nullptr && offset < sequence->len);

				sequence->genes[offset] = gene;
			}

			typename TSequence::gene_type get(TSequence* const& sequence, const sequence_len_t offset) const override
			{
				assert(sequence != nullptr && offset < sequence->len);

				return sequence->genes[offset];
			}

			sequence_len_t len(TSequence* const& sequence) const override
			{
				return sequence->len;
			}

			virtual float fitness(TSequence* const& sequence) override
			{
				return fitness_func(sequence);
			}

			virtual hash_t hash(TSequence* const& sequence) override
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
				for(sequence_len_t i = 0; i < seq->len; ++i)
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
	F PGenomeBase<TGene, F, H, Cmp, TSequence>::fitness_func;

	template<typename TGene, typename F, typename H, typename Cmp, typename TSequence>
	H PGenomeBase<TGene, F, H, Cmp, TSequence>::hash_func;

	/**
	   @typedef Int32PGenomeBase
	   @tparam F a fitness function

	   ea::PGenomeBase with int32_t genes.
	  */
	template<typename F>
	using Int32PGenomeBase = PGenomeBase<int32_t, F>;

	/**
	   @typedef UInt32PGenomeBase
	   @tparam F a fitness function

	   ea::PGenomeBase with uint32_t genes.
	  */
	template<typename F>
	using UInt32PGenomeBase = PGenomeBase<uint32_t, F>;

	/**
	   @typedef BinaryPGenomeBase
	   @tparam F a fitness function

	   ea::PGenomeBase with bool genes.
	  */
	template<typename F>
	using BinaryPGenomeBase = PGenomeBase<bool, F>;

	/**
	   @typedef DoublePGenomeBase
	   @tparam F a fitness function

	   ea::PGenomeBase with double genes.
	  */
	template<typename F>
	using DoublePGenomeBase = PGenomeBase<double, F>;

	/**
	   @typedef StringPGenomeBase
	   @tparam F a fitness function

	   ea::PGenomeBase with string genes.
	  */
	template<typename F>
	using StringPGenomeBase = PGenomeBase<std::string,
	                                              F,
	                                              PGenomeIteratingHashFunc<Sequence<std::string>>,
	                                              SequenceStrCmp<Sequence<std::string>>>;

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
	struct CSequence : Sequence<TGene>
	{
		/*! Flags. */
		uint8_t flags;
		/*! Cached fitness value. */
		float fitness;
		/*! Cached hash value. */
		hash_t hash;
	};

	/**
	   @class CPGenomeBase
	   @tparam TGene type of genes
	   @tparam F a fitness function
	   @tparam H an hash function
	   @tparam Cmp functor to compare sequences
	   @brief A genome base class providing access to sequences of datatype CSequence.
	 */
	template<typename TGene,
	         typename F,
	         typename H = PGenomeHashFunc<CSequence<TGene>>,
	         typename Cmp = DirectSequenceCmp<CSequence<TGene>>>
	class CPGenomeBase : public PGenomeBase<TGene, F, H, Cmp, CSequence<TGene>>
	{
		public:
			virtual ~CPGenomeBase() {}

			hash_t hash(CSequence<TGene>* const& sequence) override
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

			float fitness(CSequence<TGene>* const& sequence) override
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

			inline void set(CSequence<TGene>*& sequence, const uint16_t offset, const TGene& gene) const override
			{
				assert(sequence != nullptr && offset < sequence->len);

				sequence->flags = 0;
				sequence->genes[offset] = gene;
			}

		protected:
			using PGenomeBase<TGene, F, H, Cmp, CSequence<TGene>>::fitness_func;
			using PGenomeBase<TGene, F, H, Cmp, CSequence<TGene>>::hash_func;
	};

	/**
	   @typedef Int32CPGenomeBase
	   @tparam F a fitness function

	   ea::PCGenomeBase with int32_t genes.
	  */
	template<typename F>
	using Int32CPGenomeBase = CPGenomeBase<int32_t, F>;

	/**
	   @typedef UInt32CPGenomeBase
	   @tparam F a fitness function

	   ea::PCGenomeBase with uint32_t genes.
	  */
	template<typename F>
	using UInt32CPGenomeBase = CPGenomeBase<uint32_t, F>;

	/**
	   @typedef BinaryCPGenomeBase
	   @tparam F a fitness function

	   ea::PCGenomeBase with bool genes.
	  */
	template<typename F>
	using BinaryCPGenomeBase = CPGenomeBase<bool, F>;

	/**
	   @typedef DoubleCPGenomeBase
	   @tparam F a fitness function

	   ea::PCGenomeBase with double genes.
	  */
	template<typename F>
	using DoubleCPGenomeBase = CPGenomeBase<double, F>;

	/**
	   @typedef StringCPGenomeBase
	   @tparam F a fitness function

	   ea::PCGenomeBase with string genes.
	  */
	template<typename F>
	using StringCPGenomeBase = CPGenomeBase<std::string,
	                                              F,
	                                              PGenomeIteratingHashFunc<Sequence<std::string>>,
	                                              SequenceStrCmp<Sequence<std::string>>>;

	/**
	   @tparam TSequence sequence datatype
	   @param seq a sequence
	   @return length of the sequence

	   Gets length of a sequence derived from ea::Sequence.
	  */
	template<typename TSequence>
	sequence_len_t sequence_len(const TSequence* seq)
	{
		assert(seq != nullptr);

		return seq->len;
	}

	/**
	   @tparam TSequence sequence datatype
	   @param seq a sequence
	   @param offset offset of the desired gene
	   @return a gene

	   Gets a gene stored in a sequence derived from ea::Sequence.
	  */
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
