/***************************************************************************
    begin........: November 2012
    copyright....: Sebastian Fedrau
    email........: lord-kefir@arcor.de
 ***************************************************************************/

/***************************************************************************
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.
 ***************************************************************************/
/*!
 * \file main.cpp
 * \brief Contains some testing stuff.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#include <iostream>
#include <vector>
#include <cmath>

#include "URandomDeviceNumberGenerator.h"
#include "AnsiRandomNumberGenerator.h"
#include "RouteFactory.h"
#include "foreach.h"
#include "OnePointCrossover.h"
#include "TwoPointCrossover.h"
#include "CutAndSpliceCrossover.h"
#include "OrderedCrossover.h"
#include "CycleCrossover.h"
#include "EdgeRecombinationCrossover.h"
#include "PMXCrossover.h"
#include "UniformCrossover.h"

using namespace ea;
using namespace std;

float fitness(Individual *individual)
{
	City* city;
	float x0, x1, y0, y1;
	float f = 0;

	for(uint32_t i = 0; i < individual->size(); i++)
	{
		city = (City*)individual->gene_at(i);

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

void print_cities(Individual* individual)
{
	City* city;

	for(uint32_t i = 0; i < individual->size(); i++)
	{
		city = (City*)individual->gene_at(i);
		cout << city->get_name();

		if(i <= individual->size() - 2)
		{
			cout << ", ";
		}
	}

	cout << endl;
}

int main()
{
	ARandomNumberGenerator* g = new AnsiRandomNumberGenerator();
	RouteFactory f(g, fitness);
	vector<Individual*> p0;

	p0 = f.random(15);

	//Individual *idv;

	//idv = p0[1];

	/*
	for(uint32_t i = 0; i < idv->size(); i++)
	{
		City* city = (City*)idv->gene_at(i);
		cout << city->get_name() << ": x=" << city->get_x() << ", y=" << city->get_y() << endl;
	}
	*/

	// display fitness:
	/*
	for(uint32_t i = 0; i < p0.size(); i++)
	{
		idv = p0[i];
		cout << idv->fitness() << endl;
	}
	*/

	// recombinate:
	PMXCrossover r(g);

	print_cities(p0[0]);
	print_cities(p0[1]);

	vector<Individual*>children;
	r.crossover(p0[0], p0[1], children);

	print_cities(children[0]);
	print_cities(children[1]);

	/*
	cout << p0[0]->fitness() << endl;
	p0[0]->gene_at(0)->set(0, true);
	cout << p0[0]->fitness() << endl;
	*/

	// cleanup:
	for(uint32_t i = 0; i < p0.size(); i++)
	{
		foreach<Individual*>(p0, delete_object<Individual>);
	}

	foreach<Individual*>(children, delete_object<Individual>);

	delete g;

	return 0;
}
