#include "irrlicht.h"

#include "Skeleton.h"
#include "XnCppWrapper.h"

using namespace irr;

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
	//HERE
	//Let's make some spheres:
	//scene::ISceneNode * node = smgr->addSphereSceneNode(5.0);
 //       if (node)
 //       {
 //               node->setPosition(core::vector3df(0,10,100));
 //       }
	
	XnSkeletonJointPosition a; a.position.X = 0; a.position.Y = 10; a.position.Z = 100;

	SkeletonJoint A(1, smgr, a, 5);
	scene::ISceneNode * node1 = smgr->addSphereSceneNode(2.0);
        if (node1)
        {
                node1->setPosition(core::vector3df(-10,0,100));
                node1->setMaterialTexture(0, driver->getTexture("../../media/wall.bmp"));
                node1->setMaterialFlag(video::EMF_LIGHTING, false);
        }

	scene::ISceneNode * node2 = smgr->addSphereSceneNode(2.0);
        if (node2)
        {
                node2->setPosition(core::vector3df(10,0,100));
                node2->setMaterialTexture(0, driver->getTexture("../../media/wall.bmp"));
                node2->setMaterialFlag(video::EMF_LIGHTING, false);
        }

	////Let's make a cube and make it fly in circles
	//scene::ISceneNode *n = smgr->addCubeSceneNode();
 //   if (n){
 //       n->setMaterialTexture(0, driver->getTexture("../../media/t351sml.jpg"));
 //       n->setMaterialFlag(video::EMF_LIGHTING, false);
 //       scene::ISceneNodeAnimator* anim =
 //               smgr->createFlyCircleAnimator(core::vector3df(0,0,30), 20.0f);
 //       if (anim){
 //           n->addAnimator(anim);
 //           anim->drop();
 //       }
 //    }

	////Let's make a straight walking ninja:
	//scene::IAnimatedMeshSceneNode* anms = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/ninja.b3d"));

 //   if (anms){
 //       scene::ISceneNodeAnimator* anim = smgr->createFlyStraightAnimator(core::vector3df(100,0,60), core::vector3df(-100,0,60), 3500, true);
 //       if (anim){
 //           anms->addAnimator(anim);
 //           anim->drop();
	//	}
	//	    
	//	anms->setMaterialFlag(video::EMF_LIGHTING, false);

 //       anms->setFrameLoop(0, 13);
 //       anms->setAnimationSpeed(15);
	//	//anms->setMD2Animation(scene::EMAT_RUN);

 //       anms->setScale(core::vector3df(2.f,2.f,2.f));
 //       anms->setRotation(core::vector3df(0,-90,0));
	//	//anms->setMaterialTexture(0, driver->getTexture("../../media/sydney.bmp"));
 //   }

	////Let's add a fps style camera
	//smgr->addCameraSceneNodeFPS();
 //   device->getCursorControl()->setVisible(false);

	smgr->addCameraSceneNode();

	//Let's add a logo and diagnostics text:
	device->getGUIEnvironment()->addImage(driver->getTexture("../../media/irrlichtlogoalpha2.tga"),core::position2d<s32>(10,20));

	gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(10, 10, 400, 20));
	diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));

	int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();

	// This is the movemen speed in units per second.
	const f32 MOVEMENT_SPEED = 50.f;

	while(device->run())
	{
		// Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;
		//core::vector3df nodePosition = node->getPosition();
		a.position.X += MOVEMENT_SPEED * frameDeltaTime;
		A = a;

		printf("%f", A.getJointPosition().position.X);

		//node->setPosition(nodePosition);

		driver->beginScene(true, true, video::SColor(255,113,113,133));

		smgr->drawAll(); // draw the 3d scene
		device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw tmp(L"Movement Example - Irrlicht Engine [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}
	
	device->drop();
		
	return 0;
}
