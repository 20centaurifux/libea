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
   @file AFileBasedRandomNumberGenerator.cpp
   @brief Base class for random number generators reading data from a file.
   @author Sebastian Fedrau <lord-kefir@arcor.de>
   @version 0.1.0
 */

#include <cstring>
#include <cassert>

#include "AFileBasedRandomNumberGenerator.h"

using namespace std;

namespace ea
{
	AFileBasedRandomNumberGenerator::AFileBasedRandomNumberGenerator(const char* filename, const int32_t buffer_size)
	{
		assert(filename != NULL);
		assert(buffer_size > 0);
		assert(!(buffer_size % 4));

		// initialize internal buffer size:
		if(buffer_size > 0)
		{
			if((_buffer_size = buffer_size) % 4)
			{
				_buffer_size = DEFAULT_BUFFER_SIZE;
			}
		}
		else
		{
			_buffer_size = 4;
		}

		// allocate memory required for buffer, initialize buffer pointer & copy filename:
		_stream = NULL;
		_buffer = new int32_t[_buffer_size / 4];
		_buffer_ptr = -1;
		_filename = strdup(filename);
	}

	AFileBasedRandomNumberGenerator::~AFileBasedRandomNumberGenerator()
	{
		// close file stream:
		if(_stream)
		{
			_stream->close();
			delete _stream;
		}

		// free allocated memory:
		free(_filename);
		delete[] _buffer;
	}

	int32_t AFileBasedRandomNumberGenerator::random()
	{
		// (re)fill buffer:
		if(_buffer_ptr == -1 || _buffer_ptr * 4 >= _buffer_size)
		{
			get_stream()->read((char*)_buffer, _buffer_size);
			_buffer_ptr = 0;
		}

		// return current buffer element & increment counter;
		return _buffer[_buffer_ptr++];
	}

	ifstream* AFileBasedRandomNumberGenerator::get_stream()
	{
		bool new_stream = true;

		// (re)open file stream:
		if(_stream)
		{
			if(_stream->good())
			{
				new_stream = false;
			}
			else
			{
				_stream->close();
				delete _stream;
			}
		}

		if(new_stream)
		{
			_stream = new ifstream(_filename, ios::in|ios::binary);
		}

		return _stream;
	}
}
