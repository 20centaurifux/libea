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
#include <vector>
#include <algorithm>

#include "AnsiRandomNumberGenerator.hpp"
#include "TR1UniformDistribution.hpp"
#include <sstream>
#include "PrimitiveGenome.hpp"
#include "FittestSelection.hpp"
#include "FitnessProportionalSelection.hpp"
#include "TournamentSelection.hpp"
#include "DoubleTournamentSelection.hpp"

using namespace std;
using namespace CPPUNIT_NS;

/*
 *	classes & methods shared by test cases:
 */

// convert array of genes to a sequence:
template<typename TGenomeBase>
typename TGenomeBase::sequence_type array_to_sequence(TGenomeBase& base, const typename TGenomeBase::gene_type* genes, const int32_t size)
{
	auto seq = base.create(10);

	for(uint32_t i = 0; i < size; i++)
	{
		base.set(seq, i, genes[i]);
	}

	return seq;
}

// fitness functor templates
template<typename TSequence>
class TestFitness
{
	public:
		float operator()(const TSequence* const& seq) const
		{
			float f = 0;

			for(auto i = 0; i < seq->len; i++)
			{
				f += seq->genes[i] * i;
			}

			return f;
		}
};

template<typename TSequence>
class TestStringFitness
{
	public:
		float operator()(const TSequence* const& seq) const
		{
			float f = 0;

			for(auto i = 0; i < seq->len; i++)
			{
				for(char c : seq->genes[i])
				{
					f += i * c;
				}
			}

			return f;
		}
};

/*
 * factory classes
 *   -generate_differing_gene_sets() is required by GenomeBaseTest
 *   -generate_gene_set() is required by SelectionOperatorTest
 */
class Int32Factory
{
	public:
		void generate_differing_gene_sets(int32_t* genes[2], uint32_t size, int32_t& not_in_set)
		{
			uint32_t i;
			uint32_t sep;

			genes[0] = new int32_t[size];
			_rnd.get_unique_int32_seq(0, size - 1, genes[0], size);

			genes[1] = new int32_t[size];

			sep = size / 2;

			for(i = 0; i < sep; i++)
			{
				genes[1][i] = genes[0][i + sep];
			}

			for(i = sep; i < size; i++)
			{
				genes[1][i] = genes[0][i - sep];
			}

			not_in_set = size;
		}

		void generate_gene_set(int32_t* genes, const int32_t len)
		{
			_rnd.get_unique_int32_seq(0, len - 1, genes, len);
		}

	private:
		ea::TR1UniformDistribution<> _rnd;
};

class DoubleFactory
{
	public:
		void generate_differing_gene_sets(double* genes[2], uint32_t size, double& not_in_set)
		{
			uint32_t i;
			uint32_t sep;

			genes[0] = new double[size];
			_rnd.get_unique_double_seq(0, size - 1, genes[0], size);

			genes[1] = new double[size];

			sep = size / 2;

			for(i = 0; i < sep; i++)
			{
				genes[1][i] = genes[0][i + sep];
			}

			for(i = sep; i < size; i++)
			{
				genes[1][i] = genes[0][i - sep];
			}

			not_in_set = size;
		}

	private:
		ea::TR1UniformDistribution<> _rnd;
};

class StringFactory
{
	public:
		void generate_differing_gene_sets(string* genes[2], uint32_t size, string& not_in_set)
		{
			genes[0] = new string[size];
			genes[1] = new string[size];

			for(uint32_t i = 0; i < size; i++)
			{
				std::ostringstream sstream;

				sstream << "[gene-" << i << "]";
				std::string gene = sstream.str();

				genes[0][i] = gene;
				genes[1][size - i - 1] = gene;
			}

			not_in_set = "[not in set]";
		}
};


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

//CPPUNIT_TEST_SUITE_REGISTRATION(AnsiRandomNumberGeneratorTest);
//CPPUNIT_TEST_SUITE_REGISTRATION(TR1UniformDistributionTest);

/*
 *	genome tests:
 */
template<typename TGenomeBase, typename Factory>
class GenomeBaseTest : public CPPUNIT_NS::TestFixture
{
	typedef GenomeBaseTest<TGenomeBase, Factory> GenomeTestCaseType;
	CPPUNIT_TEST_SUITE(GenomeTestCaseType);
	CPPUNIT_TEST(test_genome_base);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_genome_base()
		{
			Factory factory;
			typename TGenomeBase::gene_type* genes[2];
			typename TGenomeBase::gene_type not_in_set;
			typename TGenomeBase::sequence_type sequences[4];
			uint32_t i;

			for(uint32_t size = 10; size < 10000; size *= 10)
			{
				/* create test sequences with same length and genes, but differing
				   order and hashes */
				factory.generate_differing_gene_sets(genes, size, not_in_set);

				sequences[0] = _base.create(size);
				sequences[1] = _base.create(size);

				for(i = 0; i < size; i++)
				{
					_base.set(sequences[0], i, genes[0][i]);
					_base.set(sequences[1], i, genes[1][i]);
				}

				for(i = 0; i < 2; i++)
				{
					// test length:
					CPPUNIT_ASSERT(_base.len(sequences[i]) == size);

					// get genes from sequence and compare them to genes of generated gene array:
					test_get(sequences[i], genes[i], size);
	
					// test index_of method:
					test_index_of(sequences[i], genes[i], size, not_in_set);

					// compare sequences:
					CPPUNIT_ASSERT(_base.hash(sequences[0]) != _base.hash(sequences[1]));
					CPPUNIT_ASSERT(_base.fitness(sequences[0]) != _base.fitness(sequences[1]));
					CPPUNIT_ASSERT(_base.cmp(sequences[0], sequences[1]) != 0);

					// copy sequence:
					auto clone = _base.copy(sequences[1]);

					// compare sequence with clone:
					CPPUNIT_ASSERT(_base.len(sequences[1]) == _base.len(clone));
					CPPUNIT_ASSERT(_base.cmp(sequences[1], clone) == 0);
					CPPUNIT_ASSERT(_base.hash(sequences[1]) == _base.hash(clone));
					CPPUNIT_ASSERT(_base.fitness(sequences[1]) == _base.fitness(clone));

					// change clone & compare it again to sequence:
					auto a = _base.get(clone, 0);
					auto b = _base.get(clone, size - 1);

					_base.set(clone, size - 1, a);
					_base.set(clone, 0, b);

					CPPUNIT_ASSERT(_base.len(sequences[1]) == _base.len(clone));
					CPPUNIT_ASSERT(_base.cmp(sequences[1], clone) != 0);
					CPPUNIT_ASSERT(_base.hash(sequences[1]) != _base.hash(clone));
					CPPUNIT_ASSERT(_base.fitness(sequences[1]) != _base.fitness(clone));

					// destroy clone:
					_base.dispose(clone);
				}

				// clean up:
				for(i = 0; i < 2; i++)
				{
					_base.dispose(sequences[i]);
					delete[] genes[i];
				}
			}
		}

	private:
		TGenomeBase _base;

		void test_get(typename TGenomeBase::sequence_type& sequence, const typename TGenomeBase::gene_type *genes, const int32_t len)
		{
			CPPUNIT_ASSERT(_base.len(sequence) == len);

			for(uint32_t i = 0; i < len; i++)
			{
				CPPUNIT_ASSERT(_base.get(sequence, i) == genes[i]);
			}
		}

		void test_index_of(typename TGenomeBase::sequence_type& sequence, const typename TGenomeBase::gene_type *genes, const int32_t len, const typename TGenomeBase::gene_type not_in_sequence)
		{
			CPPUNIT_ASSERT(_base.len(sequence) == len);

			for(uint32_t i = 0; i < len; i++)
			{
				CPPUNIT_ASSERT(_base.index_of(sequence, genes[i]) == i);
			}

			CPPUNIT_ASSERT(_base.index_of(sequence, not_in_sequence) == -1);
		}
};

typedef ea::Int32PGenomeBase<TestFitness<ea::Sequence<int32_t>>> PrimitiveInt32GenomeBase;
typedef GenomeBaseTest<PrimitiveInt32GenomeBase, Int32Factory> PrimitiveInt32GenomeBaseTest;

typedef ea::Int32CPGenomeBase<TestFitness<ea::CSequence<int32_t>>> CachedPrimitiveInt32GenomeBase;
typedef GenomeBaseTest<CachedPrimitiveInt32GenomeBase, Int32Factory> CachedPrimitiveInt32GenomeBaseTest;

typedef ea::DoublePGenomeBase<TestFitness<ea::Sequence<double>>> PrimitiveDoubleGenomeBase;
typedef GenomeBaseTest<PrimitiveDoubleGenomeBase, DoubleFactory> PrimitiveDoubleGenomeBaseTest;

typedef ea::DoubleCPGenomeBase<TestFitness<ea::Sequence<double>>> CachedPrimitiveDoubleGenomeBase;
typedef GenomeBaseTest<CachedPrimitiveDoubleGenomeBase, DoubleFactory> CachedPrimitiveDoubleGenomeBaseTest;

typedef ea::StringPGenomeBase<TestStringFitness<ea::Sequence<std::string>>> PrimitiveStringGenomeBase;
typedef GenomeBaseTest<PrimitiveStringGenomeBase, StringFactory> PrimitiveStringGenomeBaseTest;

typedef ea::StringCPGenomeBase<TestStringFitness<ea::Sequence<std::string>>> CachedPrimitiveStringGenomeBase;
typedef GenomeBaseTest<CachedPrimitiveStringGenomeBase, StringFactory> CachedPrimitiveStringGenomeBaseTest;

//CPPUNIT_TEST_SUITE_REGISTRATION(PrimitiveInt32GenomeBaseTest);
//CPPUNIT_TEST_SUITE_REGISTRATION(CachedPrimitiveInt32GenomeBaseTest);
//CPPUNIT_TEST_SUITE_REGISTRATION(PrimitiveDoubleGenomeBaseTest);
//CPPUNIT_TEST_SUITE_REGISTRATION(CachedPrimitiveDoubleGenomeBaseTest);
//CPPUNIT_TEST_SUITE_REGISTRATION(PrimitiveStringGenomeBaseTest);
//CPPUNIT_TEST_SUITE_REGISTRATION(CachedPrimitiveStringGenomeBaseTest);

/*
 *	selection operators:
 */
template<typename TGenomeBase, typename TFactory, typename TSelection>
class SelectionOperatorTest : public CPPUNIT_NS::TestFixture
{
	typedef SelectionOperatorTest<TGenomeBase, TFactory, TSelection> SelectionOperatorTestCaseType;
	CPPUNIT_TEST_SUITE(SelectionOperatorTestCaseType);
	CPPUNIT_TEST(test_selection);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_selection()
		{
			TFactory f;
			TGenomeBase base;
			TSelection sel;
			typename TGenomeBase::gene_type genes[10];
			std::vector<typename TGenomeBase::sequence_type> population;

			// create population:
			for(uint32_t i = 0; i < 100; i++)
			{
				f.generate_gene_set(genes, 10);
				auto seq = array_to_sequence(base, genes, 10);
				population.push_back(seq);
			}

			// create input adapter for population vector:
			auto input = ea::make_input_adapter(population);

			// create index vector & corresponding output adapter:
			std::vector<uint32_t> indexes;
			auto inserter = std::back_inserter(indexes);
			ea::STLVectorAdapter<uint32_t> output(inserter);

			// select genomes:
			sel.select(input, 80, output);

			// test selection:
			CPPUNIT_ASSERT(indexes.size() == 80);

			std::for_each(begin(indexes), end(indexes), [](const uint32_t index)
			{
				CPPUNIT_ASSERT(index >= 0 && index < 100);
			});

			// cleanup:
			ea::dispose(base, begin(population), end(population));
		}
};

typedef SelectionOperatorTest<PrimitiveInt32GenomeBase,
        Int32Factory,
        ea::FittestSelection<PrimitiveInt32GenomeBase>> FittestPrimitiveInt32GenomeBaseSelection;

typedef SelectionOperatorTest<CachedPrimitiveInt32GenomeBase,
        Int32Factory,
        ea::FittestSelection<CachedPrimitiveInt32GenomeBase>> FittestCachedPrimitiveInt32GenomeBaseSelection;

typedef SelectionOperatorTest<PrimitiveInt32GenomeBase,
        Int32Factory,
        ea::FitnessProportionalSelection<PrimitiveInt32GenomeBase>> FitnessProportionalPrimitiveInt32GenomeBaseSelection;

typedef SelectionOperatorTest<CachedPrimitiveInt32GenomeBase,
        Int32Factory,
        ea::FitnessProportionalSelection<CachedPrimitiveInt32GenomeBase>> FitnessProportionalCachedPrimitiveInt32GenomeBaseSelection;

typedef SelectionOperatorTest<PrimitiveInt32GenomeBase,
        Int32Factory,
        ea::AlignedFitnessProportionalSelection<PrimitiveInt32GenomeBase>> AlignedFitnessProportionalPrimitiveInt32GenomeBaseSelection;

typedef SelectionOperatorTest<CachedPrimitiveInt32GenomeBase,
        Int32Factory,
        ea::AlignedFitnessProportionalSelection<CachedPrimitiveInt32GenomeBase>> AlignedFitnessProportionalCachedPrimitiveInt32GenomeBaseSelection;

typedef SelectionOperatorTest<PrimitiveInt32GenomeBase,
        Int32Factory,
        ea::MinimizingFitnessProportionalSelection<PrimitiveInt32GenomeBase>> MinimizingFitnessProportionalPrimitiveInt32GenomeBaseSelection;

typedef SelectionOperatorTest<CachedPrimitiveInt32GenomeBase,
        Int32Factory,
        ea::MinimizingFitnessProportionalSelection<CachedPrimitiveInt32GenomeBase>> MinimizingFitnessProportionalCachedPrimitiveInt32GenomeBaseSelection;

typedef SelectionOperatorTest<PrimitiveInt32GenomeBase,
        Int32Factory,
        ea::TournamentSelection<PrimitiveInt32GenomeBase>> TournamentPrimitiveInt32GenomeBaseSelection;

typedef SelectionOperatorTest<CachedPrimitiveInt32GenomeBase,
        Int32Factory,
        ea::TournamentSelection<CachedPrimitiveInt32GenomeBase>> TournamentCachedPrimitiveInt32GenomeBaseSelection;

typedef SelectionOperatorTest<PrimitiveInt32GenomeBase,
        Int32Factory,
        ea::DoubleTournamentSelection<PrimitiveInt32GenomeBase>> DoubleTournamentPrimitiveInt32GenomeBaseSelection;

typedef SelectionOperatorTest<CachedPrimitiveInt32GenomeBase,
        Int32Factory,
        ea::DoubleTournamentSelection<CachedPrimitiveInt32GenomeBase>> DoubleTournamentCachedPrimitiveInt32GenomeBaseSelection;

CPPUNIT_TEST_SUITE_REGISTRATION(FittestPrimitiveInt32GenomeBaseSelection);
CPPUNIT_TEST_SUITE_REGISTRATION(FittestCachedPrimitiveInt32GenomeBaseSelection);
CPPUNIT_TEST_SUITE_REGISTRATION(FitnessProportionalPrimitiveInt32GenomeBaseSelection);
CPPUNIT_TEST_SUITE_REGISTRATION(FitnessProportionalCachedPrimitiveInt32GenomeBaseSelection);
CPPUNIT_TEST_SUITE_REGISTRATION(AlignedFitnessProportionalPrimitiveInt32GenomeBaseSelection);
CPPUNIT_TEST_SUITE_REGISTRATION(AlignedFitnessProportionalCachedPrimitiveInt32GenomeBaseSelection);
CPPUNIT_TEST_SUITE_REGISTRATION(MinimizingFitnessProportionalPrimitiveInt32GenomeBaseSelection);
CPPUNIT_TEST_SUITE_REGISTRATION(MinimizingFitnessProportionalCachedPrimitiveInt32GenomeBaseSelection);
CPPUNIT_TEST_SUITE_REGISTRATION(TournamentPrimitiveInt32GenomeBaseSelection);
CPPUNIT_TEST_SUITE_REGISTRATION(TournamentCachedPrimitiveInt32GenomeBaseSelection);
CPPUNIT_TEST_SUITE_REGISTRATION(DoubleTournamentPrimitiveInt32GenomeBaseSelection);
CPPUNIT_TEST_SUITE_REGISTRATION(DoubleTournamentCachedPrimitiveInt32GenomeBaseSelection);

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
