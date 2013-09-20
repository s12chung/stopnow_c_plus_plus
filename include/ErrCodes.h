#ifndef ERRORCODES_H_
#define ERRORCODES_H_

/*
 * Syntax:
 * 								  V- tab until it reaches the same point as SUCESS
 * #define ERR_<ERROR CODE NAME>	1 <--the value (can always change)
 * Example:
 * #define ERR_FILE_NOT_FOUND		1
 */

#define SUCCESS			0

#define ERR_TIMER_PLAYING 			1200
#define ERR_TIMER_ONBREAK 			1210
#define ERR_TIMER_NEG_DURATION		1220

#define ERR_CLOCK_PLAYING			1100

//1000-1099 range for options
#define ERROR_OPENING_OPTION_FILE	1000	//error opening option file, defaults applied
#define COULD_NOT_OPEN_OPTION_FILE	1010

//time related errors
#define ERR_INVALID_TIME_STRING		2020

#endif /* ERRORCODES_H_ */
