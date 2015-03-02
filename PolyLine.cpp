
#include "PolyLine.h"
#include "Linelet.h"

PolyLine::PolyLine( void )
{

}

PolyLine::~PolyLine()
{

}

PolyLine::PolyLine( const PolyLine& polyLine )
{
	_lineletList = polyLine._lineletList;
}

PolyLine& PolyLine::operator=( const PolyLine& polyLine )
{
	_lineletList = polyLine._lineletList;
	return *this;
}

Linelet& PolyLine::operator[]( const int index )
{
	return _lineletList[index];
}
