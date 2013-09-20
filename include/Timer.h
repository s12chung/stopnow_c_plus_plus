//! Timer Interface
/*
 * The core of the program.
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <GUI.h>
#include <Structs.h>
#include <Clock.h>
#include <plugin/PluginManager.h>

//TODO: Do we really need all this?,  move to .cpp?
#include <boost/date_time/time_resolution_traits.hpp>
#include <boost/date_time/time_defs.hpp>
#include <boost/date_time/int_adapter.hpp>
#include <boost/cstdint.hpp>

//! Class that holds the duration (or time)
class Timer : public Clockable {
	GUI *gui;
	TimerStructs::Options *options;
	PluginManager *pluginManager;
	Clock *clock;
	boost::posix_time::time_duration *duration; //! current duration, normally not accessed directly, but through durationRefresh()
	bool playing; //! Determines the state of the current program.
	bool onBreak;
	bool idle;

	//! Refreshes GUI and updates duration
	void durationRefresh(boost::posix_time::time_duration duration);

public:
	//! The Timer Constructor.
	/*!
	 * Constructs a Timer object that accepts a GUI and Options object as parameters.
	 */
	Timer(GUI * gui, TimerStructs::Options * options, PluginManager * pluginManager);
	//! The Timer Destructor.
	/*!
	 * Destroys the Timer object and any objects created on the heap by the Timer.
	 */
	virtual ~Timer();

	/*!
	 * Starts the break and gives the duration of the break to the duration object.
	 * Returns:
	 * ERR_TIMER_ONBREAK = if on break
	 */
	int breakStart();
	/*!
	 * Ends the break and resets the duration object.
	 *
	 * Returns:
	 * ERR_TIMER_ONBREAK = if not on break
	 */
	int breakEnd();

	// GUI Methods.
	/***************/
	/*!
	 * if playing==true then call the pause method.
	 * else if pausing==true then call the play method.
	 * else call the play method.
	 */
	void playPause();
	/*!
	 * Set the clock state to playing.
	 *
	 * Returns:
	 * ERR_TIMER_PLAYING = timer already playing
	 */
	int play();
	/*!
	 * Set the clock state to paused.
	 *
	 * Returns:
	 * ERR_TIMER_PLAYING = time already paused
	 */
	int pause();

	// Time Methods.
	/****************/
	/*!
	 * Add "secs" amount of duration to Time object
	 * Returns:
	 * ERR_TIMER_NEG_DURATION = operation will make this->duration negative, only occurs when "subtract" = true
	 */
	int add(boost::posix_time::time_duration & duration, bool subtract);

	/*!
	 * Resets the duration back to the workduration.
	 */
	int reset();

	//Clock Methods.
	/****************/
	//! The Clock calls this method to refresh the GUI.
	/*!
	 * The Clock calls this method to decrement the Timer and
	 * do any other operation to reflect the state of the Timer
	 * Returns:
	 * ERR_TIMER_PLAYING = if currently paused
	 */
	int decrement();

	virtual void function();

};

#endif /* TIMER_H_ */
