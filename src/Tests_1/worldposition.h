#ifndef WORLDPOSITION_H
#define WORLDPOSITION_H

#include "typeDefs.h"
#include <sal.h>

typedef struct world_position
{
	float coords[3];
	float radius;
	int16 heading;

	uint32
		worldMapWorldId,
		worldMapSectionId,
		worldMapGuardId,
		areaId;


} world_position;

world_position * WINAPI wposition_clone(_In_ world_position * src);
bool WINAPI wposition_copy(_In_ world_position *src, _In_ world_position* dest);
bool WINAPI wposition_has_collided(_In_ world_position * a, _In_ world_position *b);
float WINAPI wposition_distance(_In_ world_position * from, _In_ world_position *to);
float WINAPI wposition_distance(_In_ world_position *from, _In_ float x, _In_ float y, _In_ float z);
float WINAPI wposition_fast_distance(_In_ world_position *from, _In_ float z, _In_ float y);
float WINAPI wposition_fast_distance(_In_ world_position *from, _In_ world_position *to);
int16 WINAPI wposition_get_heading(_In_ world_position *from, _In_ world_position *to);

#endif
