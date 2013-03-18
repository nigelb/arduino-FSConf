/*
  FSConf is an Arduino library to retrieve configuration data from a SD card
  Copyright (C) 2013  NigelB

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef __FS_CONF_H__
#define __FS_CONF_H__


#include <SD.h>

#define FSCONF_OK 0
#define FSCONF_ERROR_CONFIG_NOT_FOUND -1
#define FSCONF_ERROR_COULD_NOT_OPEN_FILE -2


class FSConf
{
	private:
		int _cs_pin;
		bool debug_mode;
		Stream* debug_stream;
		String base_path;

	public:
		/*
		 * FSConf creates a new FSConf with the base directory set to name.
		 */
		FSConf(String name);

		/*
		 * begin initializes the SD card
		 */
		int begin(int cs_pin);
		void set_debug_stream(Stream* _stream);
		int getValue(String &value, int count, ...);

};

namespace __FSCONFIG_NAMESPACE__
{
	 extern unsigned int size;
	 extern unsigned int size1;
	 extern unsigned int size2;
	 extern String* tmp;
};

#define FSCONFIG_FSCONFIG_VA_NARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define FSCONFIG_VA_NARGS(...) FSCONFIG_FSCONFIG_VA_NARGS_IMPL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)


#define FSCONFIG_get_config(config, name, ...)\
		{\
			__FSCONFIG_NAMESPACE__::tmp = new String(); \
			config.getValue(*__FSCONFIG_NAMESPACE__::tmp, FSCONFIG_VA_NARGS(__VA_ARGS__), __VA_ARGS__); \
			__FSCONFIG_NAMESPACE__::size = __FSCONFIG_NAMESPACE__::tmp->length(); \
		}\
		char name[__FSCONFIG_NAMESPACE__::size + 1];\
		{\
			for(unsigned int i = 0; i < __FSCONFIG_NAMESPACE__::size; i++){;name[i] = __FSCONFIG_NAMESPACE__::tmp[0][i];}\
			name[__FSCONFIG_NAMESPACE__::size]=0; \
			delete __FSCONFIG_NAMESPACE__::tmp;\
		}

#define FSCONFIG_str_concat(name, src1, src2)\
		__FSCONFIG_NAMESPACE__::size1 = strlen(src1);\
		__FSCONFIG_NAMESPACE__::size2 = strlen(src2);\
		__FSCONFIG_NAMESPACE__::size = __FSCONFIG_NAMESPACE__::size1 + __FSCONFIG_NAMESPACE__::size2;\
		char name[__FSCONFIG_NAMESPACE__::size+1];\
		for(unsigned int i = 0; i < __FSCONFIG_NAMESPACE__::size1; i++)\
		{\
			name[i] = src1[i];\
		}\
		for(unsigned int i = 0; i < __FSCONFIG_NAMESPACE__::size2; i++)\
		{\
			name[i + __FSCONFIG_NAMESPACE__::size1] = src2[i];\
		}\
		name[__FSCONFIG_NAMESPACE__::size] = 0;

#endif
