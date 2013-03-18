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

#include "FSConf.h"

unsigned int __FSCONFIG_NAMESPACE__::size = 0;
unsigned int __FSCONFIG_NAMESPACE__::size1 = 0;
unsigned int __FSCONFIG_NAMESPACE__::size2 = 0;
String*      __FSCONFIG_NAMESPACE__::tmp = 0;


FSConf::FSConf(String name)
{
	base_path = name;
}

int FSConf::begin(int cs_pin)
{
	_cs_pin = cs_pin;
	bool sd_initilized = false;
	//Setup CS for SD Card
	if(debug_mode){
		debug_stream->println("Trying SD Card.");
	}
	pinMode(cs_pin, OUTPUT);
	if (!SD.begin(cs_pin)) {
		if(debug_mode) debug_stream->println("initialization failed! Hanging...");
		sd_initilized = false;
	}
	else{
		if(debug_mode) debug_stream->println("initialization done.");
		sd_initilized = true;

		if(debug_mode) debug_stream->println("FSConfig Beginning...");
	}
	return sd_initilized;
}

void FSConf::set_debug_stream(Stream* _stream)
{
	debug_stream = _stream;
	debug_mode = true;
}

int FSConf::getValue(String &value, int count, ...)
{
	String builder("/");
	builder.concat(base_path);
	va_list vl;
	va_start(vl, count);
	char* val;
	for (int i = 0; i < count; i++)
	{
		val=va_arg(vl, char*);
		builder.concat("/");
		builder.concat(val);
	}
	va_end(vl);
	unsigned int len = builder.length();
	char result[len+1];
	result[len] = '\0';
	for(unsigned int i = 0; i < len; i++)
	{
		result[i] = builder[i];
	}
	if(SD.exists(result))
	{
		if(debug_mode) {
			debug_stream->print("Found configuration element: ");
			debug_stream->println(result);
		}
		File dataFile = SD.open(result, O_RDONLY);
		int val, count = 0, size = -1;
		if (dataFile) {
			size = dataFile.size();
			char tmp[size+1];
			tmp[size] = '\0';
			while (dataFile.available()) {
				val = dataFile.read();
				if(val > 0)
				{
					tmp[count] = val;
					count++;
				}
			}
			count--;
			while(tmp[count] == 13 || tmp[count] == 10)
			{
				tmp[count] = '\0';
				count--;
			}
			dataFile.close();
			value.concat(tmp);
			if(debug_mode) {
				debug_stream->print("Configuration value: ");
				debug_stream->println(value);
			}
			return FSCONF_OK;
		}
		return FSCONF_ERROR_COULD_NOT_OPEN_FILE;
	}else
	{
		return FSCONF_ERROR_CONFIG_NOT_FOUND;
	}
	return FSCONF_ERROR_CONFIG_NOT_FOUND;
}



