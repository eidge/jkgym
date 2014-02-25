#include "jknect.h"

SimpleKinect::SimpleKinect(const std::string xmldir): nRetVal(XN_STATUS_OK), fn(NULL)
			{
				//Import Configuration file:
				if    (fileExists(xmldir.c_str()))
					fn = xmldir.c_str();
				else 
					throw std::exception("No config file found.");
				
				nRetVal = g_Context.InitFromXmlFile(fn, g_scriptNode, &errors);
				if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
				{
					XnChar strError[1024];
					errors.ToString(strError, 1024);
					
					throw std::exception(strError);
				}
				else if (nRetVal != XN_STATUS_OK)
				{
					throw std::exception(xnGetStatusString(nRetVal));
				}
			}

SimpleKSkeleton::SimpleKSkeleton(const std::string xmldir): SimpleKinect(xmldir)
{
	g_strPose[0] = XnChar("");
	g_bNeedPose  = false;
	//Activate Depth and skeleton tracking
    nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
    //checkrc("No depth");

    nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_USER, g_UserGenerator);
    if (nRetVal != XN_STATUS_OK)
    {
        nRetVal = g_UserGenerator.Create(g_Context);
        //checkrc("Find user generator");
    }

    XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected;
    if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON)){
        printf("Supplied user generator doesn't support skeleton\n");
        //FIXMEreturn 1;
    }
    nRetVal = g_UserGenerator.RegisterUserCallbacks(&SimpleKSkeleton::User_NewUser, &SimpleKSkeleton::User_LostUser, NULL, hUserCallbacks);
    //CHECK_RC(nRetVal, "Register to user callbacks");
    nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationStart(UserCalibration_CalibrationStart, NULL, hCalibrationStart);
    //CHECK_RC(nRetVal, "Register to calibration start");
    nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete(UserCalibration_CalibrationComplete, NULL, hCalibrationComplete);
    //CHECK_RC(nRetVal, "Register to calibration complete");

    if (g_UserGenerator.GetSkeletonCap().NeedPoseForCalibration())
    {
        g_bNeedPose = TRUE;
        if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
        {
            printf("Pose required, but not supported\n");
            //FIXMEreturn 1;
        }
        nRetVal = g_UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected(UserPose_PoseDetected, NULL, hPoseDetected);
        //CHECK_RC(nRetVal, "Register to Pose Detected");
        g_UserGenerator.GetSkeletonCap().GetCalibrationPose(g_strPose);
    }

    g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

    nRetVal = g_Context.StartGeneratingAll();
    //CHECK_RC(nRetVal, "StartGenerating");
}
