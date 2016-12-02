#ifndef JOB_H
#define JOB_H

#include "typeDefs.h"
#include "jobEnums.h"
#include "socket.h"

typedef struct job
{
	WSAOVERLAPPED ov;

	e_job_type type;
	uint32 argc;
	void** argv;
} job;


#endif // ifndef JOB_H
