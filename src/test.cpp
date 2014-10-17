/// @cond INTERNAL
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include <random>
#include <set>
#include <algorithm>

#include "AnsiRandomNumberGenerator.hpp"
#include "TR1UniformDistribution.hpp"

using namespace std;
using namespace CPPUNIT_NS;

/*
 *	test random number generators:
 */
template<typename TRandom>
class RandomNumberGeneratorTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(RandomNumberGeneratorTest<TRandom>);
	CPPUNIT_TEST(test_int32);
	CPPUNIT_TEST(test_double);
	CPPUNIT_TEST(test_int32_seq);
	CPPUNIT_TEST(test_double_seq);
	CPPUNIT_TEST(test_unique_int32_seq);
	CPPUNIT_TEST(test_unique_double_seq);

	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_int32()
		{
			static TRandom g;
			int32_t step;

			typedef struct
			{
				int32_t operator()(int32_t min, int32_t max) { return g.get_int32(min, max); }
			} GetInt32;

			CPPUNIT_ASSERT(g.get_min_int32() < g.get_max_int32());

			for(uint32_t i = 0; i < 1000000; i++)
			{
				uint32_t n = g.get_int32();

				CPPUNIT_ASSERT(n >= g.get_min_int32() && n <= g.get_max_int32());
			}

			test_range<int32_t, GetInt32>(g.get_min_int32(), g.get_max_int32());
			test_range<int32_t, GetInt32>(g.get_min_int32(), g.get_min_int32() + 1);
			test_range<int32_t, GetInt32>(g.get_max_int32() - 1, g.get_max_int32());

			if((step = (g.get_max_int32() - g.get_min_int32()) / 10))
			{
				for(int32_t m = g.get_min_int32(); m < g.get_max_int32() - step; m += step)
				{
					test_range<int32_t, GetInt32>(m, m + step);
				}
			}
		}

		void test_double()
		{
			static TRandom g;

			typedef struct
			{
				double operator()(double min, double max) { return g.get_double(min, max); }
			} GetDouble;

			CPPUNIT_ASSERT(g.get_min_double() < g.get_max_double());

			for(uint32_t i = 0; i < 1000000; i++)
			{
				double n = g.get_double();

				CPPUNIT_ASSERT(n >= g.get_min_double() && n <= g.get_max_double());
			}

			test_range<double, GetDouble>(g.get_min_double(), g.get_max_double());
			test_range<double, GetDouble>(g.get_min_double(), g.get_min_double() + 1);
			test_range<double, GetDouble>(g.get_max_double() - 1, g.get_max_double());

			if(g.get_max_double() - g.get_min_double() >= 2.0)
			{
				for(double m = -1.0; m < 1.0; m += 0.1)
				{
					test_range<double, GetDouble>(m, 1.0);
				}
			}
		}

		void test_int32_seq()
		{
			TRandom g;
			int32_t seq[4096];

			g.get_int32_seq(-42, 42, seq, 4096);

			for_each(begin(seq), end(seq), [](int32_t n)
			{
				CPPUNIT_ASSERT(n >= -42 && n <= 42);
			});
		}

		void test_double_seq()
		{
			TRandom g;
			double seq[4096];

			g.get_double_seq(-42.42, 42.42, seq, 4096);

			for_each(begin(seq), end(seq), [](double n)
			{
				CPPUNIT_ASSERT(n >= -42.42 && n <= 42.42);
			});
		}

		void test_unique_int32_seq()
		{
			TRandom g;
			int32_t seq[10240];

			g.get_unique_int32_seq(-5120, 5119, seq, 10240);

			for(uint32_t i = -5120; i < 5119; i++)
			{
				auto iter = find(begin(seq), end(seq), i);
				CPPUNIT_ASSERT(iter != end(seq));
			}
		}

		void test_unique_double_seq()
		{
			TRandom g;
			double seq[10240];
			set<double> set;

			g.get_unique_double_seq(-10.0, 10.0, seq, 10240);

			for_each(begin(seq), end(seq), [&set](double v)
			{
				set.insert(v);
			});

			CPPUNIT_ASSERT_EQUAL(set.size(), (size_t)10240);
		}

	private:
		template<typename T, typename F>
		void test_range(const T min, const T max)
		{
			F f;

			for(uint32_t i = 0; i < 1000000; i++)
			{
				T n = f(min, max);
				CPPUNIT_ASSERT(n >= min && n <= max);
			}
		}
};

typedef RandomNumberGeneratorTest<ea::AnsiRandomNumberGenerator> AnsiRandomNumberGeneratorTest;
typedef RandomNumberGeneratorTest<ea::TR1UniformDistribution<mt19937_64>> TR1UniformDistributionTest;

CPPUNIT_TEST_SUITE_REGISTRATION(AnsiRandomNumberGeneratorTest);
CPPUNIT_TEST_SUITE_REGISTRATION(TR1UniformDistributionTest);

int main(int argc, char* argv[])
{
	// run test cases:
	TestResult result;

	TestResultCollector collector;
	result.addListener(&collector);

	BriefTestProgressListener progress;
	result.addListener(&progress);

	TestRunner runner;
	runner.addTest(TestFactoryRegistry::getRegistry().makeTest());
	runner.run(result);

	CompilerOutputter output(&collector, cerr);
	output.write();

	return collector.wasSuccessful() ? 0 : 1;
}
/// @endcond
