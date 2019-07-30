package DeviceManager

import ()

type SessionManager struct {
	activeSessions []uint16
}

//instantiate a global session manager
var GSM SessionManager = SessionManager{}

func (sm *SessionManager) IsPresent(x uint16) int {
	for i := range sm.activeSessions {
		if sm.activeSessions[i] == x {
			return i
		}
	}
	return -1
}

func (sm *SessionManager) Add(x uint16) bool {
	if sm.IsPresent(x) == -1 {
		//fmt.Println("Adding new ")
		sm.activeSessions = append(sm.activeSessions, []uint16{x}...)
		return true
	} else {
		return false
	}
}

func (sm *SessionManager) Delete(x uint16) bool {
	i := sm.IsPresent(x)
	if i == -1 {
		return false
	} else {
		sm.activeSessions = append(sm.activeSessions[:i], sm.activeSessions[i+1:]...)
		return true
	}
}

func (sm *SessionManager) Length() int {
	return len(sm.activeSessions)
}
