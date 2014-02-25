#include "Skeleton.h"

#include <irrlicht.h>
#include <XnCppWrapper.h>

#include <vector>
#include <algorithm>
#include <exception>

using namespace std;

irr::video::IVideoDriver* Skeleton::driver = 0;
irr::scene::ISceneManager* Skeleton::smgr  = 0;

std::string SkeletonJoint::Texture = "../../media/wall.bmp";
std::string SkeletonJoint::badTexture = "../../media/wall_red.bmp";
std::string SkeletonJoint::okTexture = "../../media/wall_green.bmp";

void Skeleton::jointAdd(const unsigned int &joint, XnSkeletonJointPosition joint_position, irr::f32 sphere_radius)
{
	if (!jointExists(joint))
		skeleton.push_back(SkeletonJoint(joint, this->smgr, this->driver, joint_position, sphere_radius));
	else
		throw std::exception("Joint already exists.");	
}

SkeletonJoint Skeleton::jointGet(const unsigned int &joint)
{
	vector<SkeletonJoint>::const_iterator joint_iter = find(this->skeleton.begin(), this->skeleton.end(), joint);

	if (joint_iter == skeleton.end())
		throw exception("Joint does not exist in this skeleton.");
	else
		return *joint_iter;
}

double Skeleton::getJointAngle(unsigned int joint1, unsigned int joint2, unsigned int joint3)
{
       
        XnVector3D j1 = jointGet(joint1).getJointPosition().position;
        XnVector3D j2 = jointGet(joint2).getJointPosition().position;
        XnVector3D j3 = jointGet(joint3).getJointPosition().position;
 
        double v1x = j1.X - j2.X;
        double v1y = j1.Y - j2.Y;
        double v1z = j1.Z - j2.Z;
 
        double v2x = j3.X - j2.X;
        double v2y = j3.Y - j2.Y;
        double v2z = j3.Z - j2.Z;
 
        // Produto Escalar e normas
 
        double dot = v1x*v2x + v1y*v2y + v1z*v2z;
        double n1 = sqrt(v1x*v1x + v1y*v1y + v1z*v1z);
        double n2 = sqrt(v2x*v2x + v2y*v2y + v2z*v2z);
 
        double PI = 3.1415926f;
        double angle = acos(dot / (n1 * n2));
        double angle2 = angle * 180 / PI;
 
        return angle2;
};

double Skeleton::getVerticalAngle(unsigned int joint1, unsigned int joint2){
        XnVector3D j1 = jointGet(joint1).getJointPosition().position;
        XnVector3D j2 = jointGet(joint2).getJointPosition().position;
        XnVector3D j3;
        j3.X = j2.X;
        j3.Y = j2.Y-500;
        j3.Z = j2.Z;
 
        double v1x = j1.X - j2.X;
        double v1y = j1.Y - j2.Y;
        double v1z = j1.Z - j2.Z;
 
        double v2x = j3.X - j2.X;
        double v2y = j3.Y - j2.Y;
        double v2z = j3.Z - j2.Z;
 
        // Produto Escalar e normas
 
        double dot = v1x*v2x + v1y*v2y + v1z*v2z;
        double n1 = sqrt(v1x*v1x + v1y*v1y + v1z*v1z);
        double n2 = sqrt(v2x*v2x + v2y*v2y + v2z*v2z);
 
        double PI = 3.1415926f;
        double angle = acos(dot / (n1 * n2));
        double angle2 = angle * 180 / PI;
 
        return angle2;
};

