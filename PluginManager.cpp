#include <plugin/PluginManager.h>

PluginManager::PluginManager() : pluginList() {
	//for now, creates a python plugin
	Plugin* p = new PythonPlugin();
	p->loadDefaultSettings();
	addPlugin(p);
	activatePlugin(0);
}

PluginManager::~PluginManager() {
	for (int i=0; i < pluginList.size(); i++) delete pluginList[i];
}

//! Load all plugins from dir
void PluginManager::loadPlugins(std::string dir) {
}

//! Activate plugin with id (ids <-> index in pluginList)
void PluginManager::activatePlugin(int id) {
	assert( id < pluginList.size() );
	pluginList[id]->activate();
}

//! Deactivate plugin with id (ids <-> index in pluginList)
void PluginManager::deactivatePlugin(int id) {
	assert( id < pluginList.size() );
	pluginList[id]->deactivate();
}

//Events, go through all the plugins and checks if its active, if so call function
void PluginManager::startBreakEvent(int duration) {
	//Is this what we want? all plugins are break event plugins?
	//Also, will need threading here to simultaneously run breaks (currently runs sequentially)
	for (int i=0; i < pluginList.size(); i++) {
		if (pluginList[i]->getActive() && (pluginList[i]->getSettings()).breakEvent){
			pluginList[i]->startBreakEvent(duration);
		}
	}
}

void PluginManager::endBreakEvent() {
	for (int i=0; i < pluginList.size(); i++) {
		pluginList[i]->endBreakEvent();
	}
}

void PluginManager::addPlugin(Plugin* plugin) {
	pluginList.push_back(plugin);
}


