#ifndef TOOL_H
#define TOOL_H

#include "util/Util.h"
#include "Bay.h"

namespace PROJECT_NAMESPACE
{
     class Tool
     {
	     public:
		     map<string, Bay, LessThanString> bays; //A map from bay names to bays.
     };
}

#endif //ndef TOOL_H
