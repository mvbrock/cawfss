#include "refcntr.hxx"

map<string,int> RefCntr::refCountMap;

void RefCntr::Inc(string name)
{
	if( refCountMap.find( name ) != refCountMap.end() )
		refCountMap[ name ]++;
	else
		refCountMap[ name ] = 0;
}

void RefCntr::Dec(string name)
{
	if( refCountMap.find( name ) != refCountMap.end() )
		refCountMap[ name ]--;
	else
		refCountMap[ name ] = 0;
}

int RefCntr::Get(string name)
{
	if( refCountMap.find( name ) == refCountMap.end() )
		return 0;
	else
		return refCountMap[ name ];
}
