#include <tinyhal.h>

UINT32 ntohl(UINT32 input)
{
	input = ((input << 8) & 0xFF00FF00) | ((input >> 8) & 0xFF00FF );
	return (input << 16 | input >> 16);
}
