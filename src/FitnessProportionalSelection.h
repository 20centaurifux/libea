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
   @file FitnessProportionalSelection.h
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
#include "ASelection.h"

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
	   @tparam TGenome type of the genome class
	   @brief Implementation of fitness-proportional selection. Use this operator
	          with positive fitness values only.
	 */
	template<typename TGenome>
	class FitnessProportionalSelection : public ASelection<TGenome>
	{
		public:
			using ASelection<TGenome>::select;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			FitnessProportionalSelection(std::shared_ptr<ARandomNumberGenerator> rnd_generator)
				: ASelection<TGenome>(rnd_generator) {}

			~FitnessProportionalSelection() {}

		protected:
			using ASelection<TGenome>::generator;

			virtual void select_impl(IInputAdapter<std::shared_ptr<TGenome>> &input, const uint32_t count, IOutputAdapter<std::shared_ptr<TGenome>> &output) override
			{
				double* sums;

				sums = new double[input.size()];

				cumulate(sums, input);
				select(input, sums, count, output);

				delete[] sums;
			}

			/**
			   @param sums an array
			   @param input a population
			   @param align value to add to each cumulated sum

			   Fills an array with cumulated fitness values.
			 */
			void cumulate(double* sums, IInputAdapter<std::shared_ptr<TGenome>> &input, const double align = 0) const
			{
				sums[0] = input.at(0)->fitness() + align;

				for(uint32_t i = 1; i < input.size(); i++)
				{
					sums[i] = sums[i - 1] + input.at(i)->fitness() + align;
				}
			}

			/**
			   @param input a population
			   @param sums an array containing cumulated fitness values
			   @param count number of individuals to select
			   @param output location to write selected genomes to

			   Selects genomes respecting the cumulated fitness values.
			 */
			void select(IInputAdapter<std::shared_ptr<TGenome>> &input, const double* sums, const uint32_t count, IOutputAdapter<std::shared_ptr<TGenome>> &output) const
			{
				double* numbers;
				double max;
				uint32_t range[2];

				max = sums[input.size() - 1];
				assert(max > 0);

				numbers = new double[count];

				generator->get_double_seq(0, max, numbers, count);

				for(uint32_t i = 0; i < count; i++)
				{
					range[0] = 0;
					range[1] = input.size() - 1;

					output.append(input.at(find(sums, range, numbers[i])));
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
			uint32_t find(const double* sums, uint32_t range[2], const double n) const
			{
				uint32_t mid;

				while(range[1] - range[0] > 1)
				{
					mid = (range[1] - range[0]) / 2 + range[0];

					if(sums[mid] > n)
					{
						range[1] = mid;
					}
					else
					{
						range[0] = mid;
					}
				}

				return range[0];
			}
	};

	/**
	   @class AlignedFitnessProportionalSelection
	   @tparam TGenome type of the genome class
	   @brief Implementation of fitness-proportional selection. This operator aligns
	          fitness values and can be used with positive and negative numbers.
	 */
	template<typename TGenome>
	class AlignedFitnessProportionalSelection : public FitnessProportionalSelection<TGenome>
	{
		public:
			using FitnessProportionalSelection<TGenome>::select;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			AlignedFitnessProportionalSelection(std::shared_ptr<ARandomNumberGenerator> rnd_generator)
				: FitnessProportionalSelection<TGenome>(rnd_generator) {}

			~AlignedFitnessProportionalSelection() {}

		protected:
			void select_impl(IInputAdapter<std::shared_ptr<TGenome>> &input, const uint32_t count, IOutputAdapter<std::shared_ptr<TGenome>> &output) override
			{
				double s;
				double* sums;
				double align = 0.0;

				if((s = smallest(input)) < 0)
				{
					align = abs(s) * 2;
				}

				sums = new double[input.size()];

				this->cumulate(sums, input, align);
				this->select(input, sums, count, output);

				delete[] sums;
			}

		private:
			double smallest(IInputAdapter<std::shared_ptr<TGenome>> &input)
			{
				double min;

				input.first();
				min = input.current()->fitness();
				input.next();

				while(!input.end())
				{
					if(input.current()->fitness() < min)
					{
						min = input.current()->fitness();
					}

					input.next();
				}

				return min;
			}
	};

	/**
	   @class MinimizingFitnessProportionalSelection
	   @tparam TGenome type of the genome class
	   @brief Implementation of fitness-proportional selection for minimization
	          problems. This operator can be used with positive and negative
	          numbers.
	 */
	template<typename TGenome>
	class MinimizingFitnessProportionalSelection : public FitnessProportionalSelection<TGenome>
	{
		public:
			using FitnessProportionalSelection<TGenome>::select;

			/**
			   @param rnd_generator instance of a random number generator
			 */
			MinimizingFitnessProportionalSelection(std::shared_ptr<ARandomNumberGenerator> rnd_generator)
				: FitnessProportionalSelection<TGenome>(rnd_generator) {}

			~MinimizingFitnessProportionalSelection() {}

		protected:
			using ASelection<TGenome>::generator;

			void select_impl(IInputAdapter<std::shared_ptr<TGenome>> &input, const uint32_t count, IOutputAdapter<std::shared_ptr<TGenome>> &output) override
			{
				std::vector<std::shared_ptr<TGenome>> ordered;
				double* sums;
				uint32_t i = 1;
				double align = 0.0;
				double* numbers;
				uint32_t range[2];
				uint32_t index;

				// sort genomes by fitness value:
				while(!input.end())
				{
					ordered.push_back(input.current());
					input.next();
				}

				std::sort(begin(ordered), end(ordered),
				          [](const std::shared_ptr<TGenome> &a, const std::shared_ptr<TGenome> &b) { return a->fitness() > b->fitness(); });

				// cumulate (aligned) fitness values:
				sums = new double[input.size()];
				sums[0] = ordered[0]->fitness();

				if(sums[0] < 0)
				{
					align = abs(sums[0]) * 2;
					sums[0] += align;
				}

				for(auto iter = begin(ordered) + 1; iter < end(ordered); iter++, i++)
				{
					sums[i] = sums[i - 1] + (*iter)->fitness() + align;
				}

				// select genomes:
				numbers = new double[count];
				generator->get_double_seq(0, sums[ordered.size() - 1], numbers, count);

				for(i = 0; i < count; i++)
				{
					range[0] = 0;
					range[1] = ordered.size() - 1;


					index = this->find(sums, range, numbers[i]);
					output.append(ordered.at(ordered.size() - 1 - index));
				}

				delete[] numbers;
				delete[] sums;
			}
	};
}
#endif
