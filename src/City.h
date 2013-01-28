#ifndef CITY_H
#define CITY_H

#include "AGene.h"
#include "SDBMHash.h"

namespace ea
{
	class City : public AGene
	{
		public:
			City(const uint32_t x, const uint32_t y, const std::string name) : AGene(), _x(x), _y(y), _name(name) {}

			void set_name(const std::string& name)
			{
				_name = name;
				notifiy();
			}

			std::string get_name()
			{
				return _name;
			}

			uint32_t get_x()
			{
				return _x;
			}

			void set_x(const uint32_t x)
			{
				_x = x;
				notifiy();
			}

			uint32_t get_y()
			{
				return _y;
			}

			void set_y(const uint32_t y)
			{
				_y = y;
				notifiy();
			}

			bool equals(const AGene* object) const
			{
				const City* city = (City*)object;

				if((city = dynamic_cast<const City*>(object)))

				{
					return _x == city->_x && _y == city->_y && !_name.compare(city->_name);
				}

				return false;
			}

			City* clone() const
			{
				return new City(_x, _y, _name);
			}

			size_t hash() const
			{
				SDBMHash hash;

				hash << _x << _y << _name;

				return hash.hash();
			}

		private:
			uint32_t _x;
			uint32_t _y;
			std::string _name;
	};
}
#endif
