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

//Kinect
#include <XnCppWrapper.h>
#include "openni_callbacks.h"

#define SAMPLE_XML_PATH "./Data/SamplesConfig.xml"
#define SAMPLE_XML_PATH_LOCAL "SamplesConfig.xml"

//std::map<XnUserID, Skeleton> UserSkeletons;

#define MAX_NUM_USERS 15

//IrrLicht
#include "irrlicht.h"

using namespace irr;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

//Main
#include "Skeleton.h"


int main()
{
    XnStatus nRetVal = XN_STATUS_OK;
    xn::EnumerationErrors errors;

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
    XnSkeletonJointTransformation torsoJoint;

    printf("Starting to run\n");
    if(g_bNeedPose)
    {
        printf("Assume calibration pose\n");
    }
	//Let's now start IrrLicht:
		IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800,600), 32, false, false, false, NULL);
		if(!device)
			return 1;
		
		device->setWindowCaption(L"jKgym - Alpha");

		//Some pointers to IrrLicht, will come in handy soon:
		video::IVideoDriver *driver = device->getVideoDriver();
		scene::ISceneManager *smgr = device->getSceneManager();

		device->getGUIEnvironment()->addImage(driver->getTexture("C:/jk.png"),core::position2d<s32>(10,20));
		
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

	
	//Main Loop
	bool is_first_run = true;
	while (!xnOSWasKeyboardHit() && device->run())
    {
		nUsers=MAX_NUM_USERS;
        g_Context.WaitOneUpdateAll(g_UserGenerator);
        // print the torso information for the first user already tracking   
        g_UserGenerator.GetUsers(aUsers, nUsers);
		
		driver->beginScene(true, true, video::SColor(255,40,40,200));
		printf("%d", nUsers);
        for(XnUInt16 i=0; i<nUsers; i++){
            if(g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i])==FALSE)
                continue;
			
            g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_HEAD,torsoJoint);
            printf("user %d: head at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                            torsoJoint.position.position.X,
                                                            torsoJoint.position.position.Y,
                                                            torsoJoint.position.position.Z);
			torsoJoint.position;
			core::vector3df head_pos = core::vector3df(f32(torsoJoint.position.position.X),f32(torsoJoint.position.position.Y),f32(torsoJoint.position.position.Z));
			if(is_first_run){
				camera->setTarget(head_pos);
				is_first_run = false;
			}

			head_sphere->setPosition(head_pos);

			g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_RIGHT_HAND,torsoJoint);
            printf("user %d: head at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                            torsoJoint.position.position.X,
                                                            torsoJoint.position.position.Y,
                                                            torsoJoint.position.position.Z);
			torsoJoint.position;
			head_pos = core::vector3df(f32(torsoJoint.position.position.X),f32(torsoJoint.position.position.Y),f32(torsoJoint.position.position.Z));
			rhand_sphere->setPosition(head_pos);
			
			g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_LEFT_HAND,torsoJoint);
            printf("user %d: head at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                            torsoJoint.position.position.X,
                                                            torsoJoint.position.position.Y,
                                                            torsoJoint.position.position.Z);
			torsoJoint.position;
			head_pos = core::vector3df(f32(torsoJoint.position.position.X),f32(torsoJoint.position.position.Y),f32(torsoJoint.position.position.Z));
			lhand_sphere->setPosition(head_pos);
        }

		smgr->drawAll();
		device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)
		driver->endScene();
    }

	//Clean Up
    g_scriptNode.Release();
    g_DepthGenerator.Release();
    g_UserGenerator.Release();
    g_Context.Release();

	device->drop();
}
