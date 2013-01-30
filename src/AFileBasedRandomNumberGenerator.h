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
/**
   @file AFileBasedRandomNumberGenerator.h
   @brief Base class for random number generators reading data from a file.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#ifndef AFILEBASEDRANDOMNUMBERGENERATOR_H
#define AFILEBASEDRANDOMNUMBERGENERATOR_H

#include <fstream>
#include "ARandomNumberGenerator.h"

namespace ea
{
	/**
	   @addtogroup Core
	   @{
	   	@addtogroup Random
	   	@{
	 */

	/**
	   @class AFileBasedRandomNumberGenerator
	   @brief Abstract base class for random number generators reading data from a file.
	 */
	class AFileBasedRandomNumberGenerator : public ARandomNumberGenerator
	{
		public:
			/*! Default size of the buffer used to read from a file. */
			static const int32_t DEFAULT_BUFFER_SIZE = 512;

			/**
			   @param filename name of the file to read random data from
			   @param buffer_size the buffer size
			 */
			AFileBasedRandomNumberGenerator(const char* filename, const int32_t buffer_size);
			virtual ~AFileBasedRandomNumberGenerator();

			int32_t random();

		private:
			char *_filename;
			std::ifstream* _stream;
			int32_t *_buffer;
			int32_t _buffer_ptr;
			int32_t _buffer_size;

			std::ifstream* get_stream();
	};

	/**
	   	@}
	   @}
	 */
}
#endif
