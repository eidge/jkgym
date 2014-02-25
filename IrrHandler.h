#ifndef IRRHANDLER
#define IRRHANDLER

#include "irrlicht.h"
#include <exception>

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

//Manages the creation of the 3d engine,
//There can only be one instance of this class running at any time.
class IrrHandler{
	public:
		IrrHandler( video::E_DRIVER_TYPE deviceType = irr::video::EDT_OPENGL,
					const core::dimension2d<u32>& windowSize = (core::dimension2d<u32>(640,480)),
					u32 bits = 16,
					bool fullscreen = false,
					bool stencilbuffer = false,
					bool vsync = false,
					IEventReceiver* receiver = 0): device(createDevice(deviceType, windowSize, bits, fullscreen, stencilbuffer, vsync, receiver)),
												   driver(device->getVideoDriver()), smgr(device->getSceneManager()) {
														if(!device) throw std::exception("Could not start 3D Engine.");
														if(!already_running) already_running = true;
														else throw std::exception("There can only be one instance of IrrHandler running.");
														}
		virtual ~IrrHandler() {
			device->drop();	
		}
	protected:
		irr::IrrlichtDevice *device;
		irr::video::IVideoDriver *driver;
		irr::scene::ISceneManager *smgr;
	private:
		static bool already_running = false;

		//No copy, no assignment:
		IrrHandler(const IrrHandler &src);
		IrrHandler& operator=(const IrrHandler &src);
		//HERE
};

#endif