#ifndef EXERCISE_H
#define EXERCISE_H

#include "Skeleton.h"

#include <vector>
#include <string>
#include <algorithm>

class Relation{
 
private:
        bool vectical_check;
        int joint1, joint2, joint3;
        double angle, threshold;
        bool validate( Skeleton Sk);
        std::string _id;
        
       
public:
	Relation() {}
	Relation(const unsigned int j1, const unsigned int j2, double ang, double thres, std::string id):
		joint1(j1), joint2(j2), angle(ang), threshold(thres), vectical_check(true), _id(id) {}
	Relation (const unsigned int j1, const unsigned int j2, const unsigned int j3, double ang, double thres, std::string id):
		joint1(j1), joint2(j2), joint3(j3), vectical_check(false), angle(ang), threshold(thres), _id(id) {}

	
	std::string getId() const {return _id;};

    bool operator==(const Relation &rhs)
        {
                return _id == rhs.getId();
        }
 
    bool operator==(const std::string tgId)
        {
                return _id == tgId;
        }

	int RepStatus(Skeleton Sk);
 
    friend class Exercise;
};

class Exercise{
	public:
		Exercise(): up(false), repetitions(0) {}
		Exercise(Relation RR): up(false), repetitions(0), RepRel(RR) {}


		void addRelation(const unsigned int joint1, const unsigned int joint2, const unsigned int joint3, double angle, double thres, std::string id)
			{Relations.push_back(Relation(joint1, joint2, joint3, angle, thres, id));}

		void addRelation(const unsigned int joint1, const unsigned int joint2, double angle, double thres, std::string id)
			{Relations.push_back(Relation(joint1, joint2, angle, thres, id));}

		void removeRelation(std::string id) {  Relations.erase(std::find(Relations.begin(), Relations.end(), id)); }

		bool validate(Skeleton Sk){ bool r_value = true;
									for(std::vector<Relation>::iterator it = Relations.begin(); it != Relations.end(); ++it){
										if(it->validate(Sk) == false)
											r_value = false;
									}

									return r_value;}
		
		void checkRepetitions(Skeleton Sk);
		unsigned int getRepetitions() {return repetitions;}

	private:
		std::vector<Relation> Relations;
		Relation RepRel;

		unsigned int repetitions;
		bool up;
};

#endif