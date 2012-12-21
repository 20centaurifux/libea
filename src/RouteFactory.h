#ifndef ROUTEFACTORY_H
#define ROUTEFACTORY_H

#include <vector>
#include "AFactory.h"
#include "City.h"
#include "Individual.h"

class RouteFactory : public ea::AFactory<ea::Individual*>
{
	public:
		RouteFactory(ea::ARandomNumberGenerator* rnd_generator, ea::FitnessFunc fitness);
		virtual ~RouteFactory() {};
		static City* create_new_city(const int32_t n);
		std::vector<ea::Individual*> random(const uint32_t count);
		ea::ISequence<ea::Individual*>* get_sequence();
		ea::ISequence<ea::Individual*>* get_sequence(ea::Individual* offset);
		static const int32_t N_CITIES = 20;

	private:
		static const uint32_t _points[N_CITIES][2];
		ea::FitnessFunc _fitness_func;
};
#endif
