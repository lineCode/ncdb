#include "stdafx.h"
#include "Math/mathUnit.h"

Pi_NameSpace_Begin
bool FlowBigger( double n1, double n2 )
{
	return abs(n1 - n2) > std::numeric_limits<float>::epsilon();
}

bool FlowEqual( double n1, double n2 )
{
	return abs(n1 - n2) < std::numeric_limits<float>::epsilon();
}

Pi_NameSpace_End