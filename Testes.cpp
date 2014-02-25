#include "irrlicht.h"

#include "Skeleton.h"
#include "XnCppWrapper.h"

using namespace irr;

//DEV
#include <iostream>
using namespace std;
//ENDDEV

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800,600), 32, false, false, false);
	if(!device)
		return 1;
	
	device->setWindowCaption(L"jKgym - Hugo Ribeira");

	//Pointers for video and scene manager
	video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();

	scene::ICameraSceneNode *camera = smgr->addCameraSceneNode();
	camera->setTarget( core::vector3df(f32(0.0),f32(200.0),f32(50.0)) );

	//Simulate Kinect Input
	XnSkeletonJointPosition head_kinect;
	head_kinect.position.X = 0;
	head_kinect.position.Y = 200;
	head_kinect.position.Z = 15;

	XnSkeletonJointPosition lshoulder_kinect;
	lshoulder_kinect.position.X = 20;
	lshoulder_kinect.position.Y = 200;
	lshoulder_kinect.position.Z = 30;

	XnSkeletonJointPosition rshoulder_kinect;
	rshoulder_kinect.position.X = -20;
	rshoulder_kinect.position.Y = 200;
	rshoulder_kinect.position.Z = 30;

	
	Skeleton guy(smgr, driver);
	guy.jointAdd(1, head_kinect, 10.0);
	guy.jointAdd(2, lshoulder_kinect, 5.0);
	guy.jointAdd(3, rshoulder_kinect, 5.0);

	try{
		while(device->run()){
			driver->beginScene(true, true, video::SColor(255,113,113,133));
			//-------------------------------



			//-------------------------------
			smgr->drawAll(); // draw the 3d scene
			driver->endScene();
		}
	}
	catch(std::exception e){
		cout << e.what();
	}

	return 0;
}
