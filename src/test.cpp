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
#include <sstream>

#include "libea.hpp"

using namespace std;
using namespace CPPUNIT_NS;

/*
 *	classes & methods shared by test cases:
 */
// convert array of genes to a sequence:
template<typename TGenomeBase>
typename TGenomeBase::sequence_type array_to_sequence(TGenomeBase& base, const typename TGenomeBase::gene_type* genes, const ea::sequence_len_t size)
{
	auto seq = base.create(10);

	for(ea::sequence_len_t i = 0; i < size; ++i)
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
		double operator()(const TSequence* const& seq) const
		{
			double f = 0;

			for(auto i = 0; i < seq->len; ++i)
			{
				f += seq->genes[i] * i;
			}

			return f;
		}
};

template<typename TSequence>
class TestBinaryFitness
{
	public:
		double operator()(const TSequence* const& seq) const
		{
			double f = 0;

			for(auto i = 0; i < seq->len; ++i)
			{
				if(seq->genes[i])
				{
					++f;
				}
			}

			return f;
		}
};

template<typename TSequence>
class TestStringFitness
{
	public:
		double operator()(const TSequence* const& seq) const
		{
			double f = 0;

			for(auto i = 0; i < seq->len; ++i)
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

			for(i = 0; i < sep; ++i)
			{
				genes[1][i] = genes[0][i + sep];
			}

			for(i = sep; i < size; ++i)
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

			for(i = 0; i < sep; ++i)
			{
				genes[1][i] = genes[0][i + sep];
			}

			for(i = sep; i < size; ++i)
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

			for(uint32_t i = 0; i < size; ++i)
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

// fake random number generator:
class FixedSeqRndGenerator : public ea::ARandomNumberGenerator
{
	public:
		FixedSeqRndGenerator(std::initializer_list<int32_t> ints)
		{
			for(int32_t v : ints)
			{
				_int32s.push_back(v);
			};

			_int32_iter = begin(_int32s);
		}

		int32_t get_int32() override
		{
			int32_t v = *_int32_iter;

			_int32_iter_next();

			return v;
		}

		int32_t get_int32(const int32_t min, const int32_t max) override
		{
			uint32_t count = 0;
			int32_t v;

			do
			{
				v = get_int32();

				if(v >= min && v <= max)
				{
					return v;
				}
			} while(count <= _int32s.size());

			abort();
		}

		double get_double() override
		{
			abort();
		}

		double get_double(const double min, const double max) override
		{
			abort();
		}

		inline int32_t get_max_int32() const override
		{
			return std::numeric_limits<int32_t>::max();
		}

		inline int32_t get_min_int32() const override
		{
			return std::numeric_limits<int32_t>::min();
		}

		double get_max_double() const override
		{
			abort();
		}

		double get_min_double() const override
		{
			abort();
		}

		void get_int32_seq(const int32_t min, const int32_t max, int32_t* numbers, const std::size_t length) override
		{
			if(length > _int32s.size())
			{
				abort();
			}

			for(std::size_t i = 0; i < length; ++i)
			{
				numbers[i] = _int32s[i];
			}
		}

		void get_double_seq(const double min, const double max, double* numbers, const std::size_t length)
		{
			abort();
		}

		void get_unique_int32_seq(const int32_t min, const int32_t max, int32_t* numbers, const std::size_t length) override
		{
			get_int32_seq(min, max, numbers, length);
		}

		void get_unique_double_seq(const double min, const double max, double* numbers, const std::size_t length) override
		{
			abort();
		}

	private:
		std::vector<int32_t> _int32s;
		std::vector<int32_t>::iterator _int32_iter;

		void _int32_iter_next()
		{
			++_int32_iter;

			if(_int32_iter == end(_int32s))
			{
				_int32_iter = begin(_int32s);
			}
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

			for(uint32_t i = 0; i < 1000000; ++i)
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

			for(uint32_t i = 0; i < 1000000; ++i)
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

			for(uint32_t i = -5120; i < 5119; ++i)
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

			for(uint32_t i = 0; i < 1000000; ++i)
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

				for(i = 0; i < size; ++i)
				{
					_base.set(sequences[0], i, genes[0][i]);
					_base.set(sequences[1], i, genes[1][i]);
				}

				for(i = 0; i < 2; ++i)
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
				for(i = 0; i < 2; ++i)
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

			for(int32_t i = 0; i < len; ++i)
			{
				CPPUNIT_ASSERT(_base.get(sequence, i) == genes[i]);
			}
		}

		void test_index_of(typename TGenomeBase::sequence_type& sequence, const typename TGenomeBase::gene_type *genes, const int32_t len, const typename TGenomeBase::gene_type not_in_sequence)
		{
			CPPUNIT_ASSERT(_base.len(sequence) == len);

			for(int32_t i = 0; i < len; ++i)
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

CPPUNIT_TEST_SUITE_REGISTRATION(PrimitiveInt32GenomeBaseTest);
CPPUNIT_TEST_SUITE_REGISTRATION(CachedPrimitiveInt32GenomeBaseTest);
CPPUNIT_TEST_SUITE_REGISTRATION(PrimitiveDoubleGenomeBaseTest);
CPPUNIT_TEST_SUITE_REGISTRATION(CachedPrimitiveDoubleGenomeBaseTest);
CPPUNIT_TEST_SUITE_REGISTRATION(PrimitiveStringGenomeBaseTest);
CPPUNIT_TEST_SUITE_REGISTRATION(CachedPrimitiveStringGenomeBaseTest);

/*
 *	test algorithms:
 */
class AlgorithmTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(AlgorithmTest);
	CPPUNIT_TEST(test_multiset_check);
	CPPUNIT_TEST(test_seq_equality_check);
	CPPUNIT_TEST(test_add);
	CPPUNIT_TEST(test_sub);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_multiset_check()
		{
			PrimitiveInt32GenomeBase base;
			auto seq = base.create(10);
			uint32_t i;

			for(i = 0; i < 10; ++i)
			{
				base.set(seq, i, i);
			}

			CPPUNIT_ASSERT(!ea::is_multiset<PrimitiveInt32GenomeBase>(seq));

			base.set(seq, 9, 0);

			CPPUNIT_ASSERT(ea::is_multiset<PrimitiveInt32GenomeBase>(seq));

			base.dispose(seq);
		}

		void test_seq_equality_check()
		{
			PrimitiveInt32GenomeBase base;
			auto seq_a = base.create(10);
			uint32_t i;

			for(i = 0; i < 10; ++i)
			{
				base.set(seq_a, i, i);
			}

			auto seq_b = base.copy(seq_a);

			CPPUNIT_ASSERT(ea::set_equals<PrimitiveInt32GenomeBase>(seq_a, seq_b));

			base.set(seq_b, 9, 11);

			CPPUNIT_ASSERT(!ea::set_equals<PrimitiveInt32GenomeBase>(seq_a, seq_b));

			base.dispose(seq_a);
			base.dispose(seq_b);
		}

		void test_add()
		{
			int32_t sum = 0;

			sum = ea::chk_add({std::numeric_limits<int32_t>::min(), 1, 13, 64, 22344534, -633175});

			CPPUNIT_ASSERT(sum == std::numeric_limits<int32_t>::min() + 1 + 13 + 64 + 22344534 + -633175);
		}

		void test_sub()
		{
			int32_t diff = 0;

			diff = ea::chk_sub({std::numeric_limits<int32_t>::max(), 246546352, -647456, 5463212});

			CPPUNIT_ASSERT(diff == std::numeric_limits<int32_t>::max() - 246546352 - -647456 - 5463212);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(AlgorithmTest);

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
			for(uint32_t i = 0; i < 100; ++i)
			{
				f.generate_gene_set(genes, 10);
				auto seq = array_to_sequence(base, genes, 10);
				population.push_back(seq);
			}

			// create input adapter for population vector:
			auto input = ea::make_input_adapter(population);

			// create index vector & corresponding output adapter:
			std::vector<std::size_t> indexes;
			auto output = ea::make_output_adapter(indexes);

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

typedef SelectionOperatorTest<PrimitiveInt32GenomeBase,
        Int32Factory,
        ea::StochasticUniversalSampling<PrimitiveInt32GenomeBase>> StochasticPrimitiveInt32GenomeBaseUniversalSampling;

typedef SelectionOperatorTest<CachedPrimitiveInt32GenomeBase,
        Int32Factory,
        ea::StochasticUniversalSampling<CachedPrimitiveInt32GenomeBase>> StochasticCachedPrimitiveInt32GenomeBaseUniversalSampling;

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
CPPUNIT_TEST_SUITE_REGISTRATION(StochasticPrimitiveInt32GenomeBaseUniversalSampling);
CPPUNIT_TEST_SUITE_REGISTRATION(StochasticCachedPrimitiveInt32GenomeBaseUniversalSampling);

/*
 *	crossover operators:
 */
class CutAndSpliceOperatorTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(CutAndSpliceOperatorTest);
	CPPUNIT_TEST(test_crossover);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_crossover()
		{
			PrimitiveInt32GenomeBase base;
			auto rnd = std::shared_ptr<FixedSeqRndGenerator>(new FixedSeqRndGenerator({3, 6}));
			ea::CutAndSpliceCrossover<PrimitiveInt32GenomeBase> c(rnd);
			int32_t i;
			int32_t j;
			int32_t sep1 = 0;
			int32_t sep2;

			// create parent sequences:
			auto a = base.create(10);
			auto b = base.create(10);

			for(i = 0; i < 10; ++i)
			{
				base.set(a, i, i);
				base.set(b, i, i + 10);
			}

			// create children:
			std::vector<ea::Sequence<int32_t>*> children;
			auto output = ea::make_output_adapter(children);

			uint32_t count = c.crossover(a, b, output);

			// test number of created children:
			CPPUNIT_ASSERT(count == 2);

			// find separators:
			for(i = 0; i < base.len(children[0]); ++i)
			{
				if(base.get(children[0], i) != base.get(a, i))
				{
					sep1 = i;
					break;
				}
			}

			sep2 = base.get(children[0], sep1) - 10;

			CPPUNIT_ASSERT(sep1 == 3);
			CPPUNIT_ASSERT(sep2 == 6);

			// validate child sequences:
			for(i = 0; i < sep1; ++i)
			{
				CPPUNIT_ASSERT(base.get(children[0], i) == base.get(a, i));
			}

			j = sep1;

			for(i = sep2; j < base.len(children[0]); ++i)
			{
				CPPUNIT_ASSERT(base.get(children[0], j++) == base.get(b, i));
			}

			for(i = 0; i < sep2; ++i)
			{
				CPPUNIT_ASSERT(base.get(children[1], i) == base.get(b, i));
			}

			j = sep2;

			for(i = sep1; j < base.len(children[1]); ++i)
			{
				CPPUNIT_ASSERT(base.get(children[1], j++) == base.get(a, i));
			}

			base.dispose(a);
			base.dispose(b);

			ea::dispose(base, begin(children), end(children));
		}
};

class CycleCrossoverTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(CycleCrossoverTest);
	CPPUNIT_TEST(test_crossover);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_crossover()
		{
			PrimitiveInt32GenomeBase base;
			ea::CycleCrossover<PrimitiveInt32GenomeBase> c;
			const int32_t seq_a[10] = { 8, 4, 7, 3, 6, 2, 5, 1, 9, 0 };
			const int32_t seq_b[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			const int32_t seq_child_a[10] = { 8, 1, 2, 3, 4, 5, 6, 7, 9, 0 };
			const int32_t seq_child_b[10] = { 0, 4, 7, 3, 6, 2, 5, 1, 8, 9 };
			int32_t i;

			// create parent sequences:
			auto a = base.create(10);
			auto b = base.create(10);

			for(i = 0; i < 10; ++i)
			{
				base.set(a, i, seq_a[i]);
				base.set(b, i, seq_b[i]);
			}

			// create children:
			std::vector<ea::Sequence<int32_t>*> children;
			auto output = ea::make_output_adapter(children);

			uint32_t count = c.crossover(a, b, output);

			// validate child sequences:
			CPPUNIT_ASSERT(count == 2);

			for(i = 0; i < 10; ++i)
			{
				CPPUNIT_ASSERT(base.get(children[0], i) ==  seq_child_a[i]);
				CPPUNIT_ASSERT(base.get(children[1], i) ==  seq_child_b[i]);
			}

			base.dispose(a);
			base.dispose(b);
			ea::dispose(base, begin(children), end(children));
		}
};

class EdgeRecombinationCrossoverTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(EdgeRecombinationCrossoverTest);
	CPPUNIT_TEST(test_crossover);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_crossover()
		{
			PrimitiveInt32GenomeBase base;
			auto rnd = std::shared_ptr<FixedSeqRndGenerator>(new FixedSeqRndGenerator({1}));
			ea::EdgeRecombinationCrossover<PrimitiveInt32GenomeBase> c(rnd);
			const int32_t seq_a[7] = { 0, 1, 2, 3, 4, 5, 6 };
			const int32_t seq_b[7] = { 5, 2, 0, 1, 6, 4, 3 };
			const int32_t seq_child[7] = { 0, 2, 5, 6, 4, 3, 1 };
			int32_t i;

			// create parent sequences:
			auto a = base.create(7);
			auto b = base.create(7);

			for(i = 0; i < 7; ++i)
			{
				base.set(a, i, seq_a[i]);
				base.set(b, i, seq_b[i]);
			}

			// create children:
			std::vector<ea::Sequence<int32_t>*> children;
			auto output = ea::make_output_adapter(children);

			uint32_t count = c.crossover(a, b, output);

			// validate child sequences:
			CPPUNIT_ASSERT(count == 1);

			for(i = 0; i < 7; ++i)
			{
				CPPUNIT_ASSERT(base.get(children[0], i) == seq_child[i]);
			}

			base.dispose(a);
			base.dispose(b);
			ea::dispose(base, begin(children), end(children));
		}
};

class OnePointCrossoverTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(OnePointCrossoverTest);
	CPPUNIT_TEST(test_crossover);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_crossover()
		{
			PrimitiveInt32GenomeBase base;
			auto rnd = std::shared_ptr<FixedSeqRndGenerator>(new FixedSeqRndGenerator({5}));
			ea::OnePointCrossover<PrimitiveInt32GenomeBase> c(rnd);
			int32_t i;

			// create parent sequences:
			auto a = base.create(10);
			auto b = base.create(10);

			for(i = 0; i < 10; ++i)
			{
				base.set(a, i, i);
				base.set(b, i, i + 10);
			}

			// create children:
			std::vector<ea::Sequence<int32_t>*> children;
			auto output = ea::make_output_adapter(children);

			uint32_t count = c.crossover(a, b, output);

			// validate child sequences:
			CPPUNIT_ASSERT(count == 2);

			for(i = 0; i < 5; ++i)
			{
				CPPUNIT_ASSERT(base.get(children[0], i) == i);
				CPPUNIT_ASSERT(base.get(children[1], i) == i + 10);
			}

			for(i = 5; i < 10; ++i)
			{
				CPPUNIT_ASSERT(base.get(children[0], i) == i + 10);
				CPPUNIT_ASSERT(base.get(children[1], i) == i);
			}

			base.dispose(a);
			base.dispose(b);

			ea::dispose(base, begin(children), end(children));
		}
};

class OrderedCrossoverTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(OrderedCrossoverTest);
	CPPUNIT_TEST(test_crossover);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_crossover()
		{
			PrimitiveInt32GenomeBase base;
			auto rnd = std::shared_ptr<FixedSeqRndGenerator>(new FixedSeqRndGenerator({5}));
			ea::OrderedCrossover<PrimitiveInt32GenomeBase> c(rnd);
			int32_t i;

			// create parent sequences:
			auto a = base.create(10);
			auto b = base.create(10);

			for(i = 0; i < 10; ++i)
			{
				base.set(a, i, 9 - i);
				base.set(b, i, i);
			}

			// create child:
			std::vector<ea::Sequence<int32_t>*> children;
			auto output = ea::make_output_adapter(children);

			uint32_t count = c.crossover(a, b, output);

			// validate child:
			CPPUNIT_ASSERT(count == 1);

			for(i = 0; i < 5; ++i)
			{
				CPPUNIT_ASSERT(base.get(a, i) == base.get(children[0], i));
			}

			for(i = 0; i < 5; ++i)
			{
				CPPUNIT_ASSERT(i == base.get(children[0], i + 5));
			}

			base.dispose(a);
			base.dispose(b);

			ea::dispose(base, begin(children), end(children));
		}
};

class PMXCrossoverTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(PMXCrossoverTest);
	CPPUNIT_TEST(test_crossover);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_crossover()
		{
			PrimitiveInt32GenomeBase base;
			auto rnd = std::shared_ptr<FixedSeqRndGenerator>(new FixedSeqRndGenerator({4, 5}));
			ea::PMXCrossover<PrimitiveInt32GenomeBase> c(rnd);
			int32_t i;
			const int32_t seq_child_a[10] = { 9, 1, 2, 3, 5, 0, 8, 7, 6, 4 };
			const int32_t seq_child_b[10] = { 0, 8, 7, 6, 4, 9, 1, 2, 3, 5 };

			// create parent sequences:
			auto a = base.create(10);
			auto b = base.create(10);

			for(i = 0; i < 5; ++i)
			{
				base.set(a, i, 9 - i);
				base.set(a, i + 5, i);
				base.set(b, i, i);
				base.set(b, i + 5, 9 - i);
			}

			// create child:
			std::vector<ea::Sequence<int32_t>*> children;
			auto output = ea::make_output_adapter(children);

			uint32_t count = c.crossover(a, b, output);

			// validate children:
			CPPUNIT_ASSERT(count == 2);

			for(uint32_t i = 0; i < 10; ++i)
			{
				CPPUNIT_ASSERT(base.get(children[0], i) == seq_child_a[i]);
				CPPUNIT_ASSERT(base.get(children[1], i) == seq_child_b[i]);
			}

			base.dispose(a);
			base.dispose(b);

			ea::dispose(base, begin(children), end(children));
		}
};

class TwoPointCrossoverTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(TwoPointCrossoverTest);
	CPPUNIT_TEST(test_crossover);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_crossover()
		{
			PrimitiveInt32GenomeBase base;
			auto rnd = std::shared_ptr<FixedSeqRndGenerator>(new FixedSeqRndGenerator({5, 6}));
			ea::TwoPointCrossover<PrimitiveInt32GenomeBase> c(rnd);
			const int32_t seq_child_a[10] = { 10, 11, 12, 13, 14, 5, 16, 17, 18, 19 };
			const int32_t seq_child_b[10] = { 0, 1, 2, 3, 4, 15, 6, 7, 8, 9 };
			int32_t i;

			// create parent sequences:
			auto a = base.create(10);
			auto b = base.create(10);

			for(i = 0; i < 10; ++i)
			{
				base.set(a, i, i);
				base.set(b, i, i + 10);
			}

			// create children:
			std::vector<ea::Sequence<int32_t>*> children;
			auto output = ea::make_output_adapter(children);

			uint32_t count = c.crossover(a, b, output);

			// validate child sequences:
			CPPUNIT_ASSERT(count == 2);

			for(i = 0; i < 9; ++i)
			{
				CPPUNIT_ASSERT(base.get(children[0], i) == seq_child_a[i]);
				CPPUNIT_ASSERT(base.get(children[1], i) == seq_child_b[i]);
			}

			base.dispose(a);
			base.dispose(b);

			ea::dispose(base, begin(children), end(children));
		}
};

class UniformCrossoverTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(UniformCrossoverTest);
	CPPUNIT_TEST(test_crossover);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_crossover()
		{
			PrimitiveInt32GenomeBase base;
			auto rnd = std::shared_ptr<FixedSeqRndGenerator>(new FixedSeqRndGenerator({1, 2}));
			ea::UniformCrossover<PrimitiveInt32GenomeBase> c(rnd);
			const int32_t seq_child_a[10] = { 0, 11, 2, 13, 4, 15, 6, 17, 8, 19 };
			const int32_t seq_child_b[10] = { 10, 1, 12, 3, 14, 5, 16, 7, 18, 9 };
			int32_t i;

			// create parent sequences:
			auto a = base.create(10);
			auto b = base.create(10);

			for(i = 0; i < 10; ++i)
			{
				base.set(a, i, i);
				base.set(b, i, i + 10);
			}

			// create children:
			std::vector<ea::Sequence<int32_t>*> children;
			auto output = ea::make_output_adapter(children);

			uint32_t count = c.crossover(a, b, output);

			// validate child sequences:
			CPPUNIT_ASSERT(count == 2);

			for(i = 0; i < 9; ++i)
			{
				CPPUNIT_ASSERT(base.get(children[0], i) == seq_child_a[i]);
				CPPUNIT_ASSERT(base.get(children[1], i) == seq_child_b[i]);
			}

			base.dispose(a);
			base.dispose(b);

			ea::dispose(base, begin(children), end(children));
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(CutAndSpliceOperatorTest);
CPPUNIT_TEST_SUITE_REGISTRATION(CycleCrossoverTest);
CPPUNIT_TEST_SUITE_REGISTRATION(EdgeRecombinationCrossoverTest);
CPPUNIT_TEST_SUITE_REGISTRATION(OnePointCrossoverTest);
CPPUNIT_TEST_SUITE_REGISTRATION(OrderedCrossoverTest);
CPPUNIT_TEST_SUITE_REGISTRATION(PMXCrossoverTest);
CPPUNIT_TEST_SUITE_REGISTRATION(TwoPointCrossoverTest);
CPPUNIT_TEST_SUITE_REGISTRATION(UniformCrossoverTest);

/*
 *	mutation operators:
 */
typedef ea::BinaryPGenomeBase<TestBinaryFitness<ea::Sequence<bool>>> PrimitiveBinaryGenomeBase;

class SingleSwapMutationTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(SingleSwapMutationTest);
	CPPUNIT_TEST(test_mutation);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_mutation()
		{
			PrimitiveInt32GenomeBase base;
			auto rnd = std::shared_ptr<FixedSeqRndGenerator>(new FixedSeqRndGenerator({6, 7}));
			ea::SingleSwapMutation<PrimitiveInt32GenomeBase> m(rnd);
			int32_t mutant[10] = {0, 1, 2, 3, 4, 5, 7, 6, 8, 9};
			int32_t i;

			auto gene = base.create(10);

			for(i = 0; i < 10; ++i)
			{
				base.set(gene, i, i);
			}

			m.mutate(gene);

			for(i = 0; i < base.len(gene); ++i)
			{
				CPPUNIT_ASSERT(base.get(gene, i) == mutant[i]);
			}

			base.dispose(gene);
		}
};

class DoubleSwapMutationTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(DoubleSwapMutationTest);
	CPPUNIT_TEST(test_mutation);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_mutation()
		{
			PrimitiveInt32GenomeBase base;
			auto rnd = std::shared_ptr<FixedSeqRndGenerator>(new FixedSeqRndGenerator({1, 2, 3}));
			ea::DoubleSwapMutation<PrimitiveInt32GenomeBase> m(rnd);
			int32_t mutant[10] = {0, 2, 3, 1, 4, 5, 6, 7, 8, 9};
			int32_t i;

			auto gene = base.create(10);

			for(i = 0; i < 10; ++i)
			{
				base.set(gene, i, i);
			}

			m.mutate(gene);

			for(i = 0; i < base.len(gene); ++i)
			{
				CPPUNIT_ASSERT(base.get(gene, i) == mutant[i]);
			}

			base.dispose(gene);
		}
};

class BitStringMutationTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(BitStringMutationTest);
	CPPUNIT_TEST(test_mutation);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_mutation()
		{
			PrimitiveBinaryGenomeBase base;
			auto rnd = std::shared_ptr<FixedSeqRndGenerator>(new FixedSeqRndGenerator({1, 1, 100, 20, 20, 70, 40, 40, 60}));
			ea::BitStringMutation<PrimitiveBinaryGenomeBase> m(rnd);
			bool child[9] = {false, false, true, false, false, true, false, false, true};
			int32_t i;

			auto gene = base.create(9);

			for(i = 0; i < 9; ++i)
			{
				base.set(gene, i, true);
			}

			m.mutate(gene);

			for(i = 0; i < 9; ++i)
			{
				CPPUNIT_ASSERT(base.get(gene, i) == child[i]);
			}

			base.dispose(gene);
		}
};

class SingleBitStringMutationTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(SingleBitStringMutationTest);
	CPPUNIT_TEST(test_mutation);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_mutation()
		{
			PrimitiveBinaryGenomeBase base;
			auto rnd = std::shared_ptr<FixedSeqRndGenerator>(new FixedSeqRndGenerator({3}));
			ea::SingleBitStringMutation<PrimitiveBinaryGenomeBase> m(rnd);
			int32_t i;

			auto gene = base.create(10);

			for(i = 0; i < 10; ++i)
			{
				base.set(gene, i, true);
			}

			m.mutate(gene);

			for(i = 0; i < 10; ++i)
			{
				CPPUNIT_ASSERT(base.get(gene, i) == (i != 3));
			}

			base.dispose(gene);
		}
};

class InverseBitStringMutationTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(InverseBitStringMutationTest);
	CPPUNIT_TEST(test_mutation);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_mutation()
		{
			PrimitiveBinaryGenomeBase base;
			ea::InverseBitStringMutation<PrimitiveBinaryGenomeBase> m;
			int32_t i;

			auto gene = base.create(10);

			for(i = 0; i < 10; ++i)
			{
				base.set(gene, i, i % 2);
			}

			m.mutate(gene);

			for(i = 0; i < 10; ++i)
			{
				CPPUNIT_ASSERT(base.get(gene, i) == !(i % 2));
			}

			base.dispose(gene);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(SingleSwapMutationTest);
CPPUNIT_TEST_SUITE_REGISTRATION(DoubleSwapMutationTest);
CPPUNIT_TEST_SUITE_REGISTRATION(BitStringMutationTest);
CPPUNIT_TEST_SUITE_REGISTRATION(SingleBitStringMutationTest);
CPPUNIT_TEST_SUITE_REGISTRATION(InverseBitStringMutationTest);

/*
 * pipeline:
 */
class PipelineTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(PipelineTest);
	CPPUNIT_TEST(test_pipeline);
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_pipeline()
		{
			PrimitiveInt32GenomeBase base;
			std::vector<ea::Sequence<int32_t>*> population;
			auto rnd = std::make_shared<ea::AnsiRandomNumberGenerator>();

			// create population:
			for(uint32_t i = 0; i < 100; ++i)
			{
				auto g = base.create(20);

				int32_t genes[20];
				rnd->get_unique_int32_seq(0, 19, genes, 20);

				for(uint32_t i = 0; i < 20; ++i)
				{
					base.set(g, i, i);
				}

				population.push_back(g);
			}

			// create and process pipeline:
			auto source = ea::make_input_adapter(population);

			std::vector<ea::Sequence<int32_t>*> children;
			auto output = ea::make_output_adapter(children);

			auto selection_a =
				ea::Pipeline::SelectionElement<PrimitiveInt32GenomeBase,
				                               ea::Pipeline::SourceDivisor<PrimitiveInt32GenomeBase>>
					(new ea::TournamentSelection<PrimitiveInt32GenomeBase>());

			auto selection_b =
				ea::Pipeline::SelectionElement<PrimitiveInt32GenomeBase,
				                               ea::Pipeline::FixedSelectionSize<PrimitiveInt32GenomeBase, 50>>
					(new ea::DoubleTournamentSelection<PrimitiveInt32GenomeBase>());

			auto crossover =
				ea::Pipeline::CrossoverElement<PrimitiveInt32GenomeBase>
					(new ea::CutAndSpliceCrossover<PrimitiveInt32GenomeBase>());

			auto mutation =
				ea::Pipeline::MutationElement<PrimitiveInt32GenomeBase>
					(new ea::SingleSwapMutation<PrimitiveInt32GenomeBase>(), rnd);

			auto terminator = ea::Pipeline::ForLoopTerminator<PrimitiveInt32GenomeBase>(10);

			ea::Pipeline::ITerminator<PrimitiveInt32GenomeBase>& terminator_ref = terminator;

			uint32_t sum = ea::Pipeline::pipeline_process<PrimitiveInt32GenomeBase>
				(source, output, { &selection_a, &crossover, &mutation, &selection_b }, terminator_ref);

			CPPUNIT_ASSERT_EQUAL(sum, 50u);

			// clean up:
			ea::dispose(base, begin(population), end(population));
			ea::dispose(base, begin(children), end(children));
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(PipelineTest);

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
