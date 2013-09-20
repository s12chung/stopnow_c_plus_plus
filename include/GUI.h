/*!
 * Interface to interact with GUI from Timer.
 */


#ifndef GUI_H_
#define GUI_H_

#include <Structs.h>

//! Interface for GUI
class GUI {
	virtual void readOptions(TimerStructs::Options &options) = 0;
	virtual void quit() = 0;
public:
	//! Refresh the timer, copies the duration so it won't mess with Timer's duration
	virtual void refresh(boost::posix_time::time_duration duration, bool onBreak) = 0;
};


#endif /* GUI_H_ */
