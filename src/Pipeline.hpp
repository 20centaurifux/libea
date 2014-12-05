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
#include "InputAdapter.hpp"
#include "OutputAdapter.hpp"

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

			~APipelineElement() {}

			/**
			   @param source source of the pipeline element
			   @param sink sink of the pipeline element
			   @return number of sequences written to sink

			   Reads sequences from the source and writes process sequences to sink.
			 */
			virtual uint32_t process(IInputAdapter<sequence_type>& source, IOutputAdapter<sequence_type>& sink) = 0;
	};

	/**
	   @class SelectionPipelineElement
	   @tparam TGenomeBase a genome base class
	   @tparam F functor returning number of sequences to select from source
	   @brief A pipeline element wrapping a selection operator.
	 */
	template<typename TGenomeBase, typename F>
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
			SelectionPipelineElement(std::shared_ptr<IIndexSelection<TGenomeBase>> selection) : _selection(selection) {}

			~SelectionPipelineElement() {}

			uint32_t process(IInputAdapter<sequence_type>& source, IOutputAdapter<sequence_type>& sink)
			{
				auto count = _f();

				std::vector<uint32_t> indices;
				auto inserter = std::back_inserter(indices);
				ea::STLVectorAdapter<uint32_t> output(inserter);

				_selection->select(source, count, output);

				for(uint32_t i : indices)
				{
					sink.push(source.at(i));
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
			CrossoverPipelineElement(std::shared_ptr<ACrossover<TGenomeBase>> crossover) : _crossover(crossover) {}

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
	   @}
	 */
}
#endif
