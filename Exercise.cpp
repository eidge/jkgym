#include "Exercise.h"
#include "Skeleton.h"
 
int Relation::RepStatus(Skeleton Sk)
{
    if( threshold > 0)
		{
			if (Sk.getJointAngle(joint1, joint2, joint3) > angle) return 1;
			if (Sk.getJointAngle(joint1, joint2, joint3) < angle - threshold) return -1;
		}
	else
		{
			if (Sk.getJointAngle(joint1, joint2, joint3) < angle) return 1;
			if (Sk.getJointAngle(joint1, joint2, joint3) > angle - threshold) return -1;
		}
	return 0;
}
 
void Exercise::checkRepetitions(Skeleton Sk)
{
        int curStatus = RepRel.RepStatus(Sk);
 
        if( curStatus == 1 && up == false){
                repetitions++;
                up = true;
        }
        else if( curStatus == -1 && up == true)
        {
                up = false;
        }
}

bool Relation::validate( Skeleton Sk)
        {
			
			if(vectical_check)
                {
                        if(abs (Sk.getVerticalAngle(joint1, joint2) - angle) > threshold){
								Sk.jointGet(joint1).setTexture(-1);
                                return false;
						}
                }
 
            else
                {
                        if(abs (Sk.getJointAngle(joint1, joint2, joint3) - angle) > threshold){
								Sk.jointGet(joint1).setTexture(-1);
                                return false;
						}
                }
			
		   Sk.jointGet(joint1).setTexture(1);
           return true;
}