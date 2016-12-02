#include "worldposition.h"

#include <memory.h>
#include <math.h>
#include <float.h>

world_position * WINAPI wposition_clone(_In_ world_position * source)
{
	world_position * clone = new world_position;
	wposition_copy(source, clone);
	return clone;
}

bool WINAPI wposition_copy(_In_ world_position * src, _In_ world_position* dest)
{
	return memcpy(dest, src, sizeof(world_position)) ? true : false;
}

bool WINAPI wposition_has_collided(_In_ world_position *, _In_ world_position *)
{
	return false; //todo
}

float WINAPI wposition_distance(_In_ world_position * from, _In_ float x, _In_ float y, _In_ float z)
{
	if (!from)
		return FLT_MAX;

	float a = x - from->coords[0];
	float b = y - from->coords[1];
	float c = z - from->coords[2];

	return sqrt(a* a + b*b + c*c);
}

float WINAPI wposition_distance(_In_ world_position * from, _In_ world_position *to)
{

	if (!from || !to)
		return FLT_MAX;

	float a = to->coords[0] - from->coords[0];
	float b = to->coords[1] - from->coords[1];
	float c = to->coords[2] - from->coords[2];

	return sqrt(a* a + b*b + c*c);
}

float WINAPI wposition_fast_distance(_In_ world_position* from, _In_ float x, _In_ float y)
{
	if (!from)
		return FLT_MAX;
	float a = x - from->coords[0];
	float b = y - from->coords[1];

	return sqrt(a* a + b*b);
}

float WINAPI wposition_fast_distance(_In_ world_position * from, _In_ world_position * to)
{
	if (!from || !to)
		return FLT_MAX;
	float a = to->coords[0] - from->coords[0];
	float b = to->coords[1] - from->coords[1];

	return sqrt(a* a + b*b);
}

int16 WINAPI wposition_get_heading(_In_ world_position * from, _In_ world_position * to)
{
	return 0;
	//todo
}
