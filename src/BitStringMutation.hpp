#ifndef EA_BITSTRING_MUTATION_HPP
#define EA_BITSTRING_MUTATION_HPP

#include <iterator>
#include <vector>
#include <stdexcept>

#include "Random.hpp"

namespace ea::mutation
{
	class BitString
	{
		public:
			BitString(const double probability = 0.8)
				: probability(probability)
			{
				if(probability <= 0.0 || probability >= 1.0)
				{
					throw std::invalid_argument("Probability out of range.");
				}
			}

			template<typename InputIterator>
			void operator()(InputIterator first, InputIterator last)
			{
				bool flipped = false;
				const auto length = std::distance(first, last);
				std::vector<double> probabilities(length);

				while(!flipped)
				{
					std::vector<double> probabilities(length);
					random::fill_n_real(begin(probabilities), length, 0.0, 1.0);

					auto n = begin(probabilities);

					for(auto g = first; g != last; g++, n++)
					{
						if(*n <= probability)
						{
							*g = !*g;
							flipped = true;
						}
					}
				}
			}

		private:
			const double probability;
	};
}

#endif

