#ifndef EA_FITNESSPROPORTIONALSELECTION_HPP
#define EA_FITNESSPROPORTIONALSELECTION_HPP

#include <iterator>
#include <limits>
#include <cfenv>
#include <stdexcept>
#include <vector>

#include "Random.hpp"

namespace ea::selection
{
	enum class Proportionality
	{
		direct,
		inverse
	};

	template<typename InputIterator>
	class FitnessProportional
	{
		public:
			FitnessProportional(Proportionality proportionality = Proportionality::direct)
				: proportionality(proportionality)
			{}

			template<typename Fitness, typename OutputIterator>
			void operator()(InputIterator first, InputIterator last, const size_t count, Fitness fitness, OutputIterator result)
			{
				std::vector<Slice> wheel;

				const size_t length = insert_slices(first, last, fitness, std::back_inserter(wheel));

				if(count > 0 && length == 0)
				{
					throw std::length_error("Population is empty.");
				}

				std::sort(begin(wheel), end(wheel));

				const double sum = align(begin(wheel), end(wheel));

				if(sum == 0) // all fitness values are zero
				{
					std::fill_n(result, count, *first);
				}
				else
				{
					accumulate(begin(wheel), end(wheel), sum);

					std::vector<double> numbers;

					random::fill_n_real(std::back_inserter(numbers), count, 0.0, 1.0);

					std::for_each(begin(numbers), end(numbers), [&](const double n)
					{
						auto slice = std::upper_bound(begin(wheel), end(wheel), n, [](const double n, const Slice &slice)
						{
							return n < slice.size;
						});

						if(proportionality == Proportionality::inverse)
						{
							auto offset = std::distance(slice, std::prev(end(wheel)));

							slice = begin(wheel) + offset;
						}

						*result = *(first + slice->index);
						++result;
					});
				}
			}

		private:
			typedef struct _Slice
			{
				size_t index;
				double size;

				bool operator<(const struct _Slice& rhs) const
				{
					return size < rhs.size;
				}
			} Slice;

			const Proportionality proportionality;

			template<typename Fitness, typename OutputIterator>
			static size_t insert_slices(InputIterator first, InputIterator last, Fitness fitness, OutputIterator result)
			{
				return std::accumulate(first, last, 0, [&fitness, &result](const size_t index, auto &g)
				{
					if(index == std::numeric_limits<size_t>::max())
					{
						throw std::overflow_error("Arithmetic overflow.");
					}

					*result = { index, fitness(begin(g), end(g)) };
					++result;

					return index + 1;
				});
			}

			template<typename Iterator>
			static double align(Iterator first, Iterator last)
			{
				std::feclearexcept(FE_OVERFLOW);

				const double alignment = std::abs(std::min(0.0, first->size)) * 2;

				test_fe_overflow();

				return std::accumulate(first, last, 0.0, [&alignment](double sum, Slice &slice)
				{
					slice.size += alignment;
					test_fe_overflow();

					sum += slice.size;
					test_fe_overflow();

					return sum;
				});
			}

			template<typename Iterator>
			static void accumulate(Iterator first, Iterator last, const double sum)
			{
				double total = 0.0;

				std::for_each(first, last, [&total, &sum](Slice &slice)
				{
					slice.size /= sum;
					slice.size += total;

					total = slice.size;
				});
			}

			static void test_fe_overflow()
			{
				if(std::fetestexcept(FE_OVERFLOW))
				{
					throw std::overflow_error("Arithmetic overflow.");
				}
			}
	};
}

#endif

