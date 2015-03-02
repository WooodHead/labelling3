#ifndef _LINELET_H_
#define _LINELET_H_

#include "BasicDataStructure.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//
//		Class Linelet
//
/////////////////////////////////////////////////////////////////////////////////////////////
class Linelet
{
public:
	Linelet( void ) 
	{ 
		// doing nothing
	}
	Linelet( PointD beginPos, PointD endPos )
	{
		_beginPos = beginPos;
		_endPos = endPos;
	}

	~Linelet(){ }

	Linelet( const Linelet& linelet )
	{	
		_beginPos = linelet._beginPos;
		_endPos = linelet._endPos;
	}

	Linelet& operator=( const Linelet& linelet )
	{
		_beginPos = linelet._beginPos;
		_endPos = linelet._endPos;
		return *this;
	}

public:
	PointD _beginPos;
	PointD _endPos;
};

#endif