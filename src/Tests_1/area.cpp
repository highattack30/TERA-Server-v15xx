#include "area.h"



area::area()
{
	
}


area::~area()
{
}

areaFenceData::areaFenceData()
{
	points = nullptr;
	pointsCount = 0;
}

areaFenceData::~areaFenceData()
{
	delete[] points;
}

areaSection::areaSection()
{
	restBonus = maze = ride = vender = pk = pcMoveCylinder = ignoreObstacleShortTel = desTex = true;
	campId = nameId = priority = floor = worldMapSectionId = id = 0;
	huntingZoneId = 0;
	substractMinZ = addMaxZ = 0.0f;
}

areaSection::~areaSection()
{

}
