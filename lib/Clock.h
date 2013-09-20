/*
 * Header for the clock, which creates a new tread that calls function every cycleInMilli milliseconds.
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include <boost/thread/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/exception/exception.hpp>
#include <boost/date_time/time.hpp>

//! Class wrapper for Clock to call a function
class Clockable {
public:
	virtual void function() = 0;
};

//! Container for second thread that calls a Clockable::function when cycleInMilli elapses
class Clock {
	unsigned int cycleInMilli;
	Clockable *obj;
	boost::system_time systime;
	boost::thread * threadMain;
	bool playing;

	//! for pause
	boost::condition_variable playCond;
	boost::mutex playMut;

	//! for changing callFunction
	boost::mutex inFunct;

	void pauseCheck();
	void cycle();
public:
	Clock(unsigned int cycleInMilli, Clockable *obj);
	~Clock();

	int run();

	void play();
	void pause();

	bool isPlaying();

	void callFunction();
	void setFunction(Clockable *obj);

	unsigned int getCycleInMilli();
	//! Changes on the next cycle
	void setCycleInMilli(unsigned int cycleInMilli);
};


#endif /* CLOCK_H_ */
