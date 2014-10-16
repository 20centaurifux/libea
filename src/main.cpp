/// @cond INTERNAL

#include "UniformCrossover.h"
#include "PrimitiveGenome.h"

#include <iostream>
#include <vector>
#include <algorithm>

typedef ea::CachedSequence<bool> Sequence;

class Fitness
{
	public:
		float operator()(const Sequence* const &seq) const
		{
			float sum = 0;

			for(auto i = 0; i < seq->len; i++)
			{
				if(seq->genes[i])
				{
					sum++;
				}
			}

			return sum;
		}
};

typedef ea::CachedPrimitiveGenomeBase<bool, Fitness> GenomeBase;

static GenomeBase base;

void dump(Sequence* seq)
{
	for(uint32_t i = 0; i < base.len(seq); i++)
	{
		std::cout << base.get(seq, i) << " ";
	}

	std::cout << std::endl;
}

int main(int argc, char* argv[])
{
	Sequence* seq = base.create(10);

	for(uint32_t i = 0; i < 10; i++)
	{
		base.set(seq, i, i % 3 ? true : false);
	}

	dump(seq);

	ea::UniformCrossover<GenomeBase> c(nullptr);


//	//UInt32_SeqFactory factory;
//
//	Sequence* a = base.create(10);
//
//	base.set(a, 0, 8);
//	base.set(a, 1, 9);
//	base.set(a, 2, 10);
//	base.set(a, 3, 11);
//	base.set(a, 4, 12);
//	base.set(a, 5, 13);
//	base.set(a, 6, 14);
//	base.set(a, 7, 15);
//	base.set(a, 8, 16);
//	base.set(a, 9, 17);
//
//	base.set(b, 0, 0);
//	base.set(b, 1, 1);
//	base.set(b, 2, 2);
//	base.set(b, 3, 3);
//	base.set(b, 4, 4);
//	base.set(b, 5, 5);
//	base.set(b, 6, 6);
//	base.set(b, 7, 7);
//	base.set(b, 8, 8);
//	base.set(b, 9, 9);
//
//	//factory.create_population(10, adapter);
//
//	std::shared_ptr<ea::ARandomNumberGenerator> r = std::make_shared<ea::TR1UniformDistribution<std::mt19937_64>>();
//
//	ea::TwoPointCrossover<UInt32_GenomeBase> c(r);
//
//	c.crossover(a, b, adapter);
//
//	dump(a);
//	dump(b);
//
//	std::cout << std::endl;
//
//	std::for_each(begin(population), end(population), [](UInt32_Seq* seq)
//	{
//		dump(seq);
//		std::cout << base.fitness(seq) << std::endl << std::endl;
//	});
//
//
//	std::vector<uint32_t> indexes;
//	auto iinserter = std::back_inserter(indexes);
//	ea::STLVectorAdapter<uint32_t> aadapter(iinserter);
//
//	auto input = ea::make_input_adapter<UInt32_Seq*>(population);
//
//	ea::DoubleTournamentSelection<UInt32_GenomeBase> sel(r);
//
//	sel.select(input, 20, aadapter);
//
//	std::for_each(begin(indexes), end(indexes), [](uint32_t index)
//	{
//		std::cout << index << std::endl;
//	});
//
//	ea::DoubleSwapMutation<UInt32_GenomeBase> m(r);
//
//	dump(a);
//	UInt32_Seq* d = base.create(10);
//	d = m.create_child(a);
//	dump(a);
//	dump(d);

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
