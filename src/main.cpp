#include <iostream>
#include "PrimitiveGenome.h"
#include "AFactory.h"
#include <vector>
#include <algorithm>

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

int main(int argc, char* argv[])
{
	UInt32_SeqFactory factory;

	std::vector<UInt32_Seq*> population;

	auto inserter = std::back_inserter(population);

	factory.create_population(10, inserter);

	std::for_each(begin(population), end(population), [](UInt32_Seq* seq)
	{
		std::cout << base.len(seq) << std::endl;
	});

	ea::dispose(base, begin(population), end(population));

	return 0;
}
