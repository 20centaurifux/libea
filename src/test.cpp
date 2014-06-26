/// @cond INTERNAL
#include <cppunit/extensions/HelperMacros.h>

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include <memory>
#include <vector>

#include "PrimitiveGenome.h"
#include "Genome.h"
#include "AGene.h"
#include "AFactory.h"

#include "AnsiRandomNumberGenerator.h"
#include "TR1UniformDistribution.h"

#include "CutAndSpliceCrossover.h"
#include "CycleCrossover.h"
#include "EdgeRecombinationCrossover.h"
#include "OnePointCrossover.h"
#include "OrderedCrossover.h"
#include "PMXCrossover.h"
#include "TwoPointCrossover.h"
#include "UniformCrossover.h"

#include "DoubleTournamentSelection.h"
#include "FitnessProportionalSelection.h"
#include "FittestSelection.h"
#include "StochasticUniversalSampling.h"
#include "TournamentSelection.h"

#include "DoubleSwapMutation.h"
#include "SingleSwapMutation.h"

#include "BitStringMutation.h"
#include "InverseBitStringMutation.h"
#include "SingleBitStringMutation.h"

#include "SDBMHash.h"

using namespace std;
using namespace ea;
using namespace CPPUNIT_NS;

/*
 *	helpers:
 */
template<typename LessThan, typename TGenome>
bool compare_genomes(const TGenome& a, const TGenome& b)
{
	LessThan lt;

	for(uint32_t i = 0; i < a->size(); i++)
	{
		if(lt(a->at(i), b->at(i)) || lt(b->at(i), a->at(i)))
		{
			return false;
		}
	}

	return true;
}

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
	CPPUNIT_TEST_SUITE_END();

	protected:
		void test_int32()
		{
			TRandom g;
			int32_t n;

			for(int32_t i = 0; i < 1000; i++)
			{
				n = g.get_int32();
				CPPUNIT_ASSERT(n <= g.get_max_int32());
			}

			for(int32_t i = 0; i < 1000; i++)
			{
				n = g.get_int32(5, 10);
				CPPUNIT_ASSERT(n >= 5 && n <= 10);
			}
		}

		void test_double()
		{
			TRandom g;
			double n;

			for(int32_t i = 0; i < 1000; i++)
			{
				n = g.get_int32();
				CPPUNIT_ASSERT(n <= g.get_max_double());
			}

			for(int32_t i = 0; i < 1000; i++)
			{
				n = g.get_double(2, 3);
				CPPUNIT_ASSERT(n >= 2 && n <= 3);
			}
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
};

typedef RandomNumberGeneratorTest<AnsiRandomNumberGenerator> AnsiRandomNumberGeneratorTest;
typedef RandomNumberGeneratorTest<TR1UniformDistribution<mt19937_64>> TR1UniformDistributionTest;

CPPUNIT_TEST_SUITE_REGISTRATION(AnsiRandomNumberGeneratorTest);
CPPUNIT_TEST_SUITE_REGISTRATION(TR1UniformDistributionTest);

/*
 *	crossover operator test functions::
 */
typedef enum
{
	COMPARE_SIZE_SUMS   = 1,
	COMPARE_CHILD_SIZES = 2
} TestCrossoverFlags;

template<typename TContainer>
void compare_genome_size_sums(TContainer& parent, TContainer& children, const uint32_t count)
{
	uint32_t psize = 0;
	uint32_t csize = 0;

	for(uint32_t i = 0; i < count; i++)
	{
		psize += parent[i]->size();
		csize += children[i]->size();
	}

	CPPUNIT_ASSERT_EQUAL(psize, csize);
}

template<typename TCrossover, typename TGenome>
void test_crossover_operator(TCrossover& crossover, vector<shared_ptr<TGenome>>& population,
                             const uint32_t result_size, int32_t flags = 0)
{
	vector<shared_ptr<TGenome>> children;
	uint32_t result;
	uint32_t actual = 0;

	CPPUNIT_ASSERT(population.size() >= 2);

	result = crossover.crossover(population[0], population[1], children);

	CPPUNIT_ASSERT_EQUAL(result_size, result);

	if(flags & COMPARE_SIZE_SUMS)
	{
		compare_genome_size_sums(population, children, 1);
	}

	if(flags & COMPARE_CHILD_SIZES)
	{
		for(uint32_t i = 0; i < result - 1; i++)
		{
			CPPUNIT_ASSERT_EQUAL(children[i]->size(), children[i + 1]->size());
		}
	}

	children.clear();

	result = crossover.crossover(begin(population), end(population), children);

	for(uint32_t i = 0; i < population.size(); i++)
	{
		for(uint32_t j = i + 1; j < population.size(); j++)
		{
			actual += result_size;
		}
	}

	CPPUNIT_ASSERT_EQUAL(actual, result);
	CPPUNIT_ASSERT_EQUAL(result, (uint32_t)children.size());
}

template<typename TGenome, typename TRandom>
void test_cut_and_splice_crossover(TRandom& g, vector<shared_ptr<TGenome>>& population)
{
	auto c = CutAndSpliceCrossover<TGenome>(g);

	test_crossover_operator(c, population, 2);
}

template<typename TGenome, typename LessThan = less<typename TGenome::value_type>, typename TRandom>
void test_cycle_crossover(TRandom& g, vector<shared_ptr<TGenome>>& population)
{
	auto c = CycleCrossover<TGenome, LessThan>(g);

	test_crossover_operator(c, population, 2, COMPARE_SIZE_SUMS|COMPARE_CHILD_SIZES);
}

template<typename TGenome,  typename LessThan = less<typename TGenome::value_type>, typename TRandom>
void test_edge_recombination(TRandom& g, vector<shared_ptr<TGenome>>& population)
{
	auto c = EdgeRecombinationCrossover<TGenome, LessThan>(g);

	test_crossover_operator(c, population, 1, COMPARE_SIZE_SUMS);
}

template<typename TGenome, typename TRandom>
void test_one_point_crossover(TRandom& g, vector<shared_ptr<TGenome>>& population)
{
	auto c = OnePointCrossover<TGenome>(g);

	test_crossover_operator(c, population, 2, COMPARE_SIZE_SUMS|COMPARE_CHILD_SIZES);
}

template<typename TGenome,  typename LessThan = ::less<typename TGenome::value_type>, typename TRandom>
void test_ordered_crossover(TRandom& g, vector<shared_ptr<TGenome>>& population)
{
	auto c = OrderedCrossover<TGenome, LessThan>(g);

	test_crossover_operator(c, population, 1, COMPARE_SIZE_SUMS);
}

template<typename TGenome,  typename LessThan = less<typename TGenome::value_type>, typename TRandom>
void test_pmx_crossover(TRandom& g, vector<shared_ptr<TGenome>>& population)
{
	auto c = PMXCrossover<TGenome, LessThan>(g);

	test_crossover_operator(c, population, 2, COMPARE_SIZE_SUMS|COMPARE_CHILD_SIZES);
}

template<typename TGenome, typename TRandom>
void test_twopoint_crossover(TRandom& g, vector<shared_ptr<TGenome>>& population)
{
	auto c = TwoPointCrossover<TGenome>(g);

	test_crossover_operator(c, population, 2, COMPARE_SIZE_SUMS|COMPARE_CHILD_SIZES);
}

template<typename TGenome, typename TRandom>
void test_uniform_crossover(TRandom& g, vector<shared_ptr<TGenome>>& population)
{
	auto c = UniformCrossover<TGenome>(g);

	test_crossover_operator(c, population, 2, COMPARE_SIZE_SUMS|COMPARE_CHILD_SIZES);
}

/*
 *	selection operator test functions:
 */
template<typename TSelection, typename TRandom, typename TContainer>
void test_selection_operator(TRandom& g, TContainer& population, const uint32_t count)
{
	TSelection selection(g);
	TContainer children;

	selection.select(population, count, children);
	CPPUNIT_ASSERT_EQUAL(count, (uint32_t)children.size());
}

/*
 *	mutation operator test functions:
 */
template<typename TMutation, typename TRandom, typename TGenome, typename LessThan = less<typename TGenome::value_type>>
void test_mutation_operator(TRandom& g, vector<shared_ptr<TGenome>>& population)
{
	TMutation mutation(g);
	shared_ptr<TGenome> genome;
	LessThan lt;
	bool changed = false;

 	genome = make_shared<TGenome>(population[0]->size(), population[0]->get_fitness_func());

	for(uint32_t i = 0; i < population.size(); i++)
	{
		copy(population[i], genome);
		mutation.mutate(population.at(i));

		if(!compare_genomes<LessThan>(genome, population[i]))
		{
			changed = true;
		}
	}

	CPPUNIT_ASSERT(changed);

	changed = false;

	for(uint32_t i = 0; i < population.size(); i++)
	{
		mutation.mutate(population[i], genome);

		if(!compare_genomes<LessThan>(genome, population[i]))
		{
			changed = true;
		}
	}

	CPPUNIT_ASSERT(changed);

	mutation.mutate(begin(population), end(population));
}

/*
 *	test PrimitiveGenome class & operators:
 */
class PrimitiveGenomeTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(PrimitiveGenomeTest);
	CPPUNIT_TEST(test_genome_class);
	CPPUNIT_TEST(test_operators);
	CPPUNIT_TEST_SUITE_END();

	protected:
		typedef PrimitiveGenome<int32_t> TestGenome;

		/*
		 *	fitness function:
		 */
		static double fitness(const TestGenome* g)
		{
			double f = 0.0;

			for(uint32_t i = 0; i < g->size(); i++)
			{
				f += g->at(i) * (i % 2);
			}

			return f;
		}

		/*
		 *	factory class:
		 */
		class TestFactory : public AFactory<shared_ptr<TestGenome>>
		{
			public:
				using AFactory<shared_ptr<TestGenome>>::random;

				const int32_t GENOME_LENGTH = 20;

				TestFactory(shared_ptr<ARandomNumberGenerator> rnd_generator) : AFactory<shared_ptr<TestGenome>>(rnd_generator) {}

			protected:
				void random_impl(const uint32_t count, IOutputAdapter<shared_ptr<TestGenome>> &output)
				{
					int32_t numbers[GENOME_LENGTH];

					for(uint32_t i = 0; i < count; i++)
					{
						auto genome = make_shared<TestGenome>(GENOME_LENGTH, &fitness);

						generator->get_unique_int32_seq(0, 19, numbers, GENOME_LENGTH);

						for(uint32_t m = 0; m < GENOME_LENGTH; ++m)
						{
							genome->set(m, numbers[m]);
						}

						output.append(genome);
					}
				}
		};

		/*
		 *	test case:
		 */
		void test_genome_class()
		{
			TestGenome g(10, &fitness);
			int i;
			uint32_t index;
			bool found;
			size_t hash;
			string str;

			// test getter methods:
			CPPUNIT_ASSERT_EQUAL(10u, g.size());
			CPPUNIT_ASSERT_EQUAL(&fitness, g.get_fitness_func());

			// set genomes:
			for(i = 0; i < 10; i++)
			{
				g.copy_to(i, 10 - i);
			}

			for(i = 0; i < 10; i++)
			{
				CPPUNIT_ASSERT_EQUAL(10 - i, g.at(i));
			}

			// test hash & string serializaton methods:
			hash = g.hash();
			str = g.to_string();

			CPPUNIT_ASSERT(hash != 0);
			CPPUNIT_ASSERT(str.length() != 0);

			// change genome & compare hash/string:
			for(i = 0; i < 10; i++)
			{
				g.set(i, i);
			}

			for(i = 0; i < 10; i++)
			{
				CPPUNIT_ASSERT_EQUAL(i, g.at(i));
			}

			CPPUNIT_ASSERT(g.hash() != hash);
			CPPUNIT_ASSERT(str.compare(g.to_string()) != 0);

			// swap two genes:
			g.swap(0, 9);

			CPPUNIT_ASSERT_EQUAL(9, g.at(0));
			CPPUNIT_ASSERT_EQUAL(0, g.at(9));

			// try to find genes:
			found = g.index_of(8, index);

			CPPUNIT_ASSERT(found);
			CPPUNIT_ASSERT_EQUAL(8u, index);

			found = g.index_of(100, index);
			CPPUNIT_ASSERT(!found);
		}

		void test_operators()
		{
			auto g = make_shared<TR1UniformDistribution<mt19937_64>>();
			TestFactory factory(g);
			vector<shared_ptr<TestGenome>> population;

			factory.random(20, population);

			test_cut_and_splice_crossover(g, population);
			test_cycle_crossover(g, population);
			test_edge_recombination(g, population);
			test_one_point_crossover(g, population);
			test_ordered_crossover(g, population);
			test_pmx_crossover(g, population);
			test_twopoint_crossover(g, population);
			test_uniform_crossover(g, population);

			test_selection_operator<DoubleTournamentSelection<TestGenome>>(g, population, 10);
			test_selection_operator<FitnessProportionalSelection<TestGenome>>(g, population, 10);
			test_selection_operator<AlignedFitnessProportionalSelection<TestGenome>>(g, population, 10);
			test_selection_operator<FittestSelection<TestGenome>>(g, population, 10);
			test_selection_operator<StochasticUniversalSampling<TestGenome>>(g, population, 10);
			test_selection_operator<TournamentSelection<TestGenome>>(g, population, 10);

			test_mutation_operator<DoubleSwapMutation<TestGenome>>(g, population);
			test_mutation_operator<SingleSwapMutation<TestGenome>>(g, population);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(PrimitiveGenomeTest);

/*
 *	test binary genome & operators:
 */
class BinaryGenomeTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(BinaryGenomeTest);
	CPPUNIT_TEST(test_genome_class);
	CPPUNIT_TEST(test_operators);
	CPPUNIT_TEST_SUITE_END();

	protected:
		typedef PrimitiveGenome<bool> TestGenome;

		/*
		 *	fitness function:
		 */
		static double fitness(const TestGenome* g)
		{
			double f = 0.0;

			for(uint32_t i = 0; i < g->size(); i++)
			{
				if(g->at(i))
				{
					f++;
				}
			}

			return f;
		}

		/*
		 *	factory class:
		 */
		class TestFactory : public AFactory<shared_ptr<TestGenome>>
		{
			public:
				using AFactory<shared_ptr<TestGenome>>::random;

				const int32_t GENOME_LENGTH = 20;

				TestFactory(shared_ptr<ARandomNumberGenerator> rnd_generator) : AFactory<shared_ptr<TestGenome>>(rnd_generator) {}

			protected:
				void random_impl(const uint32_t count, IOutputAdapter<shared_ptr<TestGenome>> &output)
				{
					int32_t numbers[GENOME_LENGTH];

					for(uint32_t i = 0; i < count; i++)
					{
						auto genome = make_shared<TestGenome>(GENOME_LENGTH, &fitness);

						generator->get_int32_seq(0, 1, numbers, GENOME_LENGTH);

						for(uint32_t m = 0; m < GENOME_LENGTH; ++m)
						{
							genome->set(m, numbers[m]);
						}

						output.append(genome);
					}
				}
		};

		/*
		 *	test case:
		 */
		void test_genome_class()
		{
			TestGenome g0(10, &fitness);
			TestGenome g1(10, &fitness);
			int i;
			uint32_t index;
			bool found;
			size_t hash;
			string str;

			// test getter methods:
			CPPUNIT_ASSERT_EQUAL(10u, g0.size());
			CPPUNIT_ASSERT_EQUAL(&fitness, g0.get_fitness_func());

			// set genomes:
			for(i = 0; i < 10; i++)
			{
				g0.copy_to(i, i % 2);
			}

			for(i = 0; i < 10; i++)
			{
				CPPUNIT_ASSERT_EQUAL((bool)(i % 2), g0.at(i));
			}

			// test hash & string serializaton methods:
			hash = g0.hash();
			str = g0.to_string();

			CPPUNIT_ASSERT(hash != 0);
			CPPUNIT_ASSERT(str.length() != 0);

			// change genome & compare hash/string:
			for(i = 0; i < 10; i++)
			{
				g0.set(i, i % 3);
			}

			for(i = 0; i < 10; i++)
			{
				CPPUNIT_ASSERT_EQUAL((bool)(i % 3), g0.at(i));
			}

			CPPUNIT_ASSERT(g0.hash() != hash);
			CPPUNIT_ASSERT(str.compare(g0.to_string()) != 0);

			// swap two genes:
			g0.set(0, false);
			g0.set(9, true);

			g0.swap(0, 9);

			CPPUNIT_ASSERT(g0.at(0));
			CPPUNIT_ASSERT(!g0.at(9));

			// try to find genes:
			found = g0.index_of(true, index);

			CPPUNIT_ASSERT(found);
			CPPUNIT_ASSERT_EQUAL(0u, index);

			// test equals() method:
			CPPUNIT_ASSERT(g0.equals(&g0));

			for(i = 0; i < 10; i++)
			{
				g1.copy_to(i, !g0.at(i));
			}

			CPPUNIT_ASSERT(!g0.equals(&g1));
		}

		void test_operators()
		{
			// generate random genomes:
			auto g = make_shared<TR1UniformDistribution<mt19937_64>>();
			TestFactory factory(g);
			vector<shared_ptr<TestGenome>> population;

			factory.random(20, population);

			// test operators:
			test_cut_and_splice_crossover(g, population);
			test_one_point_crossover(g, population);
			test_twopoint_crossover(g, population);
			test_uniform_crossover(g, population);

			test_selection_operator<DoubleTournamentSelection<TestGenome>>(g, population, 10);
			test_selection_operator<FitnessProportionalSelection<TestGenome>>(g, population, 10);
			test_selection_operator<AlignedFitnessProportionalSelection<TestGenome>>(g, population, 10);
			test_selection_operator<FittestSelection<TestGenome>>(g, population, 10);
			test_selection_operator<TournamentSelection<TestGenome>>(g, population, 10);

			test_mutation_operator<DoubleSwapMutation<TestGenome>>(g, population);
			test_mutation_operator<SingleSwapMutation<TestGenome>>(g, population);
			test_mutation_operator<BitStringMutation>(g, population);
			test_mutation_operator<InverseBitStringMutation>(g, population);
			test_mutation_operator<SingleBitStringMutation>(g, population);
		}
};

CPPUNIT_TEST_SUITE_REGISTRATION(BinaryGenomeTest);

/*
 *	test Genome class & operators:
 */
class GenomeTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(GenomeTest);
	CPPUNIT_TEST(test_genome_class);
	CPPUNIT_TEST(test_operators);
	CPPUNIT_TEST_SUITE_END();

	protected:
		/*
		 *	genome test class:
		 */
		class City : public AGene<City>
		{
			public:
				City(const string name, const int32_t x, const int32_t y) : _name(name), _x(x), _y(y) {}

				void set_name(const string name)
				{
					_name = name;
					modified();
				}

				string get_name() const
				{
					return _name;
				}

				void set_x(const int32_t x)
				{
					_x = x;
					modified();
				}

				int32_t get_x() const
				{
					return _x;
				}

				void set_y(const int32_t y)
				{
					_y = y;
					modified();
				}

				int32_t get_y() const
				{
					return _y;
				}

				size_t hash() override
				{
					SDBMHash hasher;

					hasher << _x << _y;

					return hasher.hash();
				}

				bool less_than(City* gene) override
				{
					return sqrt(pow(_x, 2) + pow(_y, 2)) < sqrt(pow(gene->_x, 2) + pow(gene->_y, 2));
				}

				City* clone() const override
				{
					return new City(_name, _x, _y);
				}

				string to_string() const override
				{
					return _name;
				}

			private:
				string _name;
				int32_t _x;
				int32_t _y;
		};

		/*
		 *	fitness function:
		 */
		static double fitness(const Genome<City>* genome)
		{
			City* city;
			double x0, x1, y0, y1;
			double f = 0;

			for(uint32_t i = 0; i < genome->size(); i++)
			{
				city = genome->at(i);

				if(!i)
				{
					x0 = city->get_x();
					y0 = city->get_y();
				}
				else
				{
					x1 = city->get_x();
					y1 = city->get_y();

					f += sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));

					x0 = city->get_x();
					y0 = city->get_y();
				}
			}

			return f;
		}

		/*
		 *	factory class:
		 */
		class RouteFactory : public AFactory<shared_ptr<Genome<City>>>
		{
			public:
				const uint32_t N_CITIES = 20;

				using AFactory<shared_ptr<Genome<City>>>::random;

				RouteFactory(shared_ptr<ARandomNumberGenerator> rnd_generator) : AFactory<shared_ptr<Genome<City>>>(rnd_generator) {}

			static City* create_new_city(const int32_t n)
			{
				char name[16];

				memset(name, 0, 16);
				sprintf(name, "%c", n + 65);

				return new City(name, _points[n][0], _points[n][1]);
			}

			protected:
				void random_impl(const uint32_t count, IOutputAdapter<shared_ptr<Genome<City>>> &output)
				{
					int32_t numbers[20];

					for(uint32_t i = 0; i < count; i++)
					{
						auto genome = make_shared<Genome<City>>(N_CITIES, &fitness);

						generator->get_unique_int32_seq(0, N_CITIES - 1, numbers, N_CITIES);

						for(uint32_t m = 0; m < N_CITIES; m++)
						{
							genome->set(m, create_new_city(numbers[m]));
						}

						output.append(genome);
					}
				}

			private:
				static const uint32_t _points[20][2];
		};

		/*
		 *	test case:
		 */
		void test_genome_class()
		{
			Genome<City> g0(10, &fitness);
			Genome<City> g1(10, &fitness);
			vector<City*> cities;
			uint32_t i;
			size_t hash;
			string str;
			City* city;
			bool found;
			uint32_t index;

			// create test cities:
			for(i = 0; i < 10; i++)
			{
				cities.push_back(RouteFactory::create_new_city(i));
			}

			// test getter methods:
			CPPUNIT_ASSERT_EQUAL(10u, g0.size());
			CPPUNIT_ASSERT_EQUAL(&fitness, g0.get_fitness_func());

			// set genomes:
			for(i = 0; i < 10; i++)
			{
				g0.set(i, cities[i]);
			}

			for(i = 0; i < 10; i++)
			{
				CPPUNIT_ASSERT(!g0.at(i)->less_than(cities[i]) && !cities[i]->less_than(g0.at(i)));
			}

			// test hash & string serializaton methods:
			hash = g0.hash();
			str = g0.to_string();

			CPPUNIT_ASSERT(hash != 0);
			CPPUNIT_ASSERT(str.length() != 0);

			// change genome & compare hash/string:
			for(i = 0; i < 10; i++)
			{
				city = RouteFactory::create_new_city(9 - i);
				g0.copy_to(i, city); /* this will delete the memory allocated for the
				                        test cities & so we have to create new genomes
				                        before we copy them */
				delete city;
			}

			CPPUNIT_ASSERT(g0.hash() != hash);
			CPPUNIT_ASSERT(str.compare(g0.to_string()) != 0);

			// swap to genes:
			g0.swap(0, 9);

			CPPUNIT_ASSERT(!g0.at(0)->get_name().compare("A"));
			CPPUNIT_ASSERT(!g0.at(9)->get_name().compare("J"));

			// try to find genes:
			city = RouteFactory::create_new_city(9);
			found = g0.index_of(city, index);

			CPPUNIT_ASSERT(found);
			CPPUNIT_ASSERT_EQUAL(9u, index);

			delete city;

			city = RouteFactory::create_new_city(19);
			found = g0.index_of(city, index);

			CPPUNIT_ASSERT(!found);

			delete city;

			// test equal() method:
			CPPUNIT_ASSERT(g0.equals(&g0));

			for(i = 1; i <= 10; i++)
			{
				g1.set(i - 1, RouteFactory::create_new_city(i));
			}

			CPPUNIT_ASSERT(!g0.equals(&g1));
		}

		void test_operators()
		{
			// generate random genomes:
			auto g = make_shared<TR1UniformDistribution<mt19937_64>>();
			RouteFactory factory(g);
			vector<shared_ptr<Genome<City>>> population;

			factory.random(20, population);

			// test operators:
			test_cycle_crossover<Genome<City>, City::LessThan>(g, population);
			test_edge_recombination<Genome<City>, City::LessThan>(g, population);
			test_ordered_crossover<Genome<City>, City::LessThan>(g, population);
			test_pmx_crossover<Genome<City>, City::LessThan>(g, population);

			test_cut_and_splice_crossover(g, population);
			test_one_point_crossover(g, population);
			test_twopoint_crossover(g, population);
			test_uniform_crossover(g, population);

			test_selection_operator<DoubleTournamentSelection<Genome<City>, less<double>>>(g, population, 10);
			test_selection_operator<MinimizingFitnessProportionalSelection<Genome<City>>>(g, population, 10);
			test_selection_operator<FittestSelection<Genome<City>, less<double>>>(g, population, 10);
			test_selection_operator<TournamentSelection<Genome<City>, less<double>>>(g, population, 10);

			test_mutation_operator<DoubleSwapMutation<Genome<City>>>(g, population);
			test_mutation_operator<SingleSwapMutation<Genome<City>>>(g, population);
		}
};

const uint32_t GenomeTest::RouteFactory::_points[20][2] =
{
	{ 12, 104 }, { 8234, 1800 }, { 929, 523 }, { 638, 102 }, { 72, 4123 },
	{ 7921, 2331 }, { 1902, 138 }, { 231, 9881}, { 97, 7510 }, { 711, 8391 },
	{ 96, 7313 }, { 219, 6 }, { 2, 8190 }, { 1532, 2310 }, { 5, 9 },
	{ 136, 196 }, { 54, 6377 }, { 74, 14 }, { 1569, 736 }, { 175, 1841 }
};

CPPUNIT_TEST_SUITE_REGISTRATION(GenomeTest);

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
