#include <plugin/PythonPlugin.h>

//Boost python includes:
#include <boost/python.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
using namespace boost::python;

PythonPlugin::PythonPlugin() : Plugin(){

}

PythonPlugin::~PythonPlugin() {

}

void PythonPlugin::activate() {
	Plugin::activate();
	Py_Initialize();
}

void PythonPlugin::startBreakEvent(int duration) {
//	Py_Initialize();
	FILE* exp_file;
	PyObject* main_module, * global_dict, * expression;
	exp_file = fopen("cpptopython.py", "r");

	//runs from main
	PyRun_SimpleFile(exp_file, "cpptopython.py");

	// Get a reference to the main module
	// and global dictionary
	main_module = PyImport_AddModule("__main__");
	global_dict = PyModule_GetDict(main_module);
	// Extract a reference to the function "func_name"
	// from the global dictionary
	expression = PyDict_GetItemString(global_dict, "startBreak");
	PyObject_CallFunction(expression, NULL);
}

void PythonPlugin::endBreakEvent() {
	FILE* exp_file;
	PyObject* main_module, * global_dict, * expression;
	exp_file = fopen("cpptopython.py", "r");
	PyRun_SimpleFile(exp_file, "cpptopython.py");

	// Get a reference to the main module
	// and global dictionary
	main_module = PyImport_AddModule("__main__");
	global_dict = PyModule_GetDict(main_module);
	// Extract a reference to the function "func_name"
	// from the global dictionary
	expression = PyDict_GetItemString(global_dict, "notifyEnd");
	PyObject_CallFunction(expression, NULL);
}

void PythonPlugin::loadDefaultSettings() {
	settings.breakEvent = true;
}



