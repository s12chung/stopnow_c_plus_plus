/*
 * Unix shell implementation of the GUI.
 */

#ifndef GUISHELL_H_
#define GUISHELL_H_

#include <GUI.h>
#include <Timer.h>
#include <plugin/PluginManager.h>
#include <map>
#include <vector>
#include <string>

//! Shell implemented GUI
class GUIShell : public GUI {
protected:
	//! Pointer type for the functions of the commands
	typedef void (GUIShell::*GUIShellFunction)(std::vector<std::string>&, std::string&);
	//! Struct to hold the syntax and associated function of a command
	struct Command {
		std::string syntax;
		GUIShellFunction function;
	};

	//! Map of command objects
	typedef std::map<std::string, Command> ShellMap;
	typedef std::pair<std::string, Command> ShellPair;

	ShellMap shellMap;
	//! Instance of time that the GUI works with
	Timer * timer;
	//! Pointer to the options that the timer has
	TimerStructs::Options * options;
	//! Instance of the plugin manager
	PluginManager * pluginManager;

	virtual void readOptions(TimerStructs::Options &options);
	virtual void setCommands();

	//Runable Commands

	virtual void quit();
	/*!
	 * Commands other than quit. Takes two parameters:
	 * argv = the string typed
	 * syntax = syntax of the command
	 */
	void breakStart(std::vector<std::string> &argv, std::string &syntax);
	void playPause(std::vector<std::string> &argv, std::string &syntax);
	void play(std::vector<std::string> &argv, std::string &syntax);
	void pause(std::vector<std::string> &argv, std::string &syntax);
	void add(std::vector<std::string> &argv, std::string &syntax);
	void set(std::vector<std::string> &argv, std::string &syntax);
	void decrement(std::vector<std::string> &argv, std::string &syntax);

public:
	GUIShell();
	virtual ~GUIShell();
	//! Function used to centralize checking the state of the Timer
	virtual void refresh(boost::posix_time::time_duration duration, bool onBreak);
	void run();
};

#endif /* GUISHELL_H_ */
