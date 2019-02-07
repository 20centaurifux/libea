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
			compare_fitness_functions(ea::fitness::fitness_by_index<DefaultTestPopulation::iterator, DefaultTestGenome::iterator>(fitness));
		}

		void memoized_fitness_by_index()
		{
			compare_fitness_functions(ea::fitness::memoize_fitness_by_index<DefaultTestPopulation::iterator, DefaultTestGenome::iterator>(fitness));
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

			auto fn = [](const DefaultTestGenome::iterator first, const DefaultTestGenome::iterator last)
			{
				double factorial = 1;

				std::for_each(first, last, [&factorial](const int n)
				{
					factorial *= n;
				});

				return factorial;
			};

			double mean = ea::fitness::mean(begin(population), end(population), fn);

			CPPUNIT_ASSERT(std::abs(mean - 403791.3) < std::numeric_limits<double>::epsilon());
		}

		void mean_empty()
		{
			test_empty_set_returns_nan(ea::fitness::mean<DefaultTestPopulation::iterator, Fitness>);
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

			double median = ea::fitness::median(begin(population), end(population), std::distance<DefaultTestGenome::iterator>);

			CPPUNIT_ASSERT(std::abs(median - 100) < std::numeric_limits<double>::epsilon());
		}

		void median_empty()
		{
			test_empty_set_returns_nan(ea::fitness::median<DefaultTestPopulation::iterator, Fitness>);
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

template<typename Compare = std::greater<double>, typename Selection>
static void default_selection_test(Selection select)
{
	DefaultTestPopulation population;

	std::generate_n(std::back_inserter(population), 10000, [&]()
	{
		DefaultTestGenome g;

		ea::random::fill_n_int(std::back_inserter(g), 10, 1, 100);

		return g;
	});

	std::function<double(DefaultTestGenome::iterator, DefaultTestGenome::iterator)>
	fn = [](DefaultTestGenome::iterator first, DefaultTestGenome::iterator last)
	{
		return static_cast<double>(std::accumulate(first, last, 0, [](int total, int n) { return total + n; }));
	};

	DefaultTestPopulation children;

	select(begin(population), end(population), 100, fn, std::back_inserter(children));

	CPPUNIT_ASSERT(children.size() == 100);

	double a = ea::fitness::mean(begin(population), end(population), fn);
	double b = ea::fitness::mean(begin(children), end(children), fn);

	CPPUNIT_ASSERT(Compare()(b, a));
}

#include "TournamentSelection.hpp"

class TournamentSelectionTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(TournamentSelectionTest);
	CPPUNIT_TEST(default_selection_test);
	CPPUNIT_TEST(invalid_args);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void default_selection_test()
		{
			::default_selection_test<>(ea::selection::Tournament<DefaultTestPopulation::iterator>());
			::default_selection_test<std::less<double>>(ea::selection::Tournament<DefaultTestPopulation::iterator, std::less<double>>());
		}

		void invalid_args()
		{
			CPPUNIT_ASSERT_THROW(ea::selection::Tournament<DefaultTestPopulation::iterator>(0), std::invalid_argument);

			ea::selection::Tournament<DefaultTestPopulation::iterator> op(5);

			DefaultTestPopulation population;
			DefaultTestPopulation children; 

			std::function<double(DefaultTestGenome::iterator, DefaultTestGenome::iterator)>
			fn = [](DefaultTestGenome::iterator first, DefaultTestGenome::iterator last)
			{
				return 0;
			};

			CPPUNIT_ASSERT_THROW(op(begin(population), end(population), 3, fn, std::back_inserter(children)), std::length_error);
			CPPUNIT_ASSERT_THROW(op(begin(population), end(population), 10, fn, std::back_inserter(children)), std::length_error);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(TournamentSelectionTest);
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

