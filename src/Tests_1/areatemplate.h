#ifndef AREATEMPLATE_H
#define AREATEMPLATE_H

#include <vector>

#include "typeDefs.h"

struct area_fence
{
	float x, y, z;
};

typedef struct area_section_template
{
	std::string name;

	int32
		areaValue,
		continentId,
		worldMapSectionId,
		huntingZoneId,
		priority,
		nameId,
		campId,
		floor;

	float
		substractMinZ,
		addMaxZ,
		subtractMinZ;

	bool
		restExpBonus,
		ride,
		vender,
		pk,
		ignoreObstacleShortTel;


	std::vector<area_fence> fences;
	std::vector<area_section_template> subSections;
}area_section, *lparea_section;

typedef struct area_template
{
	std::string name;

	byte
		ride;

	int
		continentId,
		worldMapWorldId,
		worldMapGuardId,
		startWorldMapSectionId;


	std::vector<area_section_template> _sections;

}area_template , *lparea_template;

#endif
