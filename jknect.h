/****************************************************************************
*                                                                           *
*  Hugo Ribeira	                                                            *
*  Copyright (C) 2012                                                       *
*                                                                           *
*  This file is part of SimpleKinect.                                       *
*                                                                           *
*  SimpleKinect is free software: you can redistribute it and/or modify     *
*  it under the terms of the GNU Lesser General Public License as published *
*  by the Free Software Foundation, either version 3 of the License, or     *
*  (at your option) any later version.                                      *
*                                                                           *
*  SimpleKinect is distributed in the hope that it will be useful,          *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
*  GNU Lesser General Public License for more details.                      *
*                                                                           *
*  You should have received a copy of the GNU Lesser General Public License *
*  along with OpenNI. If not, see <http://www.gnu.org/licenses/>.           *
*                                                                           *
****************************************************************************/

#ifndef JKNECT
#define JKNECT


#include <XnCppWrapper.h>

#include <exception>
#include <string>


#define SAMPLE_XML_PATH "./Data/SamplesConfig.xml"
#define SAMPLE_XML_PATH_LOCAL "SamplesConfig.xml"

#define MAX_NUM_USERS 15

class SimpleKinect{
	protected:
		//Variable Declaration
		xn::Context g_Context;
		xn::ScriptNode g_scriptNode;

		XnStatus nRetVal;
		xn::EnumerationErrors errors;

		const char *fn;

		void checkrc(std::string what)
			{
				if (nRetVal != XN_STATUS_OK)
					throw std::exception(std::string(what + xnGetStatusString(nRetVal)).c_str());
			}

	public:
		SimpleKinect(const std::string xmldir = SAMPLE_XML_PATH);
		virtual ~SimpleKinect()
			{
				g_scriptNode.Release();
				g_Context.Release();
			}

	private:
		XnBool fileExists(const char *fn)
			{
				XnBool exists;
				xnOSDoesFileExist(fn, &exists);
				return exists;
			}



		//No Copy, Assignment...
		SimpleKinect(const SimpleKinect &src);
		SimpleKinect& operator=(const SimpleKinect &src);
};

class SimpleKSkeleton : public SimpleKinect{
	protected:
		static xn::DepthGenerator g_DepthGenerator;
		static xn::UserGenerator g_UserGenerator;
		
		static XnBool g_bNeedPose;
		static XnChar g_strPose[20];

	public:
		SimpleKSkeleton(const std::string xmldir = SAMPLE_XML_PATH);

		//DEV
		static void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/){
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
		static void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Lost user %d\n", epochTime, nId);	
}
		static void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& /*capability*/, const XnChar* strPose, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Pose %s detected for user %d\n", epochTime, strPose, nId);
    g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
    g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}
		static void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& /*capability*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Calibration started for user %d\n", epochTime, nId);
}
		static void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& /*capability*/, XnUserID nId, XnCalibrationStatus eStatus, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    if (eStatus == XN_CALIBRATION_STATUS_OK){
        // Calibration succeeded
        printf("%d Calibration complete, start tracking user %d\n", epochTime, nId);		
        g_UserGenerator.GetSkeletonCap().StartTracking(nId);
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
		//UNDEV

};

#endif