#include "Sessions.h"


SessionManager GSM;

bool SessionManager::Add(UINT16 no){
	if(IsPresent(no)){
		return false;
	}
	if(numberOfSessions>=MAX_SESSIONS) return false;
	sess[numberOfSessions]=no;
	numberOfSessions++;
}
bool SessionManager::IsPresent(UINT16 no){
	for (int i=0; i<MAX_SESSIONS; i++){
		if(sess[i]==no){
			return true;
		}
	}
	return false;
}
bool SessionManager::Delete(UINT16 no){
	for (int i=0; i<MAX_SESSIONS; i++){
		if(sess[i]==no){
			if(i<numberOfSessions){
				sess[i]=sess[numberOfSessions];
			}else {
				sess[i]=0;
			}
			numberOfSessions--;
			return true;
		}
	}
	return false;

}

UINT8 SessionManager::Length(){
	return numberOfSessions;
}

void SessionManager::Init(){
	for (int i=0; i<MAX_SESSIONS; i++){
		sess[i]=0;
	}
	numberOfSessions=0;
}
