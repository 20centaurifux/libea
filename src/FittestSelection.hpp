#ifndef EA_FITTEST_SELECTION
#define EA_FITTEST_SELECTION

#include <limits>
#include <stdexcept>
#include <set>

#include "Random.hpp"
#include "Utils.hpp"
#include "Fitness.hpp"

namespace ea::selection
{
	template<typename InputIterator, typename Compare = std::greater<double>>
	class Fittest
	{
		public:
			template<typename Fitness, typename OutputIterator>
			void operator()(InputIterator first, InputIterator last, const size_t count, Fitness fitness, OutputIterator result)
			{
				std::multiset<Genotype> genotypes;
				size_t i = 0;

				for(auto p = first; p != last; ++p, ++i)
				{ 
					genotypes.insert({ i, fitness(begin(*p), end(*p)) });

					if(i == std::numeric_limits<size_t>::max())
					{
						throw std::overflow_error("Arithmetic overflow.");
					}
				};

				i = 0;

				for(auto g = begin(genotypes); g != end(genotypes) && i < count; ++g, ++i, ++result)
				{
					*result = *(first + g->index);
				};
			}

		private:
			typedef struct _Genotype
			{
				size_t index;
				double fitness;

				bool operator<(const struct _Genotype& rhs) const
				{

					return Compare()(fitness, rhs.fitness);
				}
			} Genotype;
	};
}

#endif


