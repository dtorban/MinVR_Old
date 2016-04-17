
#ifndef VRGRAPHICSTOOLKIT_H_
#define VRGRAPHICSTOOLKIT_H_

#include "VRToolkit.h"

namespace MinVR {

/** Abstract base class for graphics toolkits that are implemented in plugins.  Examples
    are OpenGL, DirectX, Processing.org, Blender, etc.. 
 */
class VRGraphicsToolkit : public VRToolkit {
public:
	virtual ~VRGraphicsToolkit() {}

	/**
	 * List of available Graphics toolkit commands
	 */
	enum VRGRAPHICSCOMMAND {
		VRGRAPHICSCOMMAND_clearScreen = 0,
		VRGRAPHICSCOMMAND_setDrawBuffer = 1,
		VRGRAPHICSCOMMAND_setViewport = 2,
		VRGRAPHICSCOMMAND_disableDrawingOnOddColumns = 3,
		VRGRAPHICSCOMMAND_disableDrawingOnEvenColumns = 4,
		VRGRAPHICSCOMMAND_enableDrawingOnAllColumns = 5,
		VRGRAPHICSCOMMAND_flushGraphics = 6,
		VRGRAPHICSCOMMAND_finishGraphics = 7
	};

	/**
	 * Possible available buffers to draw
	 */
	enum VRDRAWBUFFER {
		VRDRAWBUFFER_BACK = 0,
		VRDRAWBUFFER_FRONT = 1,
		VRDRAWBUFFER_BACKLEFT = 2,
		VRDRAWBUFFER_BACKRIGHT = 3,
		VRDRAWBUFFER_FRONTLEFT = 4,
		VRDRAWBUFFER_FRONTRIGHT = 5
	};

	// Specific commands that also pass arguments in
	typedef VRToolkitArgCommand<VRDRAWBUFFER> VRDrawBufferCommand;
	typedef VRToolkitArgCommand<VRRect> VRSetViewportCommand;

	// Abstract functions that can be called for a graphics toolkit
	virtual void clearScreen() { notSupported("clearScreen"); }
	virtual void setDrawBuffer(VRDRAWBUFFER buffer) { notSupported("setDrawBuffer"); }
	virtual void setViewport(VRRect rect) { notSupported("setViewport"); }
	virtual void disableDrawingOnOddColumns() { notSupported("disableDrawingOnOddColumns"); }
	virtual void disableDrawingOnEvenColumns() { notSupported("disableDrawingOnEvenColumns"); }
	virtual void enableDrawingOnAllColumns() { notSupported("enableDrawingOnAllColumns"); }
	virtual void flushGraphics() { notSupported("flushGraphics"); }
	virtual void finishGraphics() { notSupported("finishGraphics"); }

	/**
	 * Executes commands that are passed in.  This method is necessary to keep the VRToolkit
	 * interface simple so that changing this interface will not affect plugins who implement
	 * the VRGraphicsToolkit interface.
	 */
	//
	virtual void execute(VRToolkitCommand& command) {
		switch(command.getId())
		{
		case VRGRAPHICSCOMMAND_clearScreen:
			clearScreen();
			break;
		case VRGRAPHICSCOMMAND_setDrawBuffer:
			VRDrawBufferCommand& cmd = dynamic_cast<VRDrawBufferCommand&>(command);
			setDrawBuffer(cmd.getArg());
			break;
		case VRGRAPHICSCOMMAND_setViewport:
			VRSetViewportCommand& cmd = dynamic_cast<VRSetViewportCommand&>(command);
			VRRect viewport = cmd.getArg();
			setViewport(viewport);
			break;
		case VRGRAPHICSCOMMAND_disableDrawingOnOddColumns:
			disableDrawingOnOddColumns();
			break;
		case VRGRAPHICSCOMMAND_disableDrawingOnEvenColumns:
			disableDrawingOnEvenColumns();
			break;
		case VRGRAPHICSCOMMAND_enableDrawingOnAllColumns:
			enableDrawingOnAllColumns();
			break;
		case VRGRAPHICSCOMMAND_flushGraphics:
			flushGraphics();
			break;
		case VRGRAPHICSCOMMAND_finishGraphics:
			finishGraphics();
			break;
		default:
			VRToolkit::execute(command);
			break;
		}
	}
};

/**
 * This is the VRGraphicsToolkit which actually invokes the execute call on a VRToolkit.
 * It assumes that the Toolkit that is passed in is a VRGraphicsToolkit.  This can be enforced
 * inside the MinVR factory.
 */
class VRGraphicsToolkitInvoker : public VRGraphicsToolkit {
public:
	VRGraphicsToolkitInvoker(VRToolkit& toolkit) : m_toolkit(toolkit) {}
	virtual ~VRGraphicsToolkitInvoker() {}

	void execute(VRToolkitCommand& command) {
		m_toolkit.execute(command);
	}

	void clearScreen() {
		VRToolkitCommand cmd(VRGRAPHICSCOMMAND_clearScreen, "clearScreen");
		execute(cmd);
	}

	void setDrawBuffer(VRDRAWBUFFER buffer)
	{
		VRDrawBufferCommand cmd(VRGRAPHICSCOMMAND_setDrawBuffer, "setDrawBuffer", buffer);
		execute(cmd);
	}

	void setViewport(VRRect rect) {
		VRSetViewportCommand cmd(VRGRAPHICSCOMMAND_setViewport, "setViewport", rect);
		execute(cmd);
	}

	void disableDrawingOnOddColumns() {
		VRToolkitCommand cmd(VRGRAPHICSCOMMAND_disableDrawingOnOddColumns, "disableDrawingOnOddColumns");
		execute(cmd);
	}

	void disableDrawingOnEvenColumns() {
		VRToolkitCommand cmd(VRGRAPHICSCOMMAND_disableDrawingOnEvenColumns, "disableDrawingOnEvenColumns");
		execute(cmd);
	}

	void enableDrawingOnAllColumns() {
		VRToolkitCommand cmd(VRGRAPHICSCOMMAND_enableDrawingOnAllColumns, "enableDrawingOnAllColumns");
		execute(cmd);
	}

	void flushGraphics() {
		VRToolkitCommand cmd(VRGRAPHICSCOMMAND_flushGraphics, "flushGraphics");
		execute(cmd);
	}

	void finishGraphics() {
		VRToolkitCommand cmd(VRGRAPHICSCOMMAND_finishGraphics, "finishGraphics");
		execute(cmd);
	}

private:
	VRToolkit& m_toolkit;
};



#endif /* VRGRAPHICSTOOLKIT_H_ */
