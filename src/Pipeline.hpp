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
   @file Pipeline.hpp
   @brief Connecting processing elements into a pipeline.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef PIPELINE_H
#define PIPELINE_H

#include <memory>
#include <vector>
#include <cassert>
#include "InputAdapter.hpp"
#include "OutputAdapter.hpp"
#include "ARandomNumberGenerator.hpp"

namespace ea
{
	/**
	   @addtogroup Pipeline
	   @{
	 */

	/**
	   @class APipelineElement
	   @tparam TGenomeBase a genome base class
	   @brief A pipeline having a source and a sink.
	 */
	template<typename TGenomeBase>
	class APipelineElement
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			virtual ~APipelineElement() {}

			/**
			   @param source source of the pipeline element
			   @param sink sink of the pipeline element
			   @return number of sequences written to sink

			   Reads sequences from the source and writes process sequences to sink.
			 */
			virtual uint32_t process(IInputAdapter<sequence_type>& source, IOutputAdapter<sequence_type>& sink) = 0;
	};

	/**
	   @tparam TGenomeBase a genome base class
	   @param source a population
	   @param sink destination to write sequences to
	   @return number of written sequences

	   Processes a pipeline.
	 */
	template<typename TGenomeBase>
	uint32_t pipeline_process(IInputAdapter<typename TGenomeBase::sequence_type>& source,
	                          IOutputAdapter<typename TGenomeBase::sequence_type>& sink,
	                          std::initializer_list<APipelineElement<TGenomeBase>*>)
	{
		return 0;
	}

	/**
	 * @class ASelectionSize
	 * @tparam TGenomeBase a genome base class
	 * @return number of sequences to select
	 *
	 * Calculates the number of sequences to select from a population.
	 */
	template<typename TGenomeBase>
	class ASelectionSize
	{
		public:
			virtual uint32_t operator()(IInputAdapter<typename TGenomeBase::sequence_type>& source) = 0;
	};

	/**
	 * @class SourceDivisor
	 * @tparam TGenomeBase a genome base class
	 * @tparam N a divisor
	 * @return size of the population / divisor
	 *
	 * Calculates the number of sequences to select from a population by dividing its size and a given divisor.
	 */
	template<typename TGenomeBase, const uint32_t N = 2>
	class SourceDivisor : ASelectionSize<TGenomeBase>
	{
		public:
			SourceDivisor()
			{
				assert(N != 0);
			}

			uint32_t operator()(IInputAdapter<typename TGenomeBase::sequence_type>& source)
			{
				return source.size() / N;
			}
	};

	/**
	   @class SelectionPipelineElement
	   @tparam TGenomeBase a genome base class
	   @tparam F a functor based on ASelectionSize returning number of genomes to select
	   @brief A pipeline element wrapping a selection operator.
	 */
	template<typename TGenomeBase, typename F = SourceDivisor<TGenomeBase>>
	class SelectionPipelineElement : public APipelineElement<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			/**
			   @param selection a selection operator
			   @return a new SelectionPipelineElement

			   Creates a new SelectionPipelineElement element.
			 */
			SelectionPipelineElement(std::shared_ptr<IIndexSelection<TGenomeBase>> selection)
				: _selection(selection) {}

			/**
			   @param selection a selection operator
			   @return a new SelectionPipelineElement

			   Creates a new SelectionPipelineElement element.
			 */
			SelectionPipelineElement(IIndexSelection<TGenomeBase>* selection)
				: _selection(std::shared_ptr<IIndexSelection<TGenomeBase>>(selection)) {}

			~SelectionPipelineElement() {}

			uint32_t process(IInputAdapter<sequence_type>& source, IOutputAdapter<sequence_type>& sink)
			{
				static TGenomeBase base;
				auto count = _f(source);

				std::vector<uint32_t> indices;
				auto inserter = std::back_inserter(indices);
				ea::STLVectorAdapter<uint32_t> output(inserter);

				_selection->select(source, count, output);

				for(uint32_t i : indices)
				{
					sink.push(base.copy(source.at(i)));
				}

				return count;
			}

		private:
			std::shared_ptr<IIndexSelection<TGenomeBase>> _selection;
			F _f;
	};

	/**
	   @class CrossoverPipelineElement
	   @tparam TGenomeBase a genome base class
	   @brief A pipeline element wrapping a crossover operator.
	 */
	template<typename TGenomeBase>
	class CrossoverPipelineElement : public APipelineElement<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			/**
			   @param crossover a crossover operator
			   @return a new CrossoverPipelineElement

			   Creates a new CrossoverPipelineElement element.
			 */
			CrossoverPipelineElement(std::shared_ptr<ACrossover<TGenomeBase>> crossover)
				: _crossover(crossover) {}

			/**
			   @param crossover a crossover operator
			   @return a new CrossoverPipelineElement

			   Creates a new CrossoverPipelineElement element.
			 */
			CrossoverPipelineElement(ACrossover<TGenomeBase>* crossover)
				: _crossover(std::shared_ptr<ACrossover<TGenomeBase>>(crossover)) {}

			~CrossoverPipelineElement() {}

			uint32_t process(IInputAdapter<sequence_type>& source, IOutputAdapter<sequence_type>& sink)
			{
				uint32_t count = 0;

				for(uint32_t i = 0; i < source.size(); i++)
				{
					for(uint32_t j = 0; j < source.size(); j++)
					{
						if(i != j)
						{
							count += _crossover->crossover(source.at(i), source.at(j), sink);
						}
					}
				}

				return count;
			}

		private:
			std::shared_ptr<ACrossover<TGenomeBase>> _crossover;
	};

	/**
	   @class MutationPipelineElement
	   @tparam TGenomeBase a genome base class
	   @brief A pipeline element wrapping a mutation operator.
	 */
	template<typename TGenomeBase, const int32_t P = 30>
	class MutationPipelineElement : public APipelineElement<TGenomeBase>
	{
		public:
			/*! Datatype of sequences provided by TGenomeBase. */
			typedef typename TGenomeBase::sequence_type sequence_type;

			MutationPipelineElement(std::shared_ptr<AMutation<TGenomeBase>> mutation, std::shared_ptr<ARandomNumberGenerator> rnd)
				: _mutation(mutation), _rnd(rnd)
			{
				assert(P >= 1 && P <= 100);
			}

			MutationPipelineElement(AMutation<TGenomeBase>* mutation, std::shared_ptr<ARandomNumberGenerator> rnd)
				: _mutation(std::shared_ptr<AMutation<TGenomeBase>>(mutation)), _rnd(rnd)
			{
				assert(P >= 1 && P <= 100);
			}

			uint32_t process(IInputAdapter<sequence_type>& source, IOutputAdapter<sequence_type>& sink)
			{
				static TGenomeBase base;
				int32_t* prohabilities = new int32_t[source.size()];

				_rnd->get_int32_seq(1, 100, prohabilities, source.size());

				for(int32_t i = 0; i < source.size(); i++)
				{
					if(prohabilities[i] <= P)
					{
						sink.push(base.copy(source.at(i)));
					}
					else
					{
						sink.push(_mutation->create_child(source.at(i)));
					}
				}

				delete prohabilities;

				return source.size();
			}

		private:
			std::shared_ptr<AMutation<TGenomeBase>> _mutation;
			std::shared_ptr<ARandomNumberGenerator> _rnd;
	};

	/**
	   @}
	 */
}
#endif
