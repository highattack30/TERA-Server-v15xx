#include "servertime.h"
#include <chrono>
#include <time.h>

uint64 WINAPI timer_get_current_UTC()
{
	time_t t;
	time(&t);
	return (uint64)t;
}

uint32 timer_get_day_seconds()
{
	uint64 t = timer_get_current_UTC();
	return t % 86400; //??
}
