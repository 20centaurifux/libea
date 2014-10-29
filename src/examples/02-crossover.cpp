/***************************************************************************
    begin........: November 2012
    copyright....: Sebastian Fedrau
    email........: sebastian.fedrau@gmail.com
 ***************************************************************************/

/***************************************************************************
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License v3 as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License v3 for more details.
 ***************************************************************************/
#include <stdint.h>
#include <iostream>
#include <vector>
#include <PrimitiveGenome.hpp>
#include <AFactory.hpp>
#include <AnsiRandomNumberGenerator.hpp>
#include <OnePointCrossover.hpp>
#include "InputAdapter.hpp"

// store genomes in sequences of int32_t, this typedef makes the code more readable:
typedef ea::Sequence<int32_t> Sequence;

// fitness function:
class Fitness
{
	public:
		float operator()(const Sequence* const &seq) const
		{
			float avg = 0;

			for(auto i = 0; i < ea::sequence_len(seq); i++)
			{
				avg += ea::sequence_get(seq, i);
			}

			return avg / ea::sequence_len(seq);
		}
};

// base class for modifying/accessing sequences:
typedef ea::PrimitiveGenomeBase<int32_t, Fitness> Base;

static Base base;

// factory class:
class Factory : public ea::AFactory<Sequence*>
{
	public:
		Sequence* create_sequence()
		{
			int32_t genes[100];

			_g.get_int32_seq(1, 100, genes, 100);

			Sequence* seq = base.create(100);

			for(uint32_t i = 0; i < 100; i++)
			{
				base.set(seq, i, genes[i]);
			}

			return seq;
		}

	private:
		static ea::AnsiRandomNumberGenerator _g;
};

ea::AnsiRandomNumberGenerator Factory::_g;

// debug function:
static void print_genome(Sequence* seq)
{
	std::cout << "hash: " << base.hash(seq) << ", fitness: " <<
base.fitness(seq) << ", sequence: ";

	for(uint32_t i = 0; i < 10; i++)
	{
		std::cout << base.get(seq, i) << " ";
	}

	std::cout << std::endl;
}

template<typename TSequence, typename F>
void combine(const ea::IInputAdapter<TSequence>& input)
{
	F f;

	if(input.size() == 0)
	{
		return;
	}

	for(uint32_t i = 0; i < input.size(); i++)
	{
		f(input.at(i), input.at(i + 1));
	}
}

template<typename TSequence, typename ACrossover>
uint32_t multi_crossover(ACrossover& crossover,
ea::IInputAdapter<TSequence>& input, ea::IOutputAdapter<TSequence>& output)
{
	uint32_t count = 0;

	for(uint32_t i = 0; i < input.size(); i++)
	{
		for(uint32_t j = 0; j < input.size(); j++)
		{
			if(i != j)
			{
				count += crossover.crossover(input.at(i), input.at(j), output);
			}
		}
	}

	return count;
}

int main(int argc, char *argv[])
{
	Factory f;
	std::vector<Sequence*> population;

	// create adapter from back_inserter:
	auto inserter = std::back_inserter(population);
	ea::STLVectorAdapter<Sequence*> adapter(inserter);

	// create parent genomes:
	f.create_population(1500, adapter);

	auto a = ea::make_input_adapter<Sequence*>(population);
	ea::OnePointCrossover<Base> c;

	std::vector<Sequence*> children;

	// create adapter from back_inserter:
	inserter = std::back_inserter(children);
	ea::STLVectorAdapter<Sequence*> c_adapter(inserter);

	// create crossover operator:
	multi_crossover(c, a, c_adapter);

	for(auto child : children)
	{
		print_genome(child);
	}

	return 0;
}
