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
   @file FitnessProportionalSelection.hpp
   @brief Implementation of the fitness-proportional selection.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */

#ifndef FITNESSPROPORTIONALSELECTION_H
#define FITNESSPROPORTIONALSELECTION_H

#include <vector>
#include <cassert>
#include <algorithm>
#include <functional>
#include <cstdlib>
#include <memory>
#include "IIndexSelection.hpp"
#include "ARandomNumberGenerator.hpp"
#include "algorithms.hpp"

namespace ea
{
	/**
	   @addtogroup Operators
	   @{
	   	@addtogroup Selection
		@{
	 */

	/**
	   @class FitnessProportionalSelection
	   @tparam TGenomeBase genome base class
	   @brief Implementation of fitness-proportional selection. Use this operator
	          with positive fitness values only.
	 */
	template<typename TGenomeBase>
	class FitnessProportionalSelection : public IIndexSelection<TGenomeBase>
	{
		public:
			/**
			   @param rnd instance of a random number generator
			 */
			FitnessProportionalSelection(std::shared_ptr<ARandomNumberGenerator> rnd)
			{
				assert(rnd != nullptr);
				rnd = rnd;
			}

			FitnessProportionalSelection()
			{
				rnd = std::make_shared<TR1UniformDistribution<>>();
			}

			~FitnessProportionalSelection() {}

			void select(IInputAdapter<typename TGenomeBase::sequence_type>& input, const uint32_t count, IOutputAdapter<uint32_t>& output) override
			{
				double* sums;

				assert(input.size() >= 2);
				assert(count > 0);

				sums = new double[input.size()];

				cumulate(sums, input);
				select_genomes(input, sums, count, output);

				delete[] sums;
			}

		protected:
			/*! A genome base class. */
			static TGenomeBase base;
			/*! A random number generator. */
			std::shared_ptr<ARandomNumberGenerator> rnd;

			/**
			   @param sums an array holding cumulated fitness values
			   @param input a population
			   @param align value to add to each fitness value

			   Fills an array with cumulated fitness values.
			 */
			void cumulate(double* sums, IInputAdapter<typename TGenomeBase::sequence_type>& input, const double align = 0) const
			{
				double fitness = base.fitness(input.at(0));

				sums[0] = chk_add(fitness, align);

				for(uint32_t i = 1; i < input.size(); i++)
				{
					fitness = base.fitness(input.at(i));
					sums[i] = chk_add({ sums[i - 1], fitness, align });
				}
			}

			/**
			   @param input a population
			   @param sums an array containing cumulated fitness values
			   @param count number of individuals to select
			   @param output location to write selected genomes to

			   Selects genomes respecting the cumulated fitness values.
			 */
			void select_genomes(IInputAdapter<typename TGenomeBase::sequence_type>& input, const double* sums, const uint32_t count, IOutputAdapter<uint32_t>& output) const
			{
				double* numbers;
				double max;
				uint32_t range[2];

				max = sums[input.size() - 1];
				assert(max > 0);

				numbers = new double[count];

				rnd->get_double_seq(0, max, numbers, count);

				for(uint32_t i = 0; i < count; i++)
				{
					range[0] = 0;
					range[1] = input.size() - 1;

					output.push(find(sums, range, numbers[i]));
				}

				delete[] numbers;
			}

			/**
			   @param sums array containing cumulated fitness values
			   @param range range of the search
			   @param n value to search
			   @return index of the found value

			   Searches for a value in the cumulated fitness value array.
			 */
			static uint32_t find(const double* sums, uint32_t range[2], const double n)
			{
				uint32_t mid;

				ASSERT_FP_NORMALITY(n);
				assert(range[0] <= range[1]);
				assert(n >= 0.0);
				assert(sums[range[1]] >= n);

				while(range[1] - range[0] > 0)
				{
					mid = (range[1] - range[0]) / 2 + range[0];

					if(sums[mid] > n)
					{
						range[1] = (range[1] == mid) ? range[1] - 1 : mid;
					}
					else
					{
						range[0] = (range[0] == mid) ? range[0] + 1 : mid;
					}
				}

				return range[0];
			}
	};

	template<typename TGenomeBase>
	TGenomeBase FitnessProportionalSelection<TGenomeBase>::base;

	/**
	   @class AlignedFitnessProportionalSelection
	   @tparam TGenomeBase genome base class
	   @brief Implementation of fitness-proportional selection. This operator aligns
	          fitness values and can be used with positive and negative numbers.
	 */
	template<typename TGenomeBase>
	class AlignedFitnessProportionalSelection : public FitnessProportionalSelection<TGenomeBase>
	{
		public:
			/**
			   @param rnd instance of a random number generator
			 */
			AlignedFitnessProportionalSelection(std::shared_ptr<ARandomNumberGenerator> rnd) : FitnessProportionalSelection<TGenomeBase>(rnd) {}

			AlignedFitnessProportionalSelection() : FitnessProportionalSelection<TGenomeBase>() {}

			~AlignedFitnessProportionalSelection() {}

			void select(IInputAdapter<typename TGenomeBase::sequence_type>& input, const uint32_t count, IOutputAdapter<uint32_t>& output)
			{
				double s;
				double* sums;
				double align = 0.0;

				assert(input.size() >= 2);
				assert(count > 0);

				if((s = smallest(input)) < 0)
				{
					assert_addition(abs(s), abs(s));
					align = abs(s) * 2;
				}

				sums = new double[input.size()];

				this->cumulate(sums, input, align);
				this->select_genomes(input, sums, count, output);

				delete[] sums;
			}

		private:
			using FitnessProportionalSelection<TGenomeBase>::base;

			static double smallest(IInputAdapter<typename TGenomeBase::sequence_type>& input)
			{
				double min;
				double fitness;

				input.first();
				min = base.fitness(input.current());
				ASSERT_FP_NORMALITY(min);
				input.next();

				while(!input.end())
				{
					fitness = base.fitness(input.current());
					ASSERT_FP_NORMALITY(fitness);

					if(fitness < min)
					{
						min = fitness;
					}

					input.next();
				}

				return min;
			}
	};

	/**
	   @class MinimizingFitnessProportionalSelection
	   @tparam TGenomeBase genome base class
	   @brief Implementation of fitness-proportional selection for minimization
	          problems. This operator can be used with positive and negative
	          numbers.
	 */
	template<typename TGenomeBase>
	class MinimizingFitnessProportionalSelection : public FitnessProportionalSelection<TGenomeBase>
	{
		public:
			using FitnessProportionalSelection<TGenomeBase>::base;
			using FitnessProportionalSelection<TGenomeBase>::rnd;

			/**
			   @param rnd instance of a random number generator
			 */
			MinimizingFitnessProportionalSelection(std::shared_ptr<ARandomNumberGenerator> rnd) : FitnessProportionalSelection<TGenomeBase>(rnd) {}

			MinimizingFitnessProportionalSelection() : FitnessProportionalSelection<TGenomeBase>() {}

			~MinimizingFitnessProportionalSelection() {}

			void select(IInputAdapter<typename TGenomeBase::sequence_type>& input, const uint32_t count, IOutputAdapter<uint32_t>& output)
			{
				std::vector<typename TGenomeBase::sequence_type> ordered;
				double* sums;
				double align = 0.0;
				uint32_t i = 1;
				uint32_t range[2];
				double fitness;

				assert(input.size() >= 2);
				assert(count > 0);

				// sort genomes ascending by their fitness values:
				while(!input.end())
				{
					ordered.push_back(input.current());
					input.next();
				}

				std::sort(begin(ordered), end(ordered),
				          []
				          (const typename TGenomeBase::sequence_type& a, const typename TGenomeBase::sequence_type& b)
				          {
				          	static TGenomeBase base;

				          	return base.cmp(a, b) > 0;
				          });

				// cumulate (aligned) fitness values:
				sums = new double[input.size()];
				sums[0] = base.fitness(ordered[0]);

				if(sums[0] < 0)
				{
					assert_addition(abs(sums[0]), abs(sums[0]));
					align = abs(sums[0]) * 2;
					sums[0] = chk_add(sums[0], align);
				}

				for(auto iter = begin(ordered) + 1; iter < end(ordered); iter++, i++)
				{
					fitness = base.fitness(*iter);
					sums[i] = chk_add({ sums[i - 1], fitness, align });
				}

				// select genomes:
				auto numbers = new double[count];
				rnd->get_double_seq(0, sums[ordered.size() - 1], numbers, count);

				for(i = 0; i < count; i++)
				{
					range[0] = 0;
					range[1] = ordered.size() - 1;

					auto index = this->find(sums, range, numbers[i]);

					assert(index < ordered.size());

					index = chk_sub({ (uint32_t)ordered.size(), 1u, index });

					output.push(map_sequence(input, ordered[index]));
				}

				// cleanup:
				delete[] numbers;
				delete[] sums;
			}

		private:
			static uint32_t map_sequence(IInputAdapter<typename TGenomeBase::sequence_type>& input, typename TGenomeBase::sequence_type& seq)
			{
				uint32_t index = 0;

				input.first();

				while(!input.end())
				{
					if(!base.cmp(input.current(), seq))
					{
						return index;
					}

					input.next(), index++;
				}

				std::abort(); // should never be reached 

				return 0;
			}
	};

	/**
		   @}
	   @}
	 */
}
#endif
