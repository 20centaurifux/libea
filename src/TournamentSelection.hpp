#ifndef EA_TOURNAMENT_SELECTION
#define EA_TOURNAMENT_SELECTION

#include <iterator>
#include <limits>
#include <stdexcept>

#include "Random.hpp"
#include "Utils.hpp"
#include "Fitness.hpp"

namespace ea::selection
{
	template<typename InputIterator, typename Compare = std::greater<double>>
	class Tournament
	{
		public:

			Tournament(const size_t Q = 3)
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
				length = std::distance(first, last);

				if(length < 0 || static_cast<typename std::make_unsigned<difference_type>::type>(length) <= Q)
				{
					throw std::length_error("Q exceeds population size.");
				}

				index_dist = std::uniform_int_distribution<difference_type>(0, length - 1);

				utils::repeat(count, [&]()
				{
					select(first, last, fitness, result);
				});
			}

		private:
			using difference_type = typename std::iterator_traits<InputIterator>::difference_type;

			const size_t Q;

			random::RandomEngine engine = random::default_engine();
			Compare compare;
			difference_type length;
			std::uniform_int_distribution<difference_type> index_dist;

			template<typename Fitness, typename OutputIterator>
			void select(InputIterator first, InputIterator last, Fitness fitness, OutputIterator result)
			{
				std::vector<difference_type> enemies(Q);

				random::fill_distinct_n_int(begin(enemies), Q, static_cast<difference_type>(0), length - 1);

				auto fitness_by_index = fitness::memoize_fitness_by_index<InputIterator>(fitness);
				difference_type index;

				do
				{
					index = random_index();
				} while(std::find(begin(enemies), end(enemies), index) != end(enemies));

				std::for_each(begin(enemies), end(enemies), [&](difference_type i)
				{
					index = compare_genotypes(first, index, i, fitness_by_index);
				});

				*result = *(first + index);
				++result;
			}

			difference_type random_index()
			{
				return index_dist(engine);
			}

			template<typename Fitness>
			difference_type compare_genotypes(InputIterator first, const difference_type a, const difference_type b, Fitness fitness) const
			{
				difference_type index = b;

				if(compare(fitness(first, a), fitness(first, b)))
				{
					index = a;
				}

				return index;
			}
	};
}

#endif

