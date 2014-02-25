/****************************************************************************
*                                                                           *
*  jKGym									                                *
*  Copyright (C) 2012 Hugo Ribeira                                          *
*                                                                           *
*  This file is part of jKGym.                                              *
*                                                                           *
*  jKGym is free software: you can redistribute it and/or modify            *
*  it under the terms of the GNU Lesser General Public License as published *
*  by the Free Software Foundation, either version 3 of the License, or     *
*  (at your option) any later version.                                      *
*                                                                           *
*  jKGym is distributed in the hope that it will be useful,                 *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
*  GNU Lesser General Public License for more details.                      *
*                                                                           *
*  You should have received a copy of the GNU Lesser General Public License *
*  along with jKGym. If not, see <http://www.gnu.org/licenses/>.            *
*                                                                           *
****************************************************************************/
//Includes:
#include <vector>
#include <map>

//Main
#include "Skeleton.h"
#include "Exercise.h"

//Kinect
#include <XnCppWrapper.h>
#include "openni_callbacks.h"

#define SAMPLE_XML_PATH "./Data/SamplesConfig.xml"
#define SAMPLE_XML_PATH_LOCAL "SamplesConfig.xml"

#define MAX_NUM_USERS 15
std::map<XnUserID, Skeleton> UserSkeletons;

//IrrLicht
#include "irrlicht.h"

using namespace irr;

//No command line
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif



int main()
{

    XnStatus nRetVal = XN_STATUS_OK;
    xn::EnumerationErrors errors;

	//------------------------------Kinect and Irrlicht Setup-----------------------------------------// 
	//Import xml config file
    const char *fn = NULL;
    if    (fileExists(SAMPLE_XML_PATH)) fn = SAMPLE_XML_PATH;
    else if (fileExists(SAMPLE_XML_PATH_LOCAL)) fn = SAMPLE_XML_PATH_LOCAL;
    else {
        printf("Could not find '%s' nor '%s'. Aborting.\n" , SAMPLE_XML_PATH, SAMPLE_XML_PATH_LOCAL);
        return XN_STATUS_ERROR;
    }
    printf("Reading config from: '%s'\n", fn);

    nRetVal = g_Context.InitFromXmlFile(fn, g_scriptNode, &errors);
    if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
    {
        XnChar strError[1024];
        errors.ToString(strError, 1024);
        printf("%s\n", strError);
        return (nRetVal);
    }
    else if (nRetVal != XN_STATUS_OK)
    {
        printf("Open failed: %s\n", xnGetStatusString(nRetVal));
        return (nRetVal);
    }

	//Activate Depth and skeleton tracking
    nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
    CHECK_RC(nRetVal,"No depth");

    nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_USER, g_UserGenerator);
    if (nRetVal != XN_STATUS_OK)
    {
        nRetVal = g_UserGenerator.Create(g_Context);
        CHECK_RC(nRetVal, "Find user generator");
    }

    XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected;
    if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
    {
        printf("Supplied user generator doesn't support skeleton\n");
        return 1;
    }
	//Register event callbacks
    nRetVal = g_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
    CHECK_RC(nRetVal, "Register to user callbacks");
    nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationStart(UserCalibration_CalibrationStart, NULL, hCalibrationStart);
    CHECK_RC(nRetVal, "Register to calibration start");
    nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete(UserCalibration_CalibrationComplete, NULL, hCalibrationComplete);
    CHECK_RC(nRetVal, "Register to calibration complete");

	//Check if Calibration Pose is needed
    if (g_UserGenerator.GetSkeletonCap().NeedPoseForCalibration())
    {
        g_bNeedPose = TRUE;
        if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
        {
            printf("Pose required, but not supported\n");
            return 1;
        }
        nRetVal = g_UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected(UserPose_PoseDetected, NULL, hPoseDetected);
        CHECK_RC(nRetVal, "Register to Pose Detected");
        g_UserGenerator.GetSkeletonCap().GetCalibrationPose(g_strPose);
    }

    g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

    nRetVal = g_Context.StartGeneratingAll();
    CHECK_RC(nRetVal, "StartGenerating");

    XnUserID aUsers[MAX_NUM_USERS];
    XnUInt16 nUsers;
    XnSkeletonJointTransformation Joint;

    printf("Starting to run\n");
    if(g_bNeedPose)
    {
        printf("Assume calibration pose\n");
    }

	//Let's now start IrrLicht:
		IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1400,800), 32, false, false, false, NULL);
		if(!device)
			return 1;
		
		device->setWindowCaption(L"jKgym - Alpha");

		//Some pointers to IrrLicht, will come in handy soon:
		video::IVideoDriver *driver = device->getVideoDriver();
		scene::ISceneManager *smgr = device->getSceneManager();
		irr::gui::IGUIEnvironment* env = device->getGUIEnvironment();

		Skeleton::driver = driver;
		Skeleton::smgr   = smgr;
		

		device->getGUIEnvironment()->addImage(driver->getTexture("C:/Users/Hugo/Dropbox/jeKnowledge/Tecnologia/Design e Multimédia/Logo/logo-2cor-01 600pxl width.png"),core::position2d<s32>(-50,-150));
		
		//Let's make a sphere to represent the head:
		scene::ISceneNode *head_sphere = smgr->addSphereSceneNode(150.0);
		if (head_sphere){
            head_sphere->setMaterialTexture(0, driver->getTexture("../../media/wall.bmp"));
            head_sphere->setMaterialFlag(video::EMF_LIGHTING, false);
		}

		scene::ISceneNode *rhand_sphere = smgr->addSphereSceneNode(150.0);
		if (rhand_sphere){
            head_sphere->setMaterialTexture(0, driver->getTexture("../../media/wall.bmp"));
            head_sphere->setMaterialFlag(video::EMF_LIGHTING, false);
		}

		scene::ISceneNode *lhand_sphere = smgr->addSphereSceneNode(150.0);
		if (lhand_sphere){
            head_sphere->setMaterialTexture(0, driver->getTexture("../../media/wall.bmp"));
            head_sphere->setMaterialFlag(video::EMF_LIGHTING, false);
		}

		//Finally we add a camera:
		scene::ICameraSceneNode *camera = smgr->addCameraSceneNode();
		int fps;

		//GUI FONT
		irr::gui::IGUISkin *skin = env->getSkin();
		irr::gui::IGUIFont* font = env->getFont("../../media/RepFont.png");

		if (font)
			skin->setFont(font);
		else
			throw std::exception("Couldn't find font.");

		char Reps[20];
		
		auto staticte =  env->addStaticText(L"0", irr::core::rect<s32>(900,400,1000,600),false);

		

		

	//--------------------------------------------Main Loop------------------------------------------------//
	//DEV
	Exercise Bicep(Relation(XN_SKEL_RIGHT_HAND, XN_SKEL_RIGHT_ELBOW, XN_SKEL_RIGHT_SHOULDER, 30, -100, "ContaReps"));
	Bicep.addRelation(XN_SKEL_RIGHT_ELBOW, XN_SKEL_RIGHT_SHOULDER, 10, 10, "RBicep");
	Bicep.addRelation(XN_SKEL_LEFT_ELBOW, XN_SKEL_LEFT_SHOULDER, 10, 10, "LBicep");
	//UNDEV

	try{
		
		bool is_first_run = true;
		while (!xnOSWasKeyboardHit() && device->run())
		{
			nUsers=MAX_NUM_USERS;
			g_Context.WaitOneUpdateAll(g_UserGenerator);
			g_UserGenerator.GetUsers(aUsers, nUsers);
		
			driver->beginScene(true, true, video::SColor(255,40,40,200));

			//Update every joint position for every user beeing tracked
			std::vector<const unsigned int>::iterator it;
			for(XnUInt16 i=0; i<nUsers; i++){
				if(g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i])==FALSE)
					continue;

				for ( it = allJoints.begin(); it != allJoints.end(); it++){
					g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i], XnSkeletonJoint(*it),Joint);

					UserSkeletons[i].jointSetPos(*it, Joint.position);

					if(is_first_run){
						printf("Pointing camera at: (%f, %f, %f)\n", Joint.position.position.X, Joint.position.position.Y, Joint.position.position.Z);
						core::vector3df head_pos = core::vector3df(f32(0),f32(0),f32(1));
						camera->setTarget(head_pos);
						is_first_run = false;
					}
				}

				//Validate exercise
				Bicep.checkRepetitions(UserSkeletons[i]);
				
				printf("%s\n", itoa(Bicep.getRepetitions(), Reps, 10));
				staticte->setText((const wchar_t *)itoa(Bicep.getRepetitions(), Reps, 10));
				

				Bicep.validate(UserSkeletons[i]);

				
			}

			//SHOW FPS:
			fps = driver->getFPS();

			core::stringw tmp(L"jKGym - Alpha [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());


			

			smgr->drawAll();
			device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)
			driver->endScene();
		}
	}

	catch(std::exception e){
		printf("Exception ocurred: %s", e.what());
		getchar();
	}

	//Clean Up
    g_scriptNode.Release();
    g_DepthGenerator.Release();
    g_UserGenerator.Release();
    g_Context.Release();

	device->drop();
}
