#ifndef EA_DOUBLE_TOURNAMENT_SELECTION
#define EA_DOUBLE_TOURNAMENT_SELECTION

#include <iterator>
#include <limits>
#include <stdexcept>
#include <set>

#include "Random.hpp"
#include "Utils.hpp"
#include "Fitness.hpp"

namespace ea::selection
{
	template<typename InputIterator, typename Compare = std::greater<double>>
	class DoubleTournament
	{
		public:

			DoubleTournament(const size_t Q = 10)
				: Q(Q)
			{
				if(Q == 0)
				{
					throw std::invalid_argument("Q cannot be zero.");
				}
			}

			template<typename Fitness, typename OutputIterator>
			void operator()(InputIterator first, InputIterator last, const size_t count, Fitness fitness, OutputIterator result)
			{
				auto length = std::distance(first, last);

				if(length < 0 || static_cast<typename std::make_unsigned<difference_type>::type>(length) <= Q)
				{
					throw std::length_error("Q exceeds population size.");
				}

				std::vector<difference_type> challengers(count);

				random::fill_distinct_n_int(begin(challengers), count, static_cast<difference_type>(0), length - 1);

				if(std::numeric_limits<difference_type>::max() < count)
				{
					throw std::overflow_error("Arithmetic overflow.");
				}

				auto fitness_by_index = fitness::memoize_fitness_by_index<InputIterator>(fitness);
				Compare compare;

				std::multiset<Genotype> genotypes;

				for(size_t i = 0; i < count; ++i)
				{
					Genotype g = { challengers[i], 0 };

					std::vector<difference_type> enemies(Q);

					random::fill_distinct_n_int(begin(enemies), Q, static_cast<difference_type>(0), length - 1);

					std::for_each(begin(enemies), end(enemies), [&](difference_type j)
					{
						if(compare(fitness_by_index(first, i), fitness_by_index(first, j)))
						{
							++g.score;
						}
					});

					genotypes.insert(g);
				}

				std::for_each(begin(genotypes), end(genotypes), [&](const Genotype& g)
				{
					*result = *(first + g.index);
					++result;
				});
			}

		private:
			using difference_type = typename std::iterator_traits<InputIterator>::difference_type;

			typedef struct _Genotype
			{
				difference_type index;
				size_t score;

				bool operator<(const struct _Genotype& rhs) const
				{
					return score > rhs.score;
				}
			} Genotype;

			const size_t Q;
	};
}

#endif

