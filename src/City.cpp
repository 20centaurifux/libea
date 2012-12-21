#include <cstring>
#include <cassert>
#include "City.h"

using namespace ea;

City::City(const uint32_t x, const uint32_t y, const char* name) : Gene(192)
{
	byte data[24];
	int len;

	assert(name != NULL);

	len = strlen(name);
	assert(len < 15);

	memset(data, 0, 24);

	memcpy(data, &x, 4);
	memcpy(data + 4, &y, 4);
	memcpy(data + 8, name, len);

	_name_init = false;

	fill(data, 0, 24);
}

const char* City::get_name() const
{
	if(!_name_init)
	{
		read(8, (byte*)_name, 15);
	}

	return _name;
}


Gene* City::clone()
{
	return new City(get_x(), get_y(), get_name());
}
