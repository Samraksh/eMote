#ifndef _DS18S20_H_
#define _DS18S20_H_

#include <tinyhal.h>
#include <Samraksh/Hal_util.h>

enum DS18S20CommandType
{
	COMMAND_SEARCH_ROM = 0xf0,
	COMMAND_READ_ROM = 0x33,
	COMMAND_MATCH_ROM = 0x55,
	COMMAND_SKIP_ROM = 0xcc,
	COMMAND_ALARM_SEARCH = 0xec,

};

enum DS18S20FunctionCommandType
{
	COMMAND_CONVERT_T = 0x44,
	COMMAND_WRITE_SCRATCHPAD = 0x4e,
	COMMAND_READ_SCRATCHPAD = 0xbe,
	COMMAND_COPY_SCRATCHPAD = 0x48,
	COMMAND_RECALL = 0xb8,
	COMMAND_READ_POWER = 0xb4,
};

class DS18S20
{
public:

	DeviceStatus Initialize(UINT16 portNum);



};

#endif
