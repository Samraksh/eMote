#ifndef _SESSIONS_MANAGER_H_
#define _SESSIONS_MANAGER_H_

#include <tinyhal.h>

#define MAX_SESSIONS 8

class SessionManager{
 uint16_t sess[MAX_SESSIONS];
 uint8_t numberOfSessions;

public:
 	void Init();
	bool Add(UINT16 no);
	bool IsPresent(UINT16 no);
	bool Delete(UINT16 no);
	uint8_t Length();
};


#endif //_SESSIONS_MANAGER_H_
