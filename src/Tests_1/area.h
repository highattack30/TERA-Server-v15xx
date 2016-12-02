#ifndef AREA_H
#define AREA_H

#include <string>

#include "typeDefs.h"
#include "linkList.h"
#include "mathUtils.h"


class areaFenceData
{
public:
	areaFenceData();
	~areaFenceData();

	uint32 pointsCount;
	mathVec3* points;
};

class areaSection
{

public:
	areaSection();
	~areaSection();

	bool 
		restBonus,
		maze,
		vender,
		ride,
		pk,
		pcMoveCylinder,
		ignoreObstacleShortTel,
		desTex;

	uint32
		subSectionsCount,

		campId,
		nameId,
		priority,
		floor,
		worldMapSectionId,
		id;

	float
		substractMinZ,
		addMaxZ;

	int32 
		huntingZoneId;

	char name[NAME_MAX];

	areaFenceData fenceData;
};


class area
{
public:
	area();
	~area();


	char name[NAME_MAX];

	bool ride;
	uint32
		worldMapWorldId,
		worldMapGuardId,
		nameId,
		areaId;


	nAry_tree<areaSection*> areaSections;
};



#endif