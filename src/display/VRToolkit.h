/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRTOOLKIT_H_
#define VRTOOLKIT_H_

#include <string>

namespace MinVR {

class VRToolkitCommand;

/**
 * Simplest extraction of all toolkits implemented in MinVR.  It is used as the communication
 * between MinVR and plugins.  It uses the Command pattern similar to the one outlined at
 * the following URL: http://www.dofactory.com/net/command-design-pattern
 */
class VRToolkit {
public:
	VRToolkit();
	virtual ~VRToolkit() {}

	virtual void execute(VRToolkitCommand& command) {
		notSupported(command.getName());
	}

	virtual void notSupported(const std::string& commandName) {
		std::cerr << commandName << "() not enabled in this Toolkit." << std::endl;
	}
};

/**
 * VRToolkitCommand is a simple interface for passing commands from the MinVR application
 * to plugins.  It holds a name and an id (usually an enum defined in the specific tooklit).
 */
class VRToolkitCommand {
public:
	VRToolkitCommand(int id, std::string name) : m_id(id), m_name(name) {}
	virtual ~VRToolkitCommand() {}

	int getId() { return m_id; }
	std::string getName() { return m_name; }

private:
	int m_id;
	std::string m_name;
};

/**
 * VRToolkitArgCommand allows commands that also pass in an argument.
 */
template<typename ArgType>
class VRToolkitArgCommand : public VRToolkitCommand {
public:
	VRToolkitArgCommand(int id, std::string name, ArgType& arg) : VRToolkitCommand(id, name), m_arg(arg) {}
	virtual ~VRToolkitArgCommand() {}

	ArgType& getArg() const {
		return m_arg;
	}

private:
	ArgType& m_arg;
};

/**
 * VRToolkitArgReturnCommand allows commands that pass in an argument and allow for a return type.
 */
template<typename ArgType, typename ReturnType>
class VRToolkitArgReturnCommand : public VRToolkitArgCommand<ArgType> {
public:
	VRToolkitArgReturnCommand(int id, std::string name, ArgType& arg) : VRToolkitArgCommand<ArgType>(id, name, arg) {}
	virtual ~VRToolkitArgReturnCommand() {}

	ReturnType getReturn() const {
		return m_return;
	}

	void setReturn(ReturnType _return) {
		m_return = _return;
	}

private:
	ReturnType m_return;

};

} /* namespace MinVR */

#endif /* VRTOOLKIT_H_ */
