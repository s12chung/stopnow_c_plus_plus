/*!
 * Interface for structures (objects that mainly hold data) for the Timer
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <string>
//TODO: Check these includes, include in .cpp instead?
#include <cstdio>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp> //include all types plus i/o
#include <ErrCodes.h>

#define OPTIONS_DEFAULT_DIR "./"
#define OPTIONS_DEFAULT_WORK_DURATION_SECONDS   15//600
#define OPTIONS_DEFAULT_BREAK_DURATION_SECONDS  10//300
#define OPTIONS_DEFAULT_SECONDS_UNTIL_IDLE      180

namespace TimerStructs {

//! string is hour:min:sec, 2 chars each, padded with 0s
std::string toTimeString(boost::posix_time::time_duration & duration);

//! toStringHour, Min, Sec, must be padded with 0s such that the string is of length 2
std::string toStringHour(boost::posix_time::time_duration & duration);
std::string toStringMin(boost::posix_time::time_duration & duration);
std::string toStringSec(boost::posix_time::time_duration & duration);

/*!
 * Creates a duration object from the given string and returns it to the
 * duration parameter
 *
 * Steve: duration not passed by pointer since it caused a strange segfault when
 * the method that calls it ends
 *
 * returns error code if parameter string is not
 * a string that can be created by std::string to_simple_string(time_duration)
 */
bool timeStringCheck(std::string * s);

	struct Options {
		//!duration work on the computer
		boost::posix_time::time_duration workDuration;
		//!duration for break
		boost::posix_time::time_duration breakDuration;
		//!seconds until the idle time is reached and the work duration is reset
		unsigned int secondsUntilIdle;
	};

	int loadOptions(Options * options, std::string * dir);
	int saveOptions(Options * options, std::string * dir);
}
#endif /* STRUCTS_H_ */
