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
   @file Stream.hpp
   @brief Combines evolutionary operators in a functional-style.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
*/
#ifndef EA_STREAM_HPP
#define EA_STREAM_HPP

#include <algorithm>
#include <vector>
#include <stdexcept>

#include "Random.hpp"

namespace ea::stream
{
	/**
	   @enum StateType
	   @brief Describes how state of a stream is managed.
	 */
	enum class StateType
	{
		/*! The stream's state is moved when applying an operation. */
		stateful,
		/*! The stream's state is copied when applying an operation. */
		stateless
	};

	/**
	   @tparam InputIterator must meet the requirements of LegacyRandomAccessIterator
	   @tparam State describes how state is managed
	   @class Stream
	   @brief Apply evolutionary operators in a functional-style.
	 */
	template<typename InputIterator, StateType State = StateType::stateless>
	class Stream
	{
		public:
			/*! Stream iterator type.*/
			using iterator = typename std::vector<typename InputIterator::value_type>::const_iterator;

			/**
			   @param first first individual of a population
			   @param last points to the past-the-end element in the sequence

			   Creates a new Stream.
			 */
			Stream(InputIterator first, InputIterator last)
				: index(0)
				, first(first)
				, last(last)
			{
				std::copy(first, last, std::back_inserter(*state));
			}

			/**
			   @tparam Operator selection operator
			   @tparam Fitness fitness function object: double fun(InputIterator first, InputIterator last)
			   @param op selection operator
			   @param count number of individuals to select from the stream
			   @param fitness a fitness function
			   @returns new Stream object

			   Applies the given selection operator to the stream.
			 */
			template<typename Operator, typename Fitness>
			Stream select(Operator op, const size_t count, Fitness fitness)
			{
				Stream stream = dup();
				const int dst = !stream.index;

				stream.state[dst].clear();

				op(std::begin(stream.state[stream.index]),
				   std::end(stream.state[stream.index]),
				   count,
				   fitness,
				   std::back_inserter(stream.state[dst]));

				stream.index = dst;

				return stream;
			}

			/**
			   @tparam Operator mutation operator
			   @param op mutation operator
			   @param probability mutation probability (0..1)
			   @returns new Stream object

			   Applies the given mutation operator to the stream.

			   Throws std::invalid_argument if probability is out of range (0.0 <= p <= 1.0).
			 */
			template<typename Operator>
			Stream mutate(Operator op, const double probability = 0.08)
			{
				if(probability <= 0.0 || probability >= 1.0)
				{
					throw std::invalid_argument("Probability out of range.");
				}

				Stream stream = dup();
				const int dst = !stream.index;

				stream.state[dst].clear();

				const auto length = std::distance(std::begin(stream.state[index]), std::end(stream.state[index]));

				#pragma omp parallel
				{
					random::RandomEngine eng = random::default_engine();
					std::uniform_real_distribution<double> dist(0.0, 1.0);
					std::vector<typename InputIterator::value_type> mutants;

					#pragma omp for
					for(typename std::remove_const<decltype(length)>::type i = 0; i < length - 1; ++i)
					{
						auto mutant = stream.state[index][i];

						if(dist(eng) <= probability)
						{
							op(std::begin(mutant), std::end(mutant));
						}

						mutants.push_back(mutant);
					}

					#pragma omp critical
					std::move(std::begin(mutants), std::end(mutants), std::back_inserter(stream.state[dst]));
				}

				stream.index = dst;

				return stream;
			}

			/**
			   @tparam Operator crossover operator
			   @param op crossover operator
			   @returns new Stream object

			   Applies the given crossover operator to the stream.
			 */
			template<typename Operator>
			Stream crossover(Operator op)
			{
				Stream stream = dup();
				const int dst = !stream.index;

				stream.state[dst].clear();

				const auto length = std::distance(std::begin(stream.state[index]), std::end(stream.state[index]));

				#pragma omp parallel
				{
					std::vector<typename InputIterator::value_type> offsprings;

					#pragma omp for
					for(typename std::remove_const<decltype(length)>::type i = 0; i < length - 1; ++i)
					{
						for(auto j = i + 1; j < length; ++j)
						{
							op(std::begin(stream.state[index][i]),
							   std::end(stream.state[index][i]),
							   std::begin(stream.state[index][j]),
							   std::end(stream.state[index][j]),
							   std::back_inserter(offsprings));
						}
					}

					#pragma omp critical
					std::move(std::begin(offsprings), std::end(offsprings), std::back_inserter(stream.state[dst]));
				}

				stream.index = dst;

				return stream;
			}

			/**
			   @tparam OutputIterator must meet the requirements of LegacyOutputIterator
			   @param result beginning of the destination range

			   Copies (or moves) the stream to a destination range.
			 */
			template<typename OutputIterator>
			void take(OutputIterator result)
			{
				take(std::begin(state[index]), std::end(state[index]), result);
			}

			/**
			   @return first element of the stream

			   Returns the first element of the stream.
			 */
			iterator begin() const
			{
				return std::begin(state[index]);
			}

			/**
			   @return past-the-end element of the stream

			   Returns the past-the-end element of the stream.
			 */
			iterator end() const
			{
				return std::end(state[index]);
			}

		private:
			int index;
			InputIterator first;
			InputIterator last;
			std::vector<typename InputIterator::value_type> state[2];

			template<StateType S = State>
			typename std::enable_if<S == StateType::stateless, Stream>::type
			dup()
			{
				return *this;
			}

			template<StateType S = State>
			typename std::enable_if<S == StateType::stateful, Stream>::type
			dup()
			{
				return std::move(*this);
			}

			template<typename OutputIterator, StateType S = State>
			typename std::enable_if<S == StateType::stateless, void>::type
			take(InputIterator first, InputIterator last, OutputIterator result)
			{
				std::copy(first, last, result);
			}

			template<typename OutputIterator, StateType S = State>
			typename std::enable_if<S == StateType::stateful, void>::type
			take(InputIterator first, InputIterator last, OutputIterator result)
			{
				std::move(first, last, result);
			}
	};

	/**
	   @tparam InputIterator must meet the requirements of LegacyRandomAccessIterator
	   @param first first individual of a population
	   @param last points to the past-the-end element in the sequence
	   @return new Stream

	   Creates a new mutable stream.
	 */
	template<typename InputIterator>
	Stream<InputIterator, StateType::stateful> make_mutable(InputIterator first, InputIterator last)
	{
		return Stream<InputIterator, StateType::stateful>(first, last);
	}

	/**
	   @tparam InputIterator must meet the requirements of LegacyRandomAccessIterator
	   @param first first individual of a population
	   @param last points to the past-the-end element in the sequence
	   @return new Stream

	   Creates a new immutable stream.
	 */
	template<typename InputIterator>
	Stream<InputIterator, StateType::stateless> make_immutable(InputIterator first, InputIterator last)
	{
		return Stream<InputIterator, StateType::stateless>(first, last);
	}
}

#endif

