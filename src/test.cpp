#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>

#include <vector>
#include <algorithm>
#include <functional>
#include <limits>
#include <cmath>

using DefaultTestGenome = std::vector<int>;
using DefaultTestPopulation = std::vector<DefaultTestGenome>;

#include "Utils.hpp"

class UtilsTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(UtilsTest);
	CPPUNIT_TEST(repeat);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void repeat()
		{
			int n = 0;

			ea::utils::repeat(100, [&n]()
			{
				++n;
			});

			CPPUNIT_ASSERT_EQUAL(100, n);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(UtilsTest);

#include "Random.hpp"

class RandomTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(RandomTest);
	CPPUNIT_TEST(fill_n_int_size);
	CPPUNIT_TEST(fill_n_int_range);
	CPPUNIT_TEST(fill_n_int_distribution);
	CPPUNIT_TEST(fill_n_int_difference);
	CPPUNIT_TEST(fill_distinct_n_int_size);
	CPPUNIT_TEST(fill_distinct_n_int_range);
	CPPUNIT_TEST(fill_distinct_n_int_difference);
	CPPUNIT_TEST(fill_distinct_n_int_uniqueness);
	CPPUNIT_TEST(fill_distinct_n_int_invalid_range_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void fill_n_int_size()
		{
			std::vector<int> numbers(MAX_NUMBERS);

			ea::random::fill_n_int(begin(numbers), MAX_NUMBERS, -100, 100);

			CPPUNIT_ASSERT_EQUAL(MAX_NUMBERS, numbers.size());
		}

		void fill_n_int_range()
		{
			std::vector<int> numbers(MAX_NUMBERS);

			ea::random::fill_n_int(begin(numbers), MAX_NUMBERS, -100, 100);

			std::for_each(begin(numbers), end(numbers), [](const int n)
			{
				CPPUNIT_ASSERT(n >= -100 && n <= 100);
			});
		}

		void fill_n_int_distribution()
		{
			std::vector<int> numbers(MAX_NUMBERS);

			ea::random::fill_n_int(begin(numbers), MAX_NUMBERS, 0, 1);

			int a = 0;
			int b = 0;

			std::for_each(begin(numbers), end(numbers), [&a, &b](auto n)
			{
				(n == 0 ? a : b)++;
			});

			CPPUNIT_ASSERT(std::abs(a - b) < 1000);
		}

		void fill_n_int_difference()
		{
			std::vector<int> a(MAX_NUMBERS);

			ea::random::fill_n_int(begin(a), MAX_NUMBERS, -100, 100);

			std::vector<int> b(MAX_NUMBERS);

			ea::random::fill_n_int(begin(b), MAX_NUMBERS, -100, 100);

			size_t differences = 0;

			for(size_t i = 0; i < MAX_NUMBERS; ++i)
			{
				if(a[i] != b[i])
				{
					++differences;
				}
			}

			CPPUNIT_ASSERT(differences > MAX_NUMBERS / 2);
		}

		void fill_distinct_n_int_size()
		{
			std::vector<int> numbers(MAX_DISTINCT_NUMBERS);

			ea::random::fill_distinct_n_int(begin(numbers), MAX_DISTINCT_NUMBERS, -MAX_DISTINCT_NUMBERS, MAX_DISTINCT_NUMBERS);

			CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(MAX_DISTINCT_NUMBERS), numbers.size());
		}

		void fill_distinct_n_int_range()
		{
			std::vector<int> numbers(MAX_DISTINCT_NUMBERS);

			ea::random::fill_n_int(begin(numbers), MAX_DISTINCT_NUMBERS, -MAX_DISTINCT_NUMBERS, MAX_DISTINCT_NUMBERS);

			std::for_each(begin(numbers), end(numbers), [&](const int n)
			{
				CPPUNIT_ASSERT(n >= -MAX_DISTINCT_NUMBERS && n <= MAX_DISTINCT_NUMBERS);
			});
		}

		void fill_distinct_n_int_uniqueness()
		{
			std::vector<int> numbers(MAX_DISTINCT_NUMBERS);

			ea::random::fill_distinct_n_int(begin(numbers), MAX_DISTINCT_NUMBERS, -MAX_DISTINCT_NUMBERS, MAX_DISTINCT_NUMBERS);

			std::vector<int> unique;

			std::unique_copy(begin(numbers), end(numbers), std::back_inserter(unique));

			CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(MAX_DISTINCT_NUMBERS), unique.size());
		}

		void fill_distinct_n_int_difference()
		{
			std::vector<int> a(MAX_DISTINCT_NUMBERS);

			ea::random::fill_distinct_n_int(begin(a), MAX_DISTINCT_NUMBERS, 1, MAX_DISTINCT_NUMBERS);
			return;
			std::vector<int> b(MAX_NUMBERS);

			ea::random::fill_distinct_n_int(begin(b), MAX_DISTINCT_NUMBERS, 1, MAX_DISTINCT_NUMBERS);

			int differences = 0;

			for(int i = 0; i < MAX_DISTINCT_NUMBERS; ++i)
			{
				if(a[i] != b[i])
				{
					++differences;
				}
			}

			CPPUNIT_ASSERT(differences > MAX_DISTINCT_NUMBERS / 2);
		}

		void fill_distinct_n_int_invalid_range_args()
		{
			std::vector<int> numbers;

			CPPUNIT_ASSERT_THROW(ea::random::fill_distinct_n_int(std::back_inserter(numbers), 100, 10, -10), std::invalid_argument);
			CPPUNIT_ASSERT_THROW(ea::random::fill_distinct_n_int(std::back_inserter(numbers), 100, -10, -20), std::invalid_argument);
			CPPUNIT_ASSERT_THROW(ea::random::fill_distinct_n_int(std::back_inserter(numbers), 100, 1, 99), std::invalid_argument);
			CPPUNIT_ASSERT_THROW(ea::random::fill_distinct_n_int(std::back_inserter(numbers), 100, -99, -1), std::invalid_argument);
		}

	private:
		const size_t MAX_NUMBERS = 81920;
		const int MAX_DISTINCT_NUMBERS = 2048;
};

CPPUNIT_TEST_SUITE_REGISTRATION(RandomTest);

#include "Fitness.hpp"

class FitnessByIndexTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(FitnessByIndexTest);
	CPPUNIT_TEST(fitness_by_index);
	CPPUNIT_TEST(memoized_fitness_by_index);
	CPPUNIT_TEST_SUITE_END();

	public:
		using DefaultTestGenome = std::vector<int>;
		using DefaultTestPopulation = std::vector<DefaultTestGenome>;

		void setUp()
		{
			std::generate_n(std::back_inserter(population), POPULATION_SIZE, [&]()
			{
				DefaultTestGenome g(GENOME_SIZE);

				ea::random::fill_n_int(begin(g), GENOME_SIZE, 1, 1024);

				return g;
			});
		}

		void tearDown()
		{
			population.clear();
		}

	protected:
		void fitness_by_index()
		{
			compare_fitness_functions(ea::fitness::fitness_by_index<DefaultTestPopulation::iterator>(fitness));
		}

		void memoized_fitness_by_index()
		{
			compare_fitness_functions(ea::fitness::memoize_fitness_by_index<DefaultTestPopulation::iterator>(fitness));
		}

	private:
		const size_t POPULATION_SIZE = 4096;
		const size_t GENOME_SIZE = 64;

		DefaultTestPopulation population;

		static double fitness(const DefaultTestGenome::iterator first, const DefaultTestGenome::iterator last)
		{
			return static_cast<double>(std::accumulate(first,
			                                           last,
			                                           0,
			                                           [](int total, int n)
			                                           { return total + n; }) / std::distance(first, last));
		}

		void compare_fitness_functions(std::function<double(const DefaultTestPopulation::iterator, const size_t index)> fn)
		{
			size_t i = 0;

			std::for_each(begin(population), end(population), [&](DefaultTestGenome g)
			{
				double a = fitness(begin(g), end(g));;
				double b = fn(begin(population), i);

				CPPUNIT_ASSERT(std::abs(a - b) < std::numeric_limits<double>::epsilon());

				++i;
			});
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(FitnessByIndexTest);

class FitnessUtilitiesTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(FitnessUtilitiesTest);
	CPPUNIT_TEST(mean);
	CPPUNIT_TEST(mean_empty);
	CPPUNIT_TEST(median);
	CPPUNIT_TEST(median_empty);
	CPPUNIT_TEST(fittest);
	CPPUNIT_TEST(fittest_empty);
	CPPUNIT_TEST_SUITE_END();

	public:
		using DefaultTestGenome = std::vector<int>;
		using DefaultTestPopulation = std::vector<DefaultTestGenome>;
		using Fitness = std::function<double(DefaultTestGenome::iterator, DefaultTestGenome::iterator)>;

	protected:
		void mean()
		{
			DefaultTestPopulation population;
			size_t size = 1;

			std::generate_n(std::back_inserter(population), 10, [&size]()
			{
				DefaultTestGenome g;

				for(size_t i = 0; i < size; ++i)
				{
					g.push_back(i + 1);
				}

				++size;

				return g;
			});

			auto fn = [](DefaultTestGenome::iterator first, DefaultTestGenome::iterator last)
			{
				return static_cast<double>(std::accumulate(first, last, 1, [](int factorial, int n) { return factorial * n; }));
			};

			double mean = ea::fitness::mean(begin(population), end(population), fn);

			CPPUNIT_ASSERT(std::abs(mean - 403791.3) < std::numeric_limits<double>::epsilon());
		}

		void mean_empty()
		{
			test_empty_set_returns_nan(ea::fitness::mean<DefaultTestPopulation::iterator>);
		}

		void median()
		{
			DefaultTestPopulation population;

			ea::random::RandomEngine engine = ea::random::default_engine();
			std::uniform_int_distribution<> dist(1, 100);

			std::generate_n(std::back_inserter(population), 8192, [&dist, &engine]()
			{
				size_t length = 100;

				if(dist(engine) <= 20)
				{
					length /= 10;
				}

				return DefaultTestGenome(length);
			});

			double median = ea::fitness::median(begin(population), end(population), [](auto first, auto last)
			{
				return static_cast<double>(std::distance(first, last));
			});

			CPPUNIT_ASSERT(std::abs(median - 100) < std::numeric_limits<double>::epsilon());
		}

		void median_empty()
		{
			test_empty_set_returns_nan(ea::fitness::median<DefaultTestPopulation::iterator>);
		}

		void fittest()
		{
			DefaultTestPopulation population;

			size_t length = 1;

			std::generate_n(std::back_inserter(population), 100, [&length]()
			{
				return DefaultTestGenome(length++);
			});
	
			std::generate_n(std::back_inserter(population), 100, [&length]()
			{
				return DefaultTestGenome(--length);
			});

			auto fittest = ea::fitness::fittest(begin(population), end(population), std::distance<DefaultTestGenome::iterator>);

			CPPUNIT_ASSERT(fittest->size() == 100);
		}

		void fittest_empty()
		{
			DefaultTestPopulation population;

			auto pos = ea::fitness::fittest(begin(population), end(population), std::distance<DefaultTestGenome::iterator>);

			CPPUNIT_ASSERT(pos == end(population));
		}

	private:
		template<typename Fn>
		void test_empty_set_returns_nan(Fn fn)
		{
			DefaultTestPopulation population;

			double value = fn(begin(population), end(population), std::distance<DefaultTestGenome::iterator>);

			CPPUNIT_ASSERT(std::fpclassify(value) == FP_NAN);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(FitnessUtilitiesTest);

template<typename Selection>
static void select_children(Selection select, const size_t size = 1000, const size_t count = 100)
{
	DefaultTestPopulation population;

	std::generate_n(std::back_inserter(population), size, [&]()
	{
		return DefaultTestGenome(10);
	});

	std::function<double(DefaultTestGenome::iterator, DefaultTestGenome::iterator)>
	fn = [](DefaultTestGenome::iterator first, DefaultTestGenome::iterator last)
	{
		return 0.0;
	};

	DefaultTestPopulation children;

	select(begin(population), end(population), 0, fn, std::back_inserter(children));
	select(begin(population), end(population), count, fn, std::back_inserter(children));

	CPPUNIT_ASSERT(children.size() == count);
}

template<typename Compare = std::greater<double>, typename Selection>
static void fitness_increases(Selection select, const size_t size = 10000, const size_t count = 1000)
{
	DefaultTestPopulation population;

	std::generate_n(std::back_inserter(population), size, [&]()
	{
		DefaultTestGenome g;

		ea::random::fill_n_int(std::back_inserter(g), 10, -100, 100);

		return g;
	});

	std::function<double(DefaultTestGenome::iterator, DefaultTestGenome::iterator)>
	fn = [](DefaultTestGenome::iterator first, DefaultTestGenome::iterator last)
	{
		return static_cast<double>(std::accumulate(first, last, 0, [](int total, int n) { return total + n; }));
	};

	DefaultTestPopulation children;

	select(begin(population), end(population), count, fn, std::back_inserter(children));

	double a = ea::fitness::mean(begin(population), end(population), fn);
	double b = ea::fitness::mean(begin(children), end(children), fn);

	CPPUNIT_ASSERT(Compare()(b, a));
}

template<typename Selection>
static void is_subset(Selection select, const size_t size = 1000, const size_t count = 10)
{
	DefaultTestPopulation population;

	std::generate_n(std::back_inserter(population), size, [&]()
	{
		DefaultTestGenome g;

		ea::random::fill_n_int(std::back_inserter(g), 10, 1, 100);

		return g;
	});

	std::function<double(DefaultTestGenome::iterator, DefaultTestGenome::iterator)>
	fn = [](DefaultTestGenome::iterator first, DefaultTestGenome::iterator last)
	{
		return static_cast<double>(std::accumulate(first, last, 1, [](int factorial, int n) { return factorial * n; }));
	};

	DefaultTestPopulation children;

	select(begin(population), end(population), count, fn, std::back_inserter(children));

	std::for_each(begin(children), end(children), [&population](auto a)
	{
		auto match = std::find_if(begin(population), end(population), [&a](auto b)
		{
			bool equal = a.size() == b.size();
			size_t i = 0;

			while(equal && i < a.size())
			{
				equal = a[i] == b[i];
				++i;
			}

			return equal;
		});

		CPPUNIT_ASSERT(match != end(population));
	});
}

template<typename Error, typename Selection>
void select_error(Selection op, size_t size = 0, size_t count = 0)
{
	DefaultTestPopulation population;

	std::generate_n(std::back_inserter(population), size, [&]()
	{
		DefaultTestGenome g;

		ea::random::fill_n_int(std::back_inserter(g), 10, -100, 100);

		return g;
	});

	DefaultTestPopulation children; 

	std::function<double(DefaultTestGenome::iterator, DefaultTestGenome::iterator)>
	fn = [](DefaultTestGenome::iterator first, DefaultTestGenome::iterator last)
	{
		return 0;
	};

	CPPUNIT_ASSERT_THROW(op(begin(population), end(population), count, fn, std::back_inserter(children)), Error);
}

#include "TournamentSelection.hpp"

class TournamentSelectionTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(TournamentSelectionTest);
	CPPUNIT_TEST(select_children);
	CPPUNIT_TEST(fitness_increases);
	CPPUNIT_TEST(is_subset);
	CPPUNIT_TEST(invalid_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void select_children()
		{
			::select_children(ea::selection::Tournament<>());
		}

		void fitness_increases()
		{
			::fitness_increases(ea::selection::Tournament<>());
			::fitness_increases<std::less<double>>(ea::selection::Tournament<std::less<double>>());
		}

		void is_subset()
		{
			::is_subset(ea::selection::Tournament<>());
		}

		void invalid_args()
		{
			CPPUNIT_ASSERT_THROW(ea::selection::Tournament<>(0), std::invalid_argument);

			ea::selection::Tournament<> op(5);

			select_error<std::length_error>(op, 4, 3);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(TournamentSelectionTest);

#include "DoubleTournamentSelection.hpp"

class DoubleTournamentSelectionTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(DoubleTournamentSelectionTest);
	CPPUNIT_TEST(select_children);
	CPPUNIT_TEST(fitness_increases);
	CPPUNIT_TEST(is_subset);
	CPPUNIT_TEST(invalid_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void select_children()
		{
			::select_children(ea::selection::DoubleTournament<>());
		}

		void is_subset()
		{
			::is_subset(ea::selection::DoubleTournament<>());
		}

		void fitness_increases()
		{
			::fitness_increases(ea::selection::DoubleTournament<>());
			::fitness_increases<std::less<double>>(ea::selection::DoubleTournament<std::less<double>>());
		}

		void invalid_args()
		{
			CPPUNIT_ASSERT_THROW(ea::selection::DoubleTournament<>(0), std::invalid_argument);

			ea::selection::DoubleTournament<> op(5);

			select_error<std::length_error>(op, 4, 3);
			select_error<std::length_error>(op, 10, 11);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(DoubleTournamentSelectionTest);

#include "FittestSelection.hpp"

class FittestSelectionTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(FittestSelectionTest);
	CPPUNIT_TEST(select_children);
	CPPUNIT_TEST(fitness_increases);
	CPPUNIT_TEST(is_subset);
	CPPUNIT_TEST(invalid_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void select_children()
		{
			::select_children(ea::selection::Fittest<>());
		}

		void fitness_increases()
		{
			::fitness_increases(ea::selection::Fittest<>());
			::fitness_increases<std::less<double>>(ea::selection::Fittest<std::less<double>>());
		}

		void is_subset()
		{
			::is_subset(ea::selection::Fittest<>());
		}

		void invalid_args()
		{
			ea::selection::Fittest<> op;

			select_error<std::length_error>(op, 10, 11);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(FittestSelectionTest);

#include "FitnessProportionalSelection.hpp"

class FitnessProportionalSelection : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(FitnessProportionalSelection);
	CPPUNIT_TEST(select_children);
	CPPUNIT_TEST(fitness_increases);
	CPPUNIT_TEST(is_subset);
	CPPUNIT_TEST(invalid_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void select_children()
		{
			::select_children(ea::selection::FitnessProportional());
		}

		void fitness_increases()
		{
			::fitness_increases(ea::selection::FitnessProportional());
			::fitness_increases<std::less<double>>(ea::selection::FitnessProportional(ea::selection::Proportionality::inverse));
		}

		void is_subset()
		{
			::is_subset(ea::selection::FitnessProportional());
		}

		void invalid_args()
		{
			ea::selection::FitnessProportional op;

			select_error<std::length_error>(op, 0, 1);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(FitnessProportionalSelection);

#include "StochasticUniversalSampling.hpp"

class StochasticUniversalSamplingTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(StochasticUniversalSamplingTest);
	CPPUNIT_TEST(select_children);
	CPPUNIT_TEST(fitness_increases);
	CPPUNIT_TEST(is_subset);
	CPPUNIT_TEST(invalid_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void select_children()
		{
			::select_children(ea::selection::StochasticUniversalSampling());
		}

		void fitness_increases()
		{
			::fitness_increases(ea::selection::StochasticUniversalSampling());
		}

		void is_subset()
		{
			::is_subset(ea::selection::StochasticUniversalSampling());
		}

		void invalid_args()
		{
			ea::selection::StochasticUniversalSampling op;

			select_error<std::length_error>(op, 0, 1);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(StochasticUniversalSamplingTest);

#include "BitStringMutation.hpp"

class BitStringMutationTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(BitStringMutationTest);
	CPPUNIT_TEST(mutate);
	CPPUNIT_TEST(invalid_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void mutate()
		{
			std::vector<bool> a;

			ea::mutation::BitString()(begin(a), end(a));

			ea::random::fill_n_int(std::back_inserter(a), 100, 0, 1);

			std::vector<bool> b;

			std::copy(begin(a), end(a), std::back_inserter(b));

			ea::mutation::BitString()(begin(a), end(a));

			CPPUNIT_ASSERT(std::equal(begin(a), end(a), begin(b)) == false);
		}

		void invalid_args()
		{
			CPPUNIT_ASSERT_THROW(ea::mutation::BitString(-0.1), std::invalid_argument);
			CPPUNIT_ASSERT_THROW(ea::mutation::BitString(1.1), std::invalid_argument);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(BitStringMutationTest);

#include "DoubleSwapMutation.hpp"

class DoubleSwapMutationTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(DoubleSwapMutationTest);
	CPPUNIT_TEST(mutate);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void mutate()
		{
			DefaultTestGenome a(10);

			ea::random::fill_distinct_n_int(begin(a), 10, 0, 9);

			DefaultTestGenome b;

			CPPUNIT_ASSERT_THROW(ea::mutation::DoubleSwap()(begin(b), begin(b)), std::length_error);

			std::copy(begin(a), end(a), std::back_inserter(b));

			ea::mutation::DoubleSwap()(begin(a), end(a));

			size_t differences = 0;

			for(size_t i = 0; i < 10; ++i)
			{
				if(a[i] != b[i])
				{
					++differences;
				}
			}

			CPPUNIT_ASSERT(differences == 3);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(DoubleSwapMutationTest);

#include "InverseBitStringMutation.hpp"

class InverseBitStringMutationTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(InverseBitStringMutationTest);
	CPPUNIT_TEST(mutate);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void mutate()
		{
			std::vector<bool> a;

			ea::mutation::BitString()(begin(a), end(a));

			ea::random::fill_n_int(std::back_inserter(a), 100, 0, 1);

			std::vector<bool> b;

			std::copy(begin(a), end(a), std::back_inserter(b));

			ea::mutation::InverseBitString()(begin(a), end(a));

			for(size_t i = 0; i < 100; ++i)
			{
				CPPUNIT_ASSERT(a[i] != b[i]);
			}
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(InverseBitStringMutationTest);

#include "SingleBitStringMutation.hpp"

class SingleBitStringMutation : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(SingleBitStringMutation);
	CPPUNIT_TEST(mutate);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void mutate()
		{
			std::vector<bool> a;

			ea::mutation::BitString()(begin(a), end(a));

			ea::random::fill_n_int(std::back_inserter(a), 100, 0, 1);

			std::vector<bool> b;

			std::copy(begin(a), end(a), std::back_inserter(b));

			ea::mutation::SingleBitString()(begin(a), end(a));

			size_t differences = 0;

			for(size_t i = 0; i < 100; ++i)
			{
				if(a[i] != b[i])
				{
					++differences;
				}
			}

			CPPUNIT_ASSERT(differences = 1);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(SingleBitStringMutation);

#include "SingleSwapMutation.hpp"

class SingleSwapMutationTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(SingleSwapMutationTest);
	CPPUNIT_TEST(mutate);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void mutate()
		{
			DefaultTestGenome a(10);

			ea::random::fill_distinct_n_int(begin(a), 10, 0, 9);

			DefaultTestGenome b;

			CPPUNIT_ASSERT_THROW(ea::mutation::SingleSwap()(begin(b), begin(b)), std::length_error);

			std::copy(begin(a), end(a), std::back_inserter(b));

			ea::mutation::SingleSwap()(begin(a), end(a));

			size_t differences = 0;

			for(size_t i = 0; i < 10; ++i)
			{
				if(a[i] != b[i])
				{
					++differences;
				}
			}

			CPPUNIT_ASSERT(differences == 2);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(SingleSwapMutationTest);

template<typename Crossover>
void crossover(Crossover crossover,
               const size_t expected_size,
               const size_t count1,
               const size_t min1,
               const size_t max1,
               const size_t count2,
               const size_t min2,
               const size_t max2)
{
	DefaultTestGenome a(count1);

	ea::random::fill_distinct_n_int(begin(a), count1, min1, max1);

	DefaultTestGenome b(count2);

	ea::random::fill_distinct_n_int(begin(b), count2, min2, max2);

	DefaultTestPopulation offsprings;

	size_t n = crossover(begin(a), end(a), begin(b), end(b), std::back_inserter(offsprings));

	CPPUNIT_ASSERT(n == expected_size);
	CPPUNIT_ASSERT(offsprings.size() == expected_size);
}

#include "CutAndSpliceCrossover.hpp"

class CutAndSpliceCrossoverTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(CutAndSpliceCrossoverTest);
	CPPUNIT_TEST(crossover);
	CPPUNIT_TEST(invalid_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void crossover()
		{
			::crossover(ea::crossover::CutAndSplice<DefaultTestGenome>(), 2, 10, 0, 9, 10, 100, 109);
		}

		void invalid_args()
		{
			ea::crossover::CutAndSplice<DefaultTestGenome> op;

			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 2, 0, 9, 10, 100, 109), std::length_error);
			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 10, 0, 9, 2, 100, 109), std::length_error);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(CutAndSpliceCrossoverTest);

#include "CycleCrossover.hpp"

class CycleCrossoverTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(CycleCrossoverTest);
	CPPUNIT_TEST(crossover);
	CPPUNIT_TEST(invalid_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void crossover()
		{
			::crossover(ea::crossover::Cycle<DefaultTestGenome>(), 2, 10, 0, 9, 10, 0, 9);
		}

		void invalid_args()
		{
			ea::crossover::Cycle<DefaultTestGenome> op;

			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 2, 0, 9, 10, 100, 109), std::length_error);
			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 10, 0, 9, 2, 100, 109), std::length_error);
			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 10, 0, 9, 10, 100, 109), std::logic_error);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(CycleCrossoverTest);

#include "EdgeRecombinationCrossover.hpp"

class EdgeRecombinationCrossoverTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(EdgeRecombinationCrossoverTest);
	CPPUNIT_TEST(crossover);
	CPPUNIT_TEST(invalid_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void crossover()
		{
			::crossover(ea::crossover::EdgeRecombination<DefaultTestGenome>(), 1, 10, 0, 9, 10, 0, 9);
		}

		void invalid_args()
		{
			ea::crossover::EdgeRecombination<DefaultTestGenome> op;

			CPPUNIT_ASSERT_THROW(::crossover(op, 1, 2, 0, 9, 10, 0, 9), std::length_error);
			CPPUNIT_ASSERT_THROW(::crossover(op, 1, 10, 0, 9, 2, 0, 9), std::length_error);
			CPPUNIT_ASSERT_THROW(::crossover(op, 1, 10, 0, 9, 10, 100, 109), std::logic_error);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(EdgeRecombinationCrossoverTest);

#include "OnePointCrossover.hpp"

class OnePointCrossoverTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(OnePointCrossoverTest);
	CPPUNIT_TEST(crossover);
	CPPUNIT_TEST(invalid_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void crossover()
		{
			::crossover(ea::crossover::OnePoint<DefaultTestGenome>(), 2, 10, 0, 9, 10, 0, 9);
		}

		void invalid_args()
		{
			ea::crossover::OnePoint<DefaultTestGenome> op;

			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 0, 0, 9, 10, 0, 9), std::length_error);
			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 10, 0, 9, 0, 0, 9), std::length_error);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(OnePointCrossoverTest);

#include "OrderedCrossover.hpp"

class OrderedCrossoverTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(OrderedCrossoverTest);
	CPPUNIT_TEST(crossover);
	CPPUNIT_TEST(invalid_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void crossover()
		{
			::crossover(ea::crossover::Ordered<DefaultTestGenome>(), 2, 10, 0, 9, 10, 0, 9);
		}

		void invalid_args()
		{
			ea::crossover::Ordered<DefaultTestGenome> op;

			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 10, 0, 9, 2, 0, 9), std::length_error);
			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 2, 0, 9, 10, 0, 9), std::length_error);
			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 2, 0, 9, 10, 100, 109), std::logic_error);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(OrderedCrossoverTest);

#include "PMXCrossover.hpp"

class PMXCrossoverTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(PMXCrossoverTest);
	CPPUNIT_TEST(crossover);
	CPPUNIT_TEST(invalid_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void crossover()
		{
			::crossover(ea::crossover::PMX<DefaultTestGenome>(), 2, 10, 0, 9, 10, 0, 9);
		}

		void invalid_args()
		{
			ea::crossover::PMX<DefaultTestGenome> op;

			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 10, 0, 9, 2, 0, 9), std::length_error);
			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 2, 0, 9, 10, 0, 9), std::length_error);
			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 10, 0, 9, 10, 100, 109), std::logic_error);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(PMXCrossoverTest);

#include "TwoPointCrossover.hpp"

class TwoPointCrossoverTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(TwoPointCrossoverTest);
	CPPUNIT_TEST(crossover);
	CPPUNIT_TEST(invalid_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void crossover()
		{
			::crossover(ea::crossover::TwoPoint<DefaultTestGenome>(), 2, 10, 0, 9, 10, 0, 9);
		}

		void invalid_args()
		{
			ea::crossover::TwoPoint<DefaultTestGenome> op;

			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 10, 0, 9, 2, 0, 9), std::length_error);
			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 2, 0, 9, 10, 0, 9), std::length_error);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(TwoPointCrossoverTest);

#include "UniformCrossover.hpp"

class UniformCrossoverTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(UniformCrossoverTest);
	CPPUNIT_TEST(crossover);
	CPPUNIT_TEST(invalid_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void crossover()
		{
			::crossover(ea::crossover::Uniform<DefaultTestGenome>(), 2, 10, 0, 9, 10, 0, 9);
		}

		void invalid_args()
		{
			ea::crossover::Uniform<DefaultTestGenome> op;

			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 10, 0, 9, 2, 0, 9), std::length_error);
			CPPUNIT_ASSERT_THROW(::crossover(op, 2, 2, 0, 9, 10, 0, 9), std::length_error);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(UniformCrossoverTest);

#include "Diversity.hpp"

class DiversityTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(DiversityTest);
	CPPUNIT_TEST(hamming_distance);
	CPPUNIT_TEST(avg_hamming_distance);
	CPPUNIT_TEST(shannon_entropy);
	CPPUNIT_TEST(avg_shannon_entropy);
	CPPUNIT_TEST(substr_diversity);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void hamming_distance()
		{
			const std::string a = "foobar";
			const std::string b = "FooBar";
			const std::string c = "foo";

			CPPUNIT_ASSERT(ea::diversity::hamming_distance(begin(a), end(a), begin(b), end(b)) == 2);
			CPPUNIT_ASSERT_THROW(ea::diversity::hamming_distance(begin(a), end(a), begin(c), end(c)), std::length_error);
		}

		void avg_hamming_distance()
		{
			const std::string strings[3] =
			{
				"foobaR",
				"FooBar",
				"BarFoo"
			};

			const double distance = ea::diversity::avg_hamming_distance(begin(strings), end(strings));

			CPPUNIT_ASSERT(std::abs(distance - 5.0) < std::numeric_limits<double>::epsilon());
		}

		void shannon_entropy()
		{
			const std::string fnord = "if you can't see the fnord it can't eat you";
			const double entropy = ea::diversity::shannon_entropy<std::log2>(begin(fnord), end(fnord));
			const double rounded = static_cast<double>(static_cast<int>(entropy * 100)) / 100;

			CPPUNIT_ASSERT(rounded == 3.69);
		}

		void avg_shannon_entropy()
		{
			const std::string fnords[5] =
			{
				"fnord",
				"is",
				"the",
				"donut",
				"hole"
			};

			const double entropy = ea::diversity::avg_shannon_entropy<std::log2>(begin(fnords), end(fnords));
			const double rounded = static_cast<double>(static_cast<int>(entropy * 100)) / 100;

			CPPUNIT_ASSERT(rounded == 1.84);
		}

		void substr_diversity()
		{
			const std::vector<char> a = { 'h', 'e', 'l', 'l', 'o' };
			const std::vector<char> b = { 'w', 'o', 'r', 'l', 'd' };
			const std::vector<std::vector<char>> p = { a, b };

			const double diversity = ea::diversity::substr_diversity(begin(p), end(p));

			CPPUNIT_ASSERT(std::abs(diversity - (50.0 / 27)) < std::numeric_limits<double>::epsilon());
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(DiversityTest);

int main(int argc, char* argv[])
{
	CPPUNIT_NS::TestResult testresult;

	CPPUNIT_NS::TestResultCollector collectedresults;
	testresult.addListener (&collectedresults);

	CPPUNIT_NS::BriefTestProgressListener progress;
	testresult.addListener (&progress);

	CPPUNIT_NS::TestRunner testrunner;
	testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
	testrunner.run(testresult);

	CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
	compileroutputter.write ();

	return collectedresults.wasSuccessful() ? 0 : 1;
}

