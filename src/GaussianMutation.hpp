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
   @file GaussianMutation.hpp
   @brief Adds a random number from a normal distribution to a gene.
   @author Sebastian Fedrau <sebastian.fedrau@gmail.com>
 */
#ifndef EA_GAUSSIAN_MUTATION_HPP
#define EA_GAUSSIAN_MUTATION_HPP

#include <iterator>
#include <vector>
#include <limits>
#include <stdexcept>

#include "Random.hpp"

namespace ea::mutation
{
	/**
	   @class Gaussian
	   @brief Adds a random number from a normal distribution to a gene.
	 */
	class Gaussian
	{
		public:
			/**
			   @param min minimum potential value
			   @param max maximum potential value
			   @param probability probability a gene is changed

			   Throws std::invalid_argument if probability is out of range (0.0 <= p <= 1.0).
			 */
			explicit Gaussian(const double min, const double max, const double probability = 0.8)
				: min(min)
				, max(max)
				, probability(probability)
			{
				if(probability <= 0.0 || probability >= 1.0)
				{
					throw std::invalid_argument("Probability out of range.");
				}
			}

			/**
			   @tparam InputIterator must meet the requirements of LegacyInputIterator
			   @param first points to the first element of a chromosome
			   @param last points to the end of a chromosome

			   Throws std::overflow_error if an overflow occurs.
			 */
			template<typename InputIterator>
			void operator()(InputIterator first, InputIterator last) const
			{
				random::RandomEngine engine = random::default_engine();
				std::uniform_real_distribution<double> prob(0.0, 1.0);
				std::normal_distribution<double> dist(min, max);

				std::feclearexcept(FE_OVERFLOW);

				for(auto g = first; g != last; ++g)
				{
					if(prob(engine) <= probability)
					{
						*g += dist(engine);

						if(std::fetestexcept(FE_OVERFLOW))
						{
							throw std::overflow_error("Arithmetic overflow.");
						}
					}
				}
			}

		private:
			const double min;
			const double max;
			const double probability;
	};
}

#endif

