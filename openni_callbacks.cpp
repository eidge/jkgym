#include "openni_callbacks.h"
#include <XnCppWrapper.h>
#include "Skeleton.h"
#include <map>
#include <vector>

xn::Context g_Context;
xn::ScriptNode g_scriptNode;
xn::DepthGenerator g_DepthGenerator;
xn::UserGenerator g_UserGenerator;

XnBool g_bNeedPose = FALSE;
XnChar g_strPose[20] = "";

int x[24] = {XN_SKEL_HEAD, XN_SKEL_LEFT_ANKLE, XN_SKEL_LEFT_COLLAR, XN_SKEL_LEFT_ELBOW, XN_SKEL_LEFT_FINGERTIP, XN_SKEL_LEFT_FOOT, XN_SKEL_LEFT_HAND,
		   XN_SKEL_LEFT_HIP, XN_SKEL_LEFT_KNEE, XN_SKEL_LEFT_SHOULDER, XN_SKEL_LEFT_WRIST, XN_SKEL_NECK, XN_SKEL_TORSO, XN_SKEL_WAIST,
		   XN_SKEL_RIGHT_ANKLE, XN_SKEL_RIGHT_COLLAR, XN_SKEL_RIGHT_ELBOW, XN_SKEL_RIGHT_FINGERTIP, XN_SKEL_RIGHT_FOOT, XN_SKEL_RIGHT_HAND,
		   XN_SKEL_RIGHT_HIP, XN_SKEL_RIGHT_KNEE, XN_SKEL_RIGHT_SHOULDER, XN_SKEL_RIGHT_WRIST};

std::vector<const unsigned int> allJoints (x, x+24);




void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d New User %d\n", epochTime, nId);
    // New user found
    if (g_bNeedPose){
        g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
	}
    else{
        g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
	}
}


void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
	
	UserSkeletons[nId-1].jointAllCleanUp();
		
	//UserSkeletons.erase(nId-1);
    printf("%d Lost user %d\n", epochTime, nId);	
}


void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& /*capability*/, const XnChar* strPose, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Pose %s detected for user %d\n", epochTime, strPose, nId);
    g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
    g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}


void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& /*capability*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Calibration started for user %d\n", epochTime, nId);
}

void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& /*capability*/, XnUserID nId, XnCalibrationStatus eStatus, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    if (eStatus == XN_CALIBRATION_STATUS_OK){
        // Calibration succeeded
        printf("%d Calibration complete, start tracking user %d\n", epochTime, nId);		
        g_UserGenerator.GetSkeletonCap().StartTracking(nId);

		//Add new skeleton
		UserSkeletons[nId-1] = Skeleton(allJoints);
    }
    else{
        // Calibration failed
        printf("%d Calibration failed for user %d\n", epochTime, nId);
        if(eStatus==XN_CALIBRATION_STATUS_MANUAL_ABORT){
            printf("Manual abort occured, stop attempting to calibrate!");
            return;
        }
        if (g_bNeedPose){
            g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
        }
        else{
            g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
        }
    }
}