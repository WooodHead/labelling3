#ifndef _POLYLINE_H_
#define _POLYLINE_H_

#include <vector>
#include <cmath>
#include "BasicDataStructure.h"
#include "Linelet.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//
//		Class PolyLine
//
/////////////////////////////////////////////////////////////////////////////////////////////
class PolyLine 
{
public:
	PolyLine( void );
	~PolyLine();
	PolyLine( const PolyLine& polyLine );
	PolyLine& operator=( const PolyLine& polyLine );
	Linelet& operator[]( const int index );

	inline void addLinelet( const Linelet& line )
	{
		_lineletList.push_back( line );
	}

	inline void updateLastLinelet( const Linelet& line )
	{
		*(_lineletList.end() -1 ) = line;
	}

	inline void updateLinelet( const Linelet& line, int index )
	{
		if ( index < (int)_lineletList.size() )
		{
			_lineletList[index] = line;
		}
	}

	inline int size()
	{
		return _lineletList.size();
	}

	inline void clear()
	{
		_lineletList.clear();
	}

	inline void setPointList()
	{
		int num = size();
		for ( int i = 0; i < num; ++i )
		{
			_pointList.push_back( _lineletList[i]._beginPos );
		}
		_pointList.push_back( _lineletList[num-1]._endPos );
	}


private:
	std::vector<Linelet>		_lineletList;
	std::vector<PointD>		_pointList;	
};


#endif