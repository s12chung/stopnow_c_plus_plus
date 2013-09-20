#ifndef PYTHONPLUGIN_H_
#define PYTHONPLUGIN_H_
#include "Plugin.h"

//! Representation of a plugin implemented in Python
class PythonPlugin : public Plugin {
	static unsigned int activeInstances;// = 0, from below
public:
	PythonPlugin();
	~PythonPlugin();

	virtual void activate();
	virtual void startBreakEvent(int duration);
	virtual void endBreakEvent();
	virtual void loadDefaultSettings();
};
//int unsigned PythonPlugin::activeInstances = 0;

#endif /* PYTHONPLUGIN_H_ */
