/// @cond INTERNAL
#include <iostream>
#include "AFactory.h"
#include <vector>
#include <algorithm>

#include "PrimitiveGenome.h"
#include "ACrossover.h"
#include "OutputAdapter.h"
#include "CutAndSpliceCrossover.h"
#include "CycleCrossover.h"
#include "EdgeRecombinationCrossover.h"
#include "TwoPointCrossover.h"
#include "OnePointCrossover.h"
#include "PMXCrossover.h"
#include "UniformCrossover.h"
#include "OrderedCrossover.h"
#include "AnsiRandomNumberGenerator.h"
#include "TR1UniformDistribution.h"
#include "InputAdapter.h"
#include "IIndexSelection.h"
#include "FittestSelection.h"
#include "TournamentSelection.h"
#include "DoubleTournamentSelection.h"
#include "StochasticUniversalSampling.h"
#include "FitnessProportionalSelection.h"

typedef ea::CachedSequence<uint32_t> UInt32_Seq;

class Fitness
{
	public:
		float operator()(const UInt32_Seq* const &seq) const
		{
			float sum = 0;

			for(auto i = 0; i < seq->len; i++)
			{
				sum += seq->genes[i];
			}

			return sum;
		}
};

typedef ea::CachedPrimitiveGenomeBase<uint32_t, Fitness> UInt32_GenomeBase;

static UInt32_GenomeBase base;

class UInt32_SeqFactory : ea::AFactory<UInt32_Seq*>
{
	public:
		using ea::AFactory<UInt32_Seq*>::create_population;

		UInt32_Seq* create_sequence()
		{
			auto seq = base.create(10);
			static int v = 0;

			for(int i = 0; i < 10; i++)
			{
				base.set(seq, i, v++);

				if(v == 3)
				{
					v = 0;
				}
			}

			return seq;
		}
};

template<typename TGenomeBase>
class Foo : ea::ACrossover<TGenomeBase>
{
	public:
		typedef typename TGenomeBase::sequence_type sequence_type;

		uint32_t crossover(const sequence_type& a, const sequence_type& b, ea::IOutputAdapter<sequence_type>& output)
		{
			TGenomeBase foo;

			output.push(foo.create(10));
			return 0;
		}
};

void dump(const UInt32_Seq* seq)
{
	for(uint32_t i = 0; i < seq->len; i++)
	{
		std::cout << seq->genes[i] << " ";
	}

	std::cout << std::endl;
}

int main(int argc, char* argv[])
{
	//UInt32_SeqFactory factory;

	std::vector<UInt32_Seq*> population;
	auto inserter = std::back_inserter(population);
	ea::STLVectorAdapter<UInt32_Seq*> adapter(inserter);

	UInt32_Seq* a = base.create(10);
	UInt32_Seq* b = base.create(10);

	base.set(a, 0, 8);
	base.set(a, 1, 9);
	base.set(a, 2, 10);
	base.set(a, 3, 11);
	base.set(a, 4, 12);
	base.set(a, 5, 13);
	base.set(a, 6, 14);
	base.set(a, 7, 15);
	base.set(a, 8, 16);
	base.set(a, 9, 17);

	base.set(b, 0, 0);
	base.set(b, 1, 1);
	base.set(b, 2, 2);
	base.set(b, 3, 3);
	base.set(b, 4, 4);
	base.set(b, 5, 5);
	base.set(b, 6, 6);
	base.set(b, 7, 7);
	base.set(b, 8, 8);
	base.set(b, 9, 9);

	//factory.create_population(10, adapter);

	std::shared_ptr<ea::ARandomNumberGenerator> r = std::make_shared<ea::TR1UniformDistribution<std::mt19937_64>>();

	ea::TwoPointCrossover<UInt32_GenomeBase> c(r);

	c.crossover(a, b, adapter);

	dump(a);
	dump(b);

	std::cout << std::endl;

	std::for_each(begin(population), end(population), [](UInt32_Seq* seq)
	{
		dump(seq);
		std::cout << base.fitness(seq) << std::endl << std::endl;
	});


	std::vector<uint32_t> indexes;
	auto iinserter = std::back_inserter(indexes);
	ea::STLVectorAdapter<uint32_t> aadapter(iinserter);

	auto input = ea::make_input_adapter<UInt32_Seq*>(population);

	ea::DoubleTournamentSelection<UInt32_GenomeBase> sel(r);

	sel.select(input, 20, aadapter);

	std::for_each(begin(indexes), end(indexes), [](uint32_t index)
	{
		std::cout << index << std::endl;
	});

//	std::for_each(begin(population), end(population), [](UInt32_Seq* seq)
//	{
//		std::cout << base.len(seq) << std::endl;
//	});
//
//	Foo<UInt32_GenomeBase> c;
//
//	UInt32_Seq* a = population.at(0);
//	UInt32_Seq* b = population.at(0);
//
//	c.crossover(a, b, adapter);
//
//	ea::dispose(base, begin(population), end(population));

	return 0;
}
/// @endcond
