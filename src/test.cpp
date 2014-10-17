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

#include "AnsiRandomNumberGenerator.h"
#include "TR1UniformDistribution.h"

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
	/*
	CPPUNIT_TEST(test_double);
	CPPUNIT_TEST(test_int32_seq);
	CPPUNIT_TEST(test_double_seq);
	CPPUNIT_TEST(test_unique_int32_seq);
	CPPUNIT_TEST(test_unique_double_seq);
*/
	CPPUNIT_TEST_SUITE_END();

	protected:
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

		typedef int32_t(*foof)(const int32_t, const int32_t);

		void test_int32()
		{
			static TRandom g;

			typedef struct
			{
				int32_t operator()(int32_t min, int32_t max) { return g.get_int32(min, max); }
			} GetInt32;

			test_range<int32_t, GetInt32>(g.get_min_int32(), g.get_max_int32());
			test_range<int32_t, GetInt32>(g.get_min_int32(), g.get_min_int32() + 1);
			test_range<int32_t, GetInt32>(g.get_max_int32() - 1, g.get_max_int32());
			test_range<int32_t, GetInt32>(-10, 0);
			test_range<int32_t, GetInt32>(0, 10);
			test_range<int32_t, GetInt32>(-10, 10);
		}

		void test_double()
		{
			static TRandom g;

			typedef struct
			{
				double operator()(double min, double max) { return g.get_double(min, max); }
			} GetDouble;

			test_range<int32_t, GetDouble>(-1, 1);
			test_range<int32_t, GetDouble>(-2000000.130, -2000000.11);
			test_range<int32_t, GetDouble>(777, 778);
		}

		void test_int32_seq()
		{
			TRandom g;
			int32_t seq[100];

			g.get_int32_seq(7, 111, seq, 100);

			for_each(begin(seq), end(seq), [](int32_t n) { CPPUNIT_ASSERT(n >= 7 && n <= 111); });
		}

		void test_double_seq()
		{
			TRandom g;
			double seq[100];

			g.get_double_seq(9.0, 9.97, seq, 100);

			for_each(begin(seq), end(seq), [](double n) { CPPUNIT_ASSERT(n >= 9.0 && n <= 9.97); });
		}

		void test_unique_int32_seq()
		{
			TRandom g;
			int32_t seq[100];

			g.get_unique_int32_seq(0, 99, seq, 100);

			for(uint32_t i = 0; i < 100; i++)
			{
				auto iter = find(begin(seq), end(seq), i);
				CPPUNIT_ASSERT(iter != end(seq));
			}
		}

		void test_unique_double_seq()
		{
			TRandom g;
			double seq[100];
			set<double> set;

			g.get_unique_double_seq(0, 10.0, seq, 100);

			for_each(begin(seq), end(seq), [&set](double v) { set.insert(v); });
			CPPUNIT_ASSERT_EQUAL(set.size(), (size_t)100);
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
