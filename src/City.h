#ifndef CITY_H
#define CITY_H

#include "Gene.h"

class City : public ea::Gene
{
	public:
		City(const uint32_t x, const uint32_t y, const char* name);
		virtual ~City() {};
		inline int32_t get_x() const { return read_uint32(0); }
		inline int32_t get_y() const { return read_uint32(4); }
		const char* get_name() const;
		Gene* clone();

	private:
		char _name[16];
		bool _name_init;
};
#endif
