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
	CPPUNIT_TEST(fill_n_test_size);
	CPPUNIT_TEST(fill_n_test_range);
	CPPUNIT_TEST(fill_n_test_distribution);
	CPPUNIT_TEST(fill_distinct_n_test_size);
	CPPUNIT_TEST(fill_distinct_n_test_range);
	CPPUNIT_TEST(fill_distinct_n_test_uniqueness);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void fill_n_test_size()
		{
			std::vector<int> numbers(MAX_NUMBERS);

			ea::random::fill_n(begin(numbers), MAX_NUMBERS, -100, 100);

			CPPUNIT_ASSERT_EQUAL(MAX_NUMBERS, numbers.size());
		}

		void fill_n_test_range()
		{
			std::vector<int> numbers(MAX_NUMBERS);

			ea::random::fill_n(begin(numbers), MAX_NUMBERS, -100, 100);

			std::for_each(begin(numbers), end(numbers), [](const int n)
			{
				CPPUNIT_ASSERT(n >= -100 && n <= 100);
			});
		}

		void fill_n_test_distribution()
		{
			std::vector<int> numbers(MAX_NUMBERS);

			ea::random::fill_n(begin(numbers), MAX_NUMBERS, -100, 100);

			for(int i = -100; i <= 100; ++i)
			{
				CPPUNIT_ASSERT(std::any_of(begin(numbers), end(numbers), [i](const int n)
				{
					return n == i;
				}));
			}
		}

		void fill_distinct_n_test_size()
		{
			std::vector<int> numbers(MAX_DISTINCT_NUMBERS);

			ea::random::fill_distinct_n(begin(numbers), MAX_DISTINCT_NUMBERS, -MAX_DISTINCT_NUMBERS, MAX_DISTINCT_NUMBERS);

			CPPUNIT_ASSERT_EQUAL((size_t)MAX_DISTINCT_NUMBERS, numbers.size());
		}

		void fill_distinct_n_test_range()
		{
			std::vector<int> numbers(MAX_DISTINCT_NUMBERS);

			ea::random::fill_n(begin(numbers), MAX_DISTINCT_NUMBERS, -MAX_DISTINCT_NUMBERS, MAX_DISTINCT_NUMBERS);

			std::for_each(begin(numbers), end(numbers), [&](const int n)
			{
				CPPUNIT_ASSERT(n >= -MAX_DISTINCT_NUMBERS && n <= MAX_DISTINCT_NUMBERS);
			});
		}

		void fill_distinct_n_test_uniqueness()
		{
			std::vector<int> numbers(MAX_DISTINCT_NUMBERS);

			ea::random::fill_distinct_n(begin(numbers), MAX_DISTINCT_NUMBERS, -MAX_DISTINCT_NUMBERS, MAX_DISTINCT_NUMBERS);

			std::vector<int> unique;

			std::unique_copy(begin(numbers), end(numbers), std::back_inserter(unique));

			CPPUNIT_ASSERT_EQUAL((size_t)MAX_DISTINCT_NUMBERS, unique.size());
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
		using Genome = std::vector<int>;
		using Population = std::vector<Genome>;

		void setUp()
		{
			std::generate_n(std::back_inserter(population), POPULATION_SIZE, [&]()
			{
				Genome g(GENOME_SIZE);

				ea::random::fill_n(begin(g), GENOME_SIZE, 1, 1024);

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
			compare_fitness_functions(ea::fitness::fitness_by_index<Population::iterator, Genome::iterator>(fitness));
		}

		void memoized_fitness_by_index()
		{
			compare_fitness_functions(ea::fitness::memoize_fitness_by_index<Population::iterator, Genome::iterator>(fitness));
		}

	private:
		const size_t POPULATION_SIZE = 4096;
		const size_t GENOME_SIZE = 64;

		Population population;

		static double fitness(const Genome::iterator first, const Genome::iterator last)
		{
			double sum = 0.0;
			size_t count = 0;

			std::for_each(first, last, [&sum, &count](const int n)
			{
				sum += n;
				++count;
			});

			return (double)sum / count;
		}

		void compare_fitness_functions(std::function<double(const Population::iterator, const size_t index)> fn)
		{
			size_t i = 0;

			std::for_each(begin(population), end(population), [&](Genome g)
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
		using Genome = std::vector<int>;
		using Population = std::vector<Genome>;
		using Fitness = std::function<double(Genome::iterator, Genome::iterator)>;

	protected:
		void mean()
		{
			Population population;
			size_t size = 1;

			std::generate_n(std::back_inserter(population), 10, [&size]()
			{
				Genome g;

				for(size_t i = 0; i < size; ++i)
				{
					g.push_back(i + 1);
				}

				++size;

				return g;
			});

			auto fn = [](const Genome::iterator first, const Genome::iterator last)
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
			test_empty_set_returns_nan(ea::fitness::mean<Population::iterator, Fitness>);
		}

		void median()
		{
			Population population;

			ea::random::RandomEngine engine = ea::random::default_engine();
			std::uniform_int_distribution<> dist(1, 100);

			std::generate_n(std::back_inserter(population), 8192, [&dist, &engine]()
			{
				size_t length = 100;

				if(dist(engine) <= 20)
				{
					length /= 10;
				}

				return Genome(length);
			});

			double median = ea::fitness::median(begin(population), end(population), std::distance<Genome::iterator>);

			CPPUNIT_ASSERT(std::abs(median - 100) < std::numeric_limits<double>::epsilon());
		}

		void median_empty()
		{
			test_empty_set_returns_nan(ea::fitness::median<Population::iterator, Fitness>);
		}

		void fittest()
		{
			Population population;

			size_t length = 1;

			std::generate_n(std::back_inserter(population), 100, [&length]()
			{
				return Genome(length++);
			});
	
			std::generate_n(std::back_inserter(population), 100, [&length]()
			{
				return Genome(--length);
			});

			auto fittest = ea::fitness::fittest(begin(population), end(population), std::distance<Genome::iterator>);

			CPPUNIT_ASSERT(fittest->size() == 100);
		}

		void fittest_empty()
		{
			Population population;

			auto pos = ea::fitness::fittest(begin(population), end(population), std::distance<Genome::iterator>);

			CPPUNIT_ASSERT(pos == end(population));
		}

	private:
		template<typename Fn>
		void test_empty_set_returns_nan(Fn fn)
		{
			Population population;

			double value = fn(begin(population), end(population), std::distance<Genome::iterator>);

			CPPUNIT_ASSERT(std::fpclassify(value) == FP_NAN);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(FitnessUtilitiesTest);

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

