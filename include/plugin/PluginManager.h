#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_
#include "Plugin.h"
#include "PythonPlugin.h"
#include <vector>
#include <string>
#include <map>

//! Handles the instances of the plugins
class PluginManager {
	std::vector<Plugin *> pluginList;
public:
	PluginManager();
	~PluginManager();

	//! Load all plugins from dir
	void loadPlugins(std::string dir);

	//! Activate plugin with id (ids <-> index in pluginList)
	void activatePlugin(int id);

	//! De-activate plugin with id (ids <-> index in pluginList)
	void deactivatePlugin(int id);

	//Events, go through all the plugins and checks if its active, if so call function
	void startBreakEvent(int duration);
	void endBreakEvent();

	//TODO: ben impl
	/*!
	 * Load plugin settings for each plugin from a file.
	 * If the file doesn't exist, load the default settings. (if one does, print to console for confirmation)
	 */
	void loadPluginSettings();
	//! Saves plugin settings for each plugin to a file (just print to console)
	void savePluginSettings();

private:
	void addPlugin(Plugin* plugin);
};

#endif /* PLUGINMANAGER_H_ */
