#include "logService.h"

#include <varargs.h>

void LOG(STRING format, ...)
{
	std::lock_guard<std::mutex> lock(_consoleMutex);

	va_list args;
	va_start(args);

	vprintf(format, args);

	va_end(args);

	return;
}