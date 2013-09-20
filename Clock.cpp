/*!
 * Implementation for the Clock Class
 *
 * State variables:
 * bool playing;
 */

#include <Clock.h>
#include <ErrCodes.h>
#include <boost/thread/condition_variable.hpp>

Clock::Clock(unsigned int cycleInMilli, Clockable *obj) {
	assert(obj != NULL);
	assert(cycleInMilli != 0);

	this->cycleInMilli = cycleInMilli;
	this->obj = obj;
	threadMain = NULL;
	systime = boost::get_system_time();
	playing = true;
}

Clock::~Clock() {
	threadMain->detach();
}

int Clock::run() {
	if (playing) {
		threadMain = new boost::thread(&Clock::cycle, this);
		return SUCCESS;
	} else {
		return ERR_CLOCK_PLAYING;
	}
}

/*!
 * CV implementation of play. From:
 * http://www.boost.org/doc/libs/1_38_0/doc/html/thread/synchronization.html#thread.synchronization.condvar_ref
 */
void Clock::play() {
	if (!playing) {
		boost::lock_guard<boost::mutex> lock(playMut);
		playing = true;
		playCond.notify_one();
	}
}

void Clock::pause() {
	playing = false;
}

bool Clock::isPlaying() {
	return playing;
}

/*!
 * CV implementation of play. From:
 * http://www.boost.org/doc/libs/1_38_0/doc/html/thread/synchronization.html#thread.synchronization.condvar_ref
 */
void Clock::pauseCheck() {
	boost::unique_lock<boost::mutex> lock(playMut);
	while (!playing) {
		playCond.wait(lock);
	}
}

void Clock::cycle() {
	for(;;) {
		systime += boost::posix_time::milliseconds(cycleInMilli);
		threadMain->sleep(systime);
		pauseCheck();
		boost::unique_lock<boost::mutex> lock(inFunct);
		callFunction();
		lock.unlock();
		if (threadMain->get_id() == boost::thread::id()) break;
	}
	delete threadMain;
}

void Clock::callFunction() {
	if (obj != NULL) {
		obj->function();
	} else {
		std::cerr << "changed" << std::endl;
	}
}

void Clock::setFunction(Clockable *obj) {
	boost::unique_lock<boost::mutex> lock(inFunct);
	assert(obj != NULL);
	this->obj = obj;
	lock.unlock();
}

unsigned int Clock::getCycleInMilli() {
	return cycleInMilli;
}
void Clock::setCycleInMilli(unsigned int cycleInMilli) {
	assert(cycleInMilli != 0);
	this->cycleInMilli = cycleInMilli;
}

