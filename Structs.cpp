#include <Structs.h>
#include <string>
#include <boost/algorithm/string/split.hpp>
#include <boost/regex.hpp>

using namespace std;
using namespace boost::posix_time;
//! length of the padded string (including escape char)
#define PADDED_STRING_LENGTH 	3

//! returns a two digit string padded with zeros
inline string padded2DString(int x) {
	char buffer[PADDED_STRING_LENGTH];
	snprintf(buffer, PADDED_STRING_LENGTH, "%02d", x);
	return string(buffer);
}

//TODO: what is the max value for timeString hours? takes up to 99 for now....
bool TimerStructs::timeStringCheck(std::string * s) {
	//perl regular expressions as default, yay for regex
	boost::regex expression("([0-9]{1,2}):([0-5]?[0-9]):([0-5]?[0-9])(.[0-9]{1,6})?");
	return regex_match(*s, expression);
}

string TimerStructs::toTimeString(time_duration & duration) {
	return (to_simple_string(duration)).substr(0,8);
}

string TimerStructs::toStringHour(time_duration & duration) {
	return padded2DString(duration.hours());
}
string TimerStructs::toStringMin(time_duration & duration) {
	return padded2DString(duration.minutes());
}
string TimerStructs::toStringSec(time_duration & duration) {
	return padded2DString(duration.seconds());
}
