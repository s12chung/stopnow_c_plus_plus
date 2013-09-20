#include <impl/GUIShell.h>
#include <iostream>

using namespace std;
using namespace TimerStructs;
using namespace boost::posix_time;

#define CMD_TT_WORKB 	"w"
#define CMD_TT_BREAKB 	"b"


int main(int argc, char *argv[]) {
	GUIShell gui;
	gui.run();
}

/*!
 * Breaks a std::string delimited by space(s) into tokens in the std::vector. Note that the std::vector will
 * be cleared before use.
 */
inline void stringSplit(const string &line, vector<string> &v) {
	v.clear();
	istringstream stream(line);
	string curr;

	for (;;) {
		stream >> curr;
		if (!stream) break;
		v.push_back(curr);
	}
}

GUIShell::GUIShell() {
	options = new Options;
	if (loadOptions(options, NULL) == ERROR_OPENING_OPTION_FILE) {
		cout << "Options File not existant: applying default options" << std::endl;
	}
	pluginManager = new PluginManager();
	timer = new Timer(this, options, pluginManager);
}

GUIShell::~GUIShell() {
	delete options;
	delete timer;
	delete pluginManager;
}

void GUIShell::readOptions(Options &options) {
}

void GUIShell::setCommands() {
	Command curr;

	//Break
	curr.syntax = "b(reak)";
	curr.function = &GUIShell::breakStart;
	shellMap.insert(ShellPair("b", curr));
	shellMap.insert(ShellPair("break", curr));

	//Play/Pause
	curr.syntax = "p(layPause)";
	curr.function = &GUIShell::playPause;
	shellMap.insert(ShellPair("p", curr));
	shellMap.insert(ShellPair("playPause", curr));
	curr.syntax = "play";
	curr.function = &GUIShell::play;
	shellMap.insert(ShellPair("play", curr));
	curr.syntax = "pause";
	curr.function = &GUIShell::pause;
	shellMap.insert(ShellPair("pause", curr));

	//add
	curr.syntax = "a(dd) [-]hh:mm:ss";
	curr.function = &GUIShell::add;
	shellMap.insert(ShellPair("a", curr));
	shellMap.insert(ShellPair("add", curr));

	//decrement
	curr.syntax = "d(ecrement)";
	curr.function = &GUIShell::decrement;
	shellMap.insert(ShellPair("d", curr));
	shellMap.insert(ShellPair("decrement", curr));

	//Set break duration/work duration
	curr.syntax = "s(et) TIMETYPE hh:mm:ss";
	curr.function = &GUIShell::set;
	shellMap.insert(ShellPair("s", curr));
	shellMap.insert(ShellPair("set", curr));
}

void GUIShell::refresh(time_duration duration, bool onBreak) {
	std::string timeString = TimerStructs::toTimeString(duration);
//	for (int i = 0; i < timeString.length()+1; i++) cout << "\b";
	cout << "\r";
	if (!onBreak) cout << "Work  ";
	else          cout << "Break ";
	cout << timeString << " ";
	cout.flush();
}

void GUIShell::run() {
	setCommands();
	string in;
	vector<string> v;

	for (;;) {
		getline(cin, in);
		stringSplit(in, v);

		ShellMap::iterator find = shellMap.find(v.front());

		if (find != shellMap.end()) {
			(this ->* ((find->second).function))(v, (find->second).syntax);
		}
		else if (!v.front().compare("q") || !v.front().compare("quit")) {
			quit();
			break;
		}
		else {
			cout << "Invalid command" << endl;
		}
	}
}


void GUIShell::quit() {
	saveOptions(options, NULL);
	cout << "Quit" << endl;
}

void GUIShell::breakStart(vector<string> &argv, string &syntax) {
	cout << "Trying to breakStart()" << endl;
	const int ret = (*timer).breakStart();
	if (ret == ERR_TIMER_ONBREAK) {
		cout << "Already on break" << endl;
	}
}

void GUIShell::play(vector<string> &argv, string &syntax) {
	cout << "Trying to play()" << endl;
	const int ret = (*timer).play();
	if (ret == ERR_TIMER_PLAYING) {
		cout << "Already playing" << endl;
	}
}

void GUIShell::pause(vector<string> &argv, string &syntax) {
	cout << "Trying to pause()" << endl;
	const int ret = (*timer).pause();
	if (ret == ERR_TIMER_PLAYING) {
		cout << "Already paused" << endl;
	}
}

void GUIShell::playPause(vector<string> &argv, string &syntax) {
	cout << "Trying to playPause()" << endl;
	(*timer).playPause();
}

void GUIShell::add(vector<string> &argv, string &syntax) {
	if (argv.size() < 2) {
		cout << syntax << endl;
	}
	else {
		vector<string>::iterator it = argv.begin();
		it++;

		time_duration duration;

		//handle negative case
		if (((string) (*it)).at(0) == '-') {
			string timeString = ((string)(*it)).substr(1);
			if (timeStringCheck(&timeString)) {
				duration = duration_from_string(timeString);
				cout << "Adding -" << toTimeString(duration) << endl;
				if (timer->add(duration, true) == -1) {
					cout << "Adding -" << toTimeString(duration)
					<< " will make Timer negative" << endl;
				}
			} else {
				cout << "Invalid time entered, syntax is [-]hh:mm:ss[.ffffff]" << endl;
			}
		}
		else {
			string timeString = (string)(*it);
			if (timeStringCheck(&timeString)) {
				duration = duration_from_string(timeString);
				cout << "Adding " << toTimeString(duration) << endl;
				timer->add(duration, false);
			} else {
				cout << "Invalid time entered, syntax is [-]hh:mm:ss[.ffffff]" << endl;
			}
		}
	}
}

void GUIShell::decrement(vector<string> &argv, string &syntax) {
	const int ret = (*timer).decrement();
	if (ret == ERR_TIMER_PLAYING) {
		cout << "Trying to decrement with pause" << endl;
	}
}

void GUIShell::set(vector<string> &argv, string &syntax) {
	vector<string>::iterator it = argv.begin();
	it++;
	string timetype = *it;
	if (argv.size() < 3) {
		cout << syntax << endl;
	}
	else {
		it++;
		time_duration val = duration_from_string((string) * it);

		//handle the different time types
		if (!timetype.compare(CMD_TT_WORKB)) {
			options->workDuration = val;
			cout << "Setting workDuration to " << toTimeString(val) << endl;
		}
		else if (!timetype.compare(CMD_TT_BREAKB)) {
			options->breakDuration = val;
			cout << "Setting breakDuration to " << toTimeString(val) << endl;
		}
		else {
			cout << syntax << endl;
		}
	}
}
