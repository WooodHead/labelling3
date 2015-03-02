#ifndef _MyPreference_h
#define _MyPreference

#include <QString>
//--------------------------------------------------------------------------------------------
// class that defines system parameters
//--------------------------------------------------------------------------------------------
class MyPreference
{
public:
	MyPreference(void);
	~MyPreference(void);
public:
	static double DefaultContourDistance; // a big number
	static double Threshold_PointDistance;
	static double Threshold_LineDistance;
	static double Threshold_ContourDistance;
	static double Threshold_CentroidDistance;
	static double Threshold_BoundingBox;
	static double Threshold_OccludingAreaRatio;
	static double Threshold_MaxTrackingVelocity;
	static double Threshold_MaxTrackingAcceleration;
	static int Threshold_MaxPointIndex;
	static QString DefaultLayerName;
	static int nPyrdLevels;
};

#endif