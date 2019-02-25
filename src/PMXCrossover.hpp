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
   @file PMXCrossover.hpp
   @brief Copies a swath of consecutive genes from one parent. The corresponding
          swath from the other parent is sprinkled about in the child.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
*/
#ifndef EA_PMX_CROSSOVER_HPP
#define EA_PMX_CROSSOVER_HPP

#include <iterator>
#include <tuple>
#include <set>
#include <stdexcept>

#include "Random.hpp"

namespace ea::crossover
{
	 /**
	   @addtogroup Crossover
	   @{
	*/

	/**
	   @class PXM
	   @tparam Chromosome chromosome sequence type
	   @brief Copies a swath of consecutive genes from one parent. The corresponding
	          swath from the other parent is sprinkled about in the child.
	*/
	template<typename Chromosome>
	class PMX
	{
		public:
			/**
			   @tparam InputIterator must meet the requirements of LegacyRandomAccessIterator
			   @tparam OutputIterator must meet the requirements of LegacyOutputIterator
			   @param first1 points to the first element of the first chromosome
			   @param last1 points to the end of the first chromosome
			   @param first2 points to the first element of the second chromosome
			   @param last2 points to the end of the second chromosome
			   @param result beginning of the destination range
			   @return number of offsprings written to \p result

			   Combines two parents and generates two offsprings.

			   Throws std::length_error or std::logic_error if the genes of the parent
			   chromosomes aren't the same.
			*/
			template<typename InputIterator, typename OutputIterator>
			size_t operator()(InputIterator first1,
					  InputIterator last1,
					  InputIterator first2,
					  InputIterator last2,
					  OutputIterator result)
			{
				const auto length = std::distance(first1, last1);

				if(length != std::distance(first2, last2))
				{
					throw std::length_error("Chromosome lengths have to be equal.");
				}

				append<InputIterator>({ first1, last1 }, { first2, last2 }, length, result);
				append<InputIterator>({ first2, last2 }, { first1, last1 }, length, result);

				return 2;
			}

		private:
			template<typename T>
			using Range = typename std::tuple<T, T>;

			template<typename InputIterator, typename Difference, typename OutputIterator>
			static void append(Range<InputIterator> parent1,
			                   Range<InputIterator> parent2,
			                   const Difference length,
			                   OutputIterator result)
			{
				const auto &[first1, last1] = parent1;
				const auto &[first2, last2] = parent2;
				const auto[from, to] = generate_swath(length);

				Chromosome offspring(length);

				std::copy(first1 + from, first1 + to, begin(offspring) + from);

				std::vector<bool> assigned(length);

				for(auto offset = from; offset < to; ++offset)
				{
					const auto index = copy_from_swath<InputIterator>({ first1, last1 },
					                                                  { from, to },
					                                                  { first2, last2 },
					                                                  offset,
					                                                  begin(offspring));

					if(index != -1)
					{
						assigned[index] = true;
					}
				}

				copy_unassigned<InputIterator>({ first2, last2 },
				                               { from, to },
				                               static_cast<Difference>(0),
				                               assigned,
				                               begin(offspring));

				*result++ = offspring;
			}

			template <typename T>
			static Range<T> generate_swath(const T length)
			{
				T swath[2];

				random::fill_distinct_n_int(swath, 2, static_cast<T>(0), length - 1);

				return std::make_tuple(std::min(swath[0], swath[1]), std::max(swath[0], swath[1]));
			}

			template<typename InputIterator, typename Difference, typename OutputIterator>
			static Difference copy_from_swath(Range<InputIterator> parent1,
			                                  const Range<Difference> swath,
			                                  Range<InputIterator> parent2,
			                                  const Difference offset2,
			                                  OutputIterator result)
			{
				const auto &[first1, last1] = parent1;
				const auto [from, to] = swath;
				const auto &[first2, last2] = parent2;
				const auto &g2 = *(first2 + offset2);
				auto index = -1;

				if(std::none_of(first1 + from,
				                first1 + to,
				                [&g2](const auto &g1) { return g1 == g2; }))
				{
					index = offset2;

					do
					{
						const auto g1 = *(first1 + index);
						const InputIterator match = std::find(first2, last2, g1);

						if(match == last2)
						{
							throw std::logic_error("Chromosomes aren't equal.");
						}

						index = std::distance(first2, match);
					} while(index >= from && index < to);

					*(result + index) = g2;
				}

				return index;
			}

			template<typename InputIterator, typename Difference, typename Bitmap, typename OutputIterator>
			static void copy_unassigned(Range<InputIterator> parent,
			                            const Range<Difference> swath,
			                            const Difference offset,
			                            Bitmap &bitmap,
			                            OutputIterator offspring)
			{
				const auto &[first, last] = parent;
				const auto[from, to] = swath;

				if(offset < std::distance(first, last))
				{
					auto next_offset = to;

					if(offset < from || offset >= to)
					{
						if(!bitmap[offset])
						{
							offspring[offset] = *(first + offset);
						}

						next_offset = offset + 1;
					}

					copy_unassigned(parent, swath, next_offset, bitmap, offspring);
				}
			}
	};

	/*! @} */
}

#endif

