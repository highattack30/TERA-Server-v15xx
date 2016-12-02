#ifndef PASSIVITYCATEGORYTEMPLATE_H
#define PASSIVITYCATEGORYTEMPLATE_H

#include "typeDefs.h"

#include <vector>

struct passivity_template;

struct passivity_category
{
	int32 id;
	bool unchangeable;
	std::vector<const passivity_template *> passivities;

};

#endif
