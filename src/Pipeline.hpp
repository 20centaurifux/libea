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
#include <ctime>
#include <chrono>
#include "InputAdapter.hpp"
#include "OutputAdapter.hpp"
#include "ARandomNumberGenerator.hpp"

namespace ea
{
	namespace Pipeline
	{
		/**
		   @addtogroup Pipeline
		   @{
		 */

		/**
		   @class IElement
		   @tparam TGenomeBase a genome base class
		   @brief A pipeline having a source and a sink.
		 */
		template<typename TGenomeBase>
		class IElement
		{
			public:
				/*! Datatype of sequences provided by TGenomeBase. */
				typedef typename TGenomeBase::sequence_type sequence_type;

				virtual ~IElement() {}

				/**
				   @param source source of the pipeline element
				   @param sink sink of the pipeline element
				   @return number of sequences written to sink

				   Reads sequences from the source and writes process sequences to sink.
				 */
				virtual uint32_t process(IInputAdapter<sequence_type>& source, IOutputAdapter<sequence_type>& sink) = 0;
		};

		/**
		   @class ITerminator
		   @tparam TGenomeBase a genome base class
		   @brief Functor executed after each pipeline iteration to test termination.
		 */
		template<typename TGenomeBase>
		class ITerminator
		{
			public:
				virtual ~ITerminator() {}

				/**
				   @param step iteration step
				   @param population the current generation
				   @return true to stop pipeline processing

				   Returns true if pipeline processing should be terminated.
				  */
				virtual bool operator()(const uint32_t step,
						       IInputAdapter<typename TGenomeBase::sequence_type>& population) = 0;
		};

		/**
		   @class ForLoopTerminator
		   @tparam TGenomeBase a genome base class
		   @brief Functor terminating pipeline processing after a given number of iterations.
		 */
		template<typename TGenomeBase>
		class ForLoopTerminator : public ITerminator<TGenomeBase>
		{
			public:
				/**
				   @param condition number of iterations before terminating pipeline processing
				  */
				ForLoopTerminator(const uint32_t condition) : _condition(condition)
				{
					assert(condition > 0);
				}

				virtual ~ForLoopTerminator() {}

				inline bool operator()(const uint32_t step, IInputAdapter<typename TGenomeBase::sequence_type>& population) override
				{
					return step == _condition;
				}

			private:
				uint32_t _condition;
		};

		/**
		   @class TimeBasedTerminator
		   @tparam TGenomeBase a genome base class
		   @brief Functor terminating pipeline processing after a specified time interval.
		 */
		template<typename TGenomeBase>
		class TimeBasedTerminator : public ITerminator<TGenomeBase>
		{
			public:
				/**
				   @param seconds seconds until pipeline processing should be terminated
				  */
				TimeBasedTerminator(const double seconds) : _seconds(seconds) {}

				virtual ~TimeBasedTerminator() {}

				inline bool operator()(const uint32_t step, IInputAdapter<typename TGenomeBase::sequence_type>& population) override
				{
					if(step == 1)
					{
						_start = std::chrono::system_clock::now();
					}

					return exceeded();
				}

			private:
				double _seconds;
				std::chrono::time_point<std::chrono::system_clock> _start;

				bool exceeded()
				{
 					std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - _start;

					return elapsed.count() >= _seconds;
				}
		};

		/**
		   @tparam TGenomeBase a genome base class
		   @param source a population
		   @param sink destination to write sequences to
		   @param terminator condition to check after each iteration
		   @return number of written sequences

		   Processes a pipeline.
		 */
		template<typename TGenomeBase>
		uint32_t pipeline_process(IInputAdapter<typename TGenomeBase::sequence_type>& source,
					  IOutputAdapter<typename TGenomeBase::sequence_type>& sink,
					  std::initializer_list<IElement<TGenomeBase>*> elements,
					  ITerminator<TGenomeBase>& terminator)
		{
			TGenomeBase base;
			uint32_t step = 0;
			uint32_t i;
			uint32_t size = 0;

			std::vector<typename TGenomeBase::sequence_type> vec_a;
			std::vector<typename TGenomeBase::sequence_type>* ptr_vec_a = &vec_a;
			std::vector<typename TGenomeBase::sequence_type> vec_b;
			std::vector<typename TGenomeBase::sequence_type>* ptr_vec_b = &vec_b;

			source.first();

			// copy initial population:
			for(i = 0; i < source.size(); ++i)
			{
				vec_a.push_back(base.copy(source.at(i)));
			}

			while(1)
			{
				for(auto el : elements)
				{
					auto in = make_input_adapter(*ptr_vec_a);
					auto out = make_output_adapter(*ptr_vec_b);

					size = el->process(in, out);

					auto ptr_vec = ptr_vec_a;
					ptr_vec_a = ptr_vec_b;
					ptr_vec_b = ptr_vec;

					dispose(base, begin(*ptr_vec_b), end(*ptr_vec_b));
					ptr_vec_b->clear();
				}

				auto in = make_input_adapter(*ptr_vec_a);

				if(terminator(++step, in))
				{
					// copy last generation:
					for(auto seq : *ptr_vec_a)
					{
						sink.push(seq);
					}

					break;
				}
			}

			return size;
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
				/**
				 * @param source source population
				 * @return number of sequences to select
				 */
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

				uint32_t operator()(IInputAdapter<typename TGenomeBase::sequence_type>& source) override
				{
					return source.size() / N;
				}
		};

		/**
		 * @class FixedSelectionSize
		 * @tparam TGenomeBase a genome base class
		 * @tparam N number of sequences to select
		 * @return N
		 *
		 * Returns a fixed number of sequences to select.
		 */
		template<typename TGenomeBase, const uint32_t N>
		class FixedSelectionSize : ASelectionSize<TGenomeBase>
		{
			public:
				uint32_t operator()(IInputAdapter<typename TGenomeBase::sequence_type>& source) override
				{
					return N;
				}
		};


		/**
		   @class SelectionElement
		   @tparam TGenomeBase a genome base class
		   @tparam F a functor based on ASelectionSize returning number of genomes to select
		   @brief A pipeline element wrapping a selection operator.
		 */
		template<typename TGenomeBase, typename F = SourceDivisor<TGenomeBase>>
		class SelectionElement : public IElement<TGenomeBase>
		{
			public:
				/*! Datatype of sequences provided by TGenomeBase. */
				typedef typename TGenomeBase::sequence_type sequence_type;

				/**
				   @param selection a selection operator
				   @return a new SelectionElement

				   Creates a new SelectionElement element.
				 */
				SelectionElement(std::shared_ptr<IIndexSelection<TGenomeBase>> selection)
					: _selection(selection) {}

				/**
				   @param selection a selection operator
				   @return a new SelectionElement

				   Creates a new SelectionElement element.
				 */
				SelectionElement(IIndexSelection<TGenomeBase>* selection)
					: _selection(std::shared_ptr<IIndexSelection<TGenomeBase>>(selection)) {}

				~SelectionElement() {}

				uint32_t process(IInputAdapter<sequence_type>& source, IOutputAdapter<sequence_type>& sink)
				{
					static TGenomeBase base;
					auto count = _f(source);

					std::vector<uint32_t> indices;
					auto output = make_output_adapter(indices);

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
		   @class CrossoverElement
		   @tparam TGenomeBase a genome base class
		   @brief A pipeline element wrapping a crossover operator.
		 */
		template<typename TGenomeBase>
		class CrossoverElement : public IElement<TGenomeBase>
		{
			public:
				/*! Datatype of sequences provided by TGenomeBase. */
				typedef typename TGenomeBase::sequence_type sequence_type;

				/**
				   @param crossover a crossover operator
				   @return a new CrossoverElement

				   Creates a new CrossoverElement element.
				 */
				CrossoverElement(std::shared_ptr<ACrossover<TGenomeBase>> crossover)
					: _crossover(crossover) {}

				/**
				   @param crossover a crossover operator
				   @return a new CrossoverElement

				   Creates a new CrossoverElement element.
				 */
				CrossoverElement(ACrossover<TGenomeBase>* crossover)
					: _crossover(std::shared_ptr<ACrossover<TGenomeBase>>(crossover)) {}

				~CrossoverElement() {}

				uint32_t process(IInputAdapter<sequence_type>& source, IOutputAdapter<sequence_type>& sink)
				{
					uint32_t count = 0;

					for(uint32_t i = 0; i < source.size(); ++i)
					{
						for(uint32_t j = 0; j < source.size(); ++j)
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
		   @class MutationElement
		   @tparam TGenomeBase a genome base class
		   @brief A pipeline element wrapping a mutation operator.
		 */
		template<typename TGenomeBase, const int32_t P = 30>
		class MutationElement : public IElement<TGenomeBase>
		{
			public:
				/*! Datatype of sequences provided by TGenomeBase. */
				typedef typename TGenomeBase::sequence_type sequence_type;

				/**
				   @param mutation a mutation operator
				   @param rnd a random number generator
				 */
				MutationElement(std::shared_ptr<AMutation<TGenomeBase>> mutation, std::shared_ptr<ARandomNumberGenerator> rnd)
					: _mutation(mutation), _rnd(rnd)
				{
					assert(P >= 1 && P <= 100);
				}

				/**
				   @param mutation a new mutation operator
				   @param rnd a random number generator
				 */
				MutationElement(AMutation<TGenomeBase>* mutation, std::shared_ptr<ARandomNumberGenerator> rnd)
					: _mutation(std::shared_ptr<AMutation<TGenomeBase>>(mutation)), _rnd(rnd)
				{
					assert(P >= 1 && P <= 100);
				}

				uint32_t process(IInputAdapter<sequence_type>& source, IOutputAdapter<sequence_type>& sink)
				{
					static TGenomeBase base;
					int32_t* prohabilities = new int32_t[source.size()];

					_rnd->get_int32_seq(1, 100, prohabilities, source.size());

					for(int32_t i = 0; i < source.size(); ++i)
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

					delete[] prohabilities;

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
}
#endif
