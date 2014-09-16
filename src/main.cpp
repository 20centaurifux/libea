#include <iostream>
#include "AFactory.h"
#include <vector>
#include <algorithm>

#include "PrimitiveGenome.h"
#include "ACrossover.h"
#include "OutputAdapter.h"
#include "CutAndSpliceCrossover.h"
#include "AnsiRandomNumberGenerator.h"

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

int main(int argc, char* argv[])
{
	//UInt32_SeqFactory factory;

	std::vector<UInt32_Seq*> population;
	auto inserter = std::back_inserter(population);
	ea::STLVectorAdapter<UInt32_Seq*> adapter(inserter);

	UInt32_Seq* a = base.create(10);
	UInt32_Seq* b = base.create(10);

	for(uint32_t i = 0; i < 10; i++)
	{
		base.set(a, i, i);
		base.set(b, i, i + 100);
	}

	//factory.create_population(10, adapter);

	ea::CutAndSpliceCrossover<UInt32_GenomeBase, ea::AnsiRandomNumberGenerator> c;

	c.crossover(a, b, adapter);


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
