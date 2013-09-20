#include <windows/WindowsCalls.h>
#include <windows.h>
#include <iostream>

int getLastInputTime(void); //prototype

//stores the lastInputTime in order to check if the user has potentially become idle
static int lastInputTime = getLastInputTime();

inline bool idleReached(unsigned int secondsUntilIdle) {
	return !(((GetTickCount() - lastInputTime)/1000) < secondsUntilIdle);
}

//returns the time from system startup to last input in milliseconds
int getLastInputTime(void) {
	LASTINPUTINFO lastInputInfo;
	lastInputInfo.cbSize = sizeof(LASTINPUTINFO);
	bool success = GetLastInputInfo(&lastInputInfo);
	if(!success) {
		int err=GetLastError(); // report error, throw exception, etc
		//TODO: how to handle this properly??
		std::cerr<<"FATAL Error: Unable to obtain Idle Time"<<std::endl;
		std::cerr<<err<<std::endl;
		exit(1);
	}
	int lastInputTime = lastInputInfo.dwTime;
	return lastInputTime;
}

int WindowsCalls::getUserIdleTime() {
	int totalTime = GetTickCount(); //gets the time from system startup to now in milliseconds
	lastInputTime = getLastInputTime();
	int idleTime = totalTime - lastInputTime;
	return idleTime/1000;
}

bool WindowsCalls::isUserIdle(unsigned int secondsUntilIdle) {
	if ( !idleReached(secondsUntilIdle) ) {
		return false;
	} else {
		lastInputTime = getLastInputTime();
		return ( idleReached(secondsUntilIdle) );
	}
}
