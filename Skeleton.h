#ifndef SKELETON
#define SKELETON

#include <irrlicht.h>
#include <XnCppWrapper.h>

#include <string>

#include <vector>
#include <algorithm>

#include <map>



class SkeletonJoint{
	public:
		SkeletonJoint(unsigned int joint, irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver, XnSkeletonJointPosition pos3d = XnSkeletonJointPosition(), irr::f32 sphere_radius = 50): smgr(smgr), driver(driver), number(joint), pos(pos3d) 
			{
				sphere = smgr->addSphereSceneNode(sphere_radius);
				sphere->setPosition(xnPos2irrPos(pos3d));
				sphere->setMaterialTexture(0, driver->getTexture("../../media/wall.bmp"));
                sphere->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			}

		virtual ~SkeletonJoint() {}

		XnSkeletonJointPosition getJointPosition() const {return pos;}
		unsigned int getJointNumber() const {return number;}
		
		SkeletonJoint& operator=(XnSkeletonJointPosition posi) {this->pos = posi; (this->sphere)->setPosition(xnPos2irrPos(this->pos));return *this;}
		void setTexture(int status) {
			switch(status){
			case 0:
				sphere->setMaterialTexture(0, driver->getTexture(Texture.c_str()));
				break;
			case 1:
				sphere->setMaterialTexture(0, driver->getTexture(okTexture.c_str()));
				break;
			case -1:
				sphere->setMaterialTexture(0, driver->getTexture(badTexture.c_str()));
				break;

			default:
				throw std::exception("Bad joint status.");
			}
		}

		bool operator==(const SkeletonJoint &rhs) const {return isEqual(*this, rhs);}
		bool operator<(const SkeletonJoint &rhs) const {return lessThan(*this, rhs);}
		bool operator>(const SkeletonJoint &rhs) const {return !lessThan(*this, rhs);}

		bool operator==(const int &rhs) const {return isEqual(*this, rhs);}
		bool operator<(const int &rhs) const {return lessThan(*this, rhs);}
		bool operator>(const int &rhs) const {return !lessThan(*this, rhs);}

		void cleanUp() {smgr->addToDeletionQueue(sphere);}
		
	protected:
		bool isEqual(const SkeletonJoint &lhs, const SkeletonJoint &rhs) const {return lhs.number == rhs.number;}
		bool lessThan(const SkeletonJoint &lhs, const SkeletonJoint &rhs) const {return lhs.number < rhs.number;}

		bool isEqual(const SkeletonJoint &lhs, const int &rhs) const {return lhs.number == rhs;}
		bool lessThan(const SkeletonJoint &lhs, const int &rhs) const {return lhs.number < rhs;}

		irr::core::vector3df xnPos2irrPos(const XnSkeletonJointPosition &pos) const {return irr::core::vector3df(pos.position.X,pos.position.Y,pos.position.Z);}
		
		irr::scene::ISceneNode *sphere;
		irr::scene::ISceneManager *smgr;
		irr::video::IVideoDriver *driver;

	private:
		unsigned int number;
		XnSkeletonJointPosition pos;

		static std::string Texture;
		static std::string badTexture;
		static std::string okTexture;
};

class Skeleton{
	public:
		Skeleton() {}
		//Initialize skeleton with all joints in jointVector;
		Skeleton(const std::vector<const unsigned int> &jointVector) { std::vector<const unsigned int>::const_iterator ini = jointVector.begin(); do{jointAdd(*ini);}while(++ini != jointVector.end()); }
		virtual ~Skeleton() {}

		static irr::scene::ISceneManager *smgr;
		static irr::video::IVideoDriver *driver;
		
		void jointAdd(const unsigned int &joint, XnSkeletonJointPosition joint_position = XnSkeletonJointPosition(), irr::f32 sphere_radius = 50.0);
		SkeletonJoint jointGet(const unsigned int &joint);
		void jointSetPos(const unsigned int &joint, const XnSkeletonJointPosition &pos){if(jointExists(joint)){ *(std::find(this->skeleton.begin(), this->skeleton.end(), joint)) = pos;}}
		const std::vector<SkeletonJoint> jointAll() const {return skeleton;}
		bool jointExists(const unsigned int &joint) const {return std::find(this->skeleton.begin(), this->skeleton.end(), joint) != skeleton.end();}
		void jointAllCleanUp() {for(std::vector<SkeletonJoint>::iterator it = skeleton.begin(); it != skeleton.end(); ++it) it->cleanUp();}

		double getJointAngle(unsigned int joint1, unsigned int joint2, unsigned int joint3);
		double getVerticalAngle(unsigned int joint1, unsigned int joint2);

	protected:
		std::vector<SkeletonJoint> skeleton;
};


extern std::map<XnUserID, Skeleton> UserSkeletons;
extern std::vector<const unsigned int> allJoints;
#endif