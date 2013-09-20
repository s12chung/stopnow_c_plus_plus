//! Timer Implementation
/*!
 * This is the implementation of timer.
 *
 * States variables:
 * bool playing;
 * bool onBreak;
 */
#include <Timer.h>
#include <windows/WindowsCalls.h>
#include <ErrCodes.h>

using namespace TimerStructs;
using namespace boost::posix_time;

/*!
 * Saves the default parameters to the class variables and initialises the duration object with zeroes as its parameter.
 * Sets the starting state to not playing and not pausing.
 */
Timer::Timer(GUI * gui, Options * options, PluginManager * pluginManager) :
		gui(gui),
		options(options),
		pluginManager(pluginManager),
		clock( new Clock(1000, this) ),
		duration( new time_duration((options->workDuration).hours(), (options->workDuration).minutes(), (options->workDuration).seconds(), 0)),
		playing(true),
		onBreak(false) {
	gui->refresh(*duration, onBreak);
	clock->run();
}

/*!
 * Destroys the duration object.
 */
Timer::~Timer() {
	delete duration;
	delete clock;
}

void Timer::durationRefresh(time_duration duration) {
	assert(gui!=NULL);
	*this->duration = duration;
	gui->refresh(duration, onBreak);

	if (this->duration->total_seconds() == 0) {
		std::cout << std::endl;
		//if on break, then end the break. otherwise, start break
		if (onBreak)
			breakEnd();
		else
			breakStart();
	}
}

/*!
 * Starts the break and gives the duration of the break to the duration object.
 * Returns:
 * ERR_TIMER_ONBREAK = if on break
 */
int Timer::breakStart() {
	if (!onBreak) {
		onBreak = true;
		pluginManager->startBreakEvent((options->breakDuration).total_seconds());
		durationRefresh(options->breakDuration);
		//temporary creation of break object
//		Plugin * p = new Plugin();
//		p->coreTimedEvent((options->breakDuration).total_seconds());
//		delete p;
	}
	else {
		return ERR_TIMER_ONBREAK;
	}
	return SUCCESS;
}

/*!
 * Ends the break and resets the duration object.
 *
 * Returns:
 * ERR_TIMER_ONBREAK = if not on break
 */
int Timer::breakEnd() {
	if (onBreak) {
		onBreak = false;
		pluginManager->endBreakEvent();
		durationRefresh(options->workDuration);
//		Plugin * p = new Plugin(); //this needs to be figured out later, just testing for now
//		p->coreTimedEventEnd();
//		delete p;
	}
	else {
		return ERR_TIMER_ONBREAK;
	}
	return SUCCESS;
}

/*!
 * if playing==true then call the pause method.
 * else if pausing==true then call the play method.
 * else call the play method.
 */
void Timer::playPause() {
	if (playing) {
		this->pause();
	}
	else {
		this->play();
	}
}

/*!
 * Set the clock state to playing.
 *
 * Returns:
 * ERR_TIMER_PLAYING = timer already playing
 */
int Timer::play() {
	if (playing) {
		return ERR_TIMER_PLAYING;
	}
	playing = true;
	clock->play();
	return SUCCESS;
}

/*!
 * Set the clock state to paused.
 *
 * Returns:
 * ERR_TIMER_PLAYING = time already paused
 */
int Timer::pause() {
	clock->pause();
	if (!playing) {
		return ERR_TIMER_PLAYING;
	}
	playing = false;
	return SUCCESS;
}

/*!
 * Add "secs" amount of duration to Time object
 * Returns:
 * ERR_TIMER_NEG_DURATION = operation will make this->duration negative
 */
int Timer::add(time_duration & duration, bool subtract) {
	if (!subtract)
		durationRefresh(*(this->duration) + duration);
	else {
		if (duration > *(this->duration)) return ERR_TIMER_NEG_DURATION;
		durationRefresh(*(this->duration) - duration);
	}
	return SUCCESS;
}

/*!
 * Resets the workduration
 */
int Timer::reset() {
	delete duration;
	duration = new time_duration((options->workDuration).hours(), (options->workDuration).minutes(), (options->workDuration).seconds(), 0);
	return SUCCESS;
}

/*!
 * The Clock calls this method to decrement the Timer and
 * do any other operation to reflect the state of the Timer
 * Returns:
 * ERR_TIMER_PLAYING = if currently paused
 */
int Timer::decrement() {
	if (!playing) return ERR_TIMER_PLAYING;

	durationRefresh(*duration - seconds(1));
	return SUCCESS;

}

void Timer::function() {
	//Purely input based.. what if the user is watching a video?
	if (!(WindowsCalls::isUserIdle(options->secondsUntilIdle))) {
		Timer::decrement();
		idle = false;
	} else if (!idle) {
		Timer::reset();
		durationRefresh(*duration);
		idle = true;
	}
}
