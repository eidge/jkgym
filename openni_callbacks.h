#ifndef OPENNICALLBACKS
#define OPENNICALLBACKS

#include <XnCppWrapper.h>

extern xn::Context g_Context;
extern xn::ScriptNode g_scriptNode;
extern xn::DepthGenerator g_DepthGenerator;
extern xn::UserGenerator g_UserGenerator;

extern XnBool g_bNeedPose;
extern XnChar g_strPose[20];

void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/);
void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/);
void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& /*capability*/, const XnChar* strPose, XnUserID nId, void* /*pCookie*/);
void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& /*capability*/, XnUserID nId, void* /*pCookie*/);
void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& /*capability*/, XnUserID nId, XnCalibrationStatus eStatus, void* /*pCookie*/);

inline XnBool fileExists(const char *fn)
{
	XnBool exists;
	xnOSDoesFileExist(fn, &exists);
	return exists;
}

#define CHECK_RC(nRetVal, what)					    \
    if (nRetVal != XN_STATUS_OK)				    \
{								    \
    printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));    \
    return nRetVal;						    \
}
#endif