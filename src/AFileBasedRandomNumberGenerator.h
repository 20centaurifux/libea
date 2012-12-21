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
 * \file AFileBasedRandomNumberGenerator.h
 * \brief Base class for random number generators reading data from a file.
 * \author Sebastian Fedrau <lord-kefir@arcor.de>
 * \version 0.1.0
 */

#ifndef AFILEBASEDRANDOMNUMBERGENERATOR_H
#define AFILEBASEDRANDOMNUMBERGENERATOR_H

#include <fstream>
#include "ARandomNumberGenerator.h"

namespace ea
{
	class AFileBasedRandomNumberGenerator : public ARandomNumberGenerator
	{
		public:
			static const int32_t DEFAULT_BUFFER_SIZE = 512;

			AFileBasedRandomNumberGenerator(const char* filename, const int32_t buffer_size);
			virtual ~AFileBasedRandomNumberGenerator();
			inline const char* get_filename() const { return _filename; };
			int32_t random();
			inline uint32_t get_buffer_size() const { return _buffer_size; }

		private:
			char *_filename;
			std::ifstream* _stream;
			int32_t *_buffer;
			int32_t _buffer_ptr;
			int32_t _buffer_size;

			std::ifstream* get_stream();
	};
}
#endif
