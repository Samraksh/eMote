#include <tinyhal.h>
#ifdef __cplusplus
extern "C" {
#endif


void debugBT_printf(const char* message){
	hal_printf("debugBT called: %s\r\n", message);
}

#ifdef __cplusplus
}
#endif

