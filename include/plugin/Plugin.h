/*
 * Interface for Plugin for Timer to interact with.
 * Allows custom plugins to be created.
 */

#ifndef PLUGIN_H_
#define PLUGIN_H_

#include <map>
#include <string>

//! Interface for plugin
class Plugin {
	static unsigned int nextId() {
		static unsigned int nextId = 0;
		return nextId++;
	}

protected:
	bool active;
	const int id;

	Plugin() : active(false), id(nextId()) {
	}

public:
	struct PluginSettings {
		bool breakEvent;
	};

	virtual ~Plugin() {

	}

	int getId() {
		return id;
	}

	bool getActive() {
		return active;
	}

	virtual void activate() {
		active = true;
	}

	virtual void deactivate() {
		active = false;
	}

	//Plugin implementation methods (we link code to allow the plugin developers implement)

	//! Aka. breakStart
	virtual void startBreakEvent(int duration) = 0;

	//! Aka. breakEnd
	virtual void endBreakEvent() = 0;

	//TODO: ben impl (try to make the plugin do the implementation)
	//! load the default settings from the plugin
	virtual void loadDefaultSettings() = 0;

	//Ideas for events:
//	virtual void fullScreenEvent() = 0;
//
//	virtual void boredEvent() = 0;
//
//	virtual void trackedAppEvent(std::string appName) = 0;

protected:
	PluginSettings settings;
public:
	PluginSettings & getSettings() {
		return settings;
	}

	void setBreakEvent(bool breakEvent) {
		breakEvent = breakEvent;
	}
};

////BOOST EXPORTING
//BOOST_PYTHON_MODULE(getting_started1) {
//	// Add regular functions to the module.
//	def("getLastInputTime", getLastInputTime);
//}

#endif /* PLUGIN_H_ */
