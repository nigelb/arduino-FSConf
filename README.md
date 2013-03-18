FSConf
=====
FSConf is an Arduino library to retrieve configuration data from a SD card.
FSConf allows one to have a hierarchical human readable/editable configuration 
on a SD card. For example if we had an FSConfig object like:

	FSConf config("CONFIG");

And a file structure:

	CONFIG/
	  CID
	  CONN1/
	    CONTYPE
	    APN
      
With the contents of the files:

	CONFIG/CID:
	  1
	CONFIG/CONN1/CONTYPE:
	  GPRS    
	CONFIG/CONN1/APN:
	  internet
    
We could grab these values with the following calls:

	FSCONFIG_get_config(config, cid, "MODEM", "CID");
	FSCONFIG_str_concat(conn, "CONN", cid);
	FSCONFIG_get_config(config, contype, "MODEM", conn, "CONTYPE");
	FSCONFIG_get_config(config, apn, "MODEM", conn, "APN");

Macros
------

This library consists of the FSConfig object and some macros to use with it.
The macros are:

	FSCONFIG_get_config(config, name, ...)
		Example:
  
		  	FSCONFIG_get_config(config, name, "part1","part2")

  		Is conceptually:

	  		String value;
			config.getValue(value, 2, "part1", "part2");
			unsigned int size = value.length();
			char name[size+1];
			name[size] = 0;
			for(unsigned int i = 0; i < size; i++){
				name[i] = value[i]
			}

		With all temporary variables out of scope.

  	FSCONFIG_str_concat(name, src1, src2)
  		Example:
  		
  			char* conn = "CONN";
  			char* cid = "1";
  			FSCONFIG_str_concat(newstring, conn, cid)
  			
  		Is conceptually:
  		
  			char* conn = "CONN";
  			char* cid = "1";
  			int conn_size = strlen(conn);
  			int cid_size = strlen(cid);
  			int size = conn_size + cid_size;
  			char newstring[size+1];
			for(int i = 0; i < conn_size; i++)
			{
				name[i] = conn[i];
		    }
			for(int i = 0; i < cid_size; i++)
			{
				name[i + conn_size] = cid[i];
			}
			newstring[size] = 0;

Example
=======
```cxx
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

/*
  This example demonstrates the use of FSConf to get config details
  for the Sim900 GPRS modem from a SD card.
  
  The file system on the SD card for this example is:
  
  CONFIG/
  	CID
  	CONN1/
      CONTYPE
      APN
      
  With the contents of the files:
  CONFIG/CID:
    1
  CONFIG/CONN1/CONTYPE:
    GPRS    
  CONFIG/CONN1/APN:
    internet
    
  This example outputs the following on the serial connection:
  
    Connection: CONN1
    Connection Type: GPRS
    APN: internet
    User: 
    Password: 
    Phone Number: 
    Connection Rate: 
  

*/

#include <SD.h>
#include "FSConf.h"

FSConf config("CONFIG");

void print_key_value(char* key, char* value)
{
  Serial.print(key);
  Serial.println(value);
}

void setup()
{
  Serial.begin(115200);
  //config.set_debug_stream(&Serial);
  config.begin(53);

  FSCONFIG_get_config(config, cid, "MODEM", "CID");
  FSCONFIG_str_concat(conn, "CONN", cid);
  print_key_value("Connection: ", conn);

  FSCONFIG_get_config(config, contype, "MODEM", conn, "CONTYPE");
  print_key_value("Connection Type: ", contype);

  FSCONFIG_get_config(config, apn, "MODEM", conn, "APN");
  print_key_value("APN: ", apn);

  FSCONFIG_get_config(config, user, "MODEM", conn, "USER");
  print_key_value("User: ", user);

  FSCONFIG_get_config(config, pwd, "MODEM", conn, "PASSWORD");
  print_key_value("Password: ", pwd);

  FSCONFIG_get_config(config, phone, "MODEM", conn, "PHONE");
  print_key_value("Phone Number: ", phone);

  FSCONFIG_get_config(config, rate, "MODEM", conn, "RATE");
  print_key_value("Connection Rate: ", rate);
}

void loop(){}
```
