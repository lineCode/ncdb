#pragma once
#include "boostFile.h"
using namespace boost::property_tree;

namespace boost
{

#ifdef _UNICODE
	typedef wptree tptree;
	//typedef wstringstream tstringstream;
	typedef wformat tformat;
	
#else
	typedef ptree tptree;
	//typedef stringstream tstringstream;
	typedef format tformat;

#endif

}
