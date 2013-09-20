#include <Structs.h>
#include <fstream>
#include <iostream>

using namespace TimerStructs;
using namespace boost::posix_time;

#define OPTIONS_FILENAME	"options.props"
//TODO:add saving/reading secondsUntilIdle

int applyDefaults(Options *options) {
	options->workDuration = seconds(OPTIONS_DEFAULT_WORK_DURATION_SECONDS);
	options->breakDuration = seconds(OPTIONS_DEFAULT_BREAK_DURATION_SECONDS);
	options->secondsUntilIdle = OPTIONS_DEFAULT_SECONDS_UNTIL_IDLE;
	return SUCCESS;
}

//note: crashes on bad file
int TimerStructs::loadOptions(Options * options, std::string * dir) {
	//TODO: remove this line once options loads and writes the seconds
	options->secondsUntilIdle = OPTIONS_DEFAULT_SECONDS_UNTIL_IDLE;

	std::string s;
	if (dir == NULL) {
		s = std::string(OPTIONS_DEFAULT_DIR);
		dir = &s;
	}

	std::ifstream inFile;

	inFile.open((*dir+OPTIONS_FILENAME).c_str(),std::ifstream::in);
	if (!inFile) {
		applyDefaults(options);
		inFile.close();
		return ERROR_OPENING_OPTION_FILE;
	}
	std::string line;
	//TODO:crashing possible here, error states
	getline(inFile,line);
	options->workDuration = duration_from_string(line);
	getline(inFile,line);
	options->breakDuration = duration_from_string(line);
	inFile.close();
	return SUCCESS;
}

int TimerStructs::saveOptions(Options * options, std::string * dir) {
	std::ofstream outfile(OPTIONS_FILENAME);
	if (outfile.is_open()) {
		outfile << to_simple_string(options->workDuration) << "\n";
		outfile << to_simple_string(options->breakDuration) << "\n";
		outfile.close();
	} else {
		outfile.close();
		return COULD_NOT_OPEN_OPTION_FILE;
	}
	return SUCCESS;
}
