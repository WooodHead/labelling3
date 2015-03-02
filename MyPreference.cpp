#include <QtGui>
#include "MyPreference.h"

double MyPreference::DefaultContourDistance=100000;
double MyPreference::Threshold_ContourDistance=5;
double MyPreference::Threshold_PointDistance=7;
double MyPreference::Threshold_LineDistance=4;
double MyPreference::Threshold_CentroidDistance=5;
double MyPreference::Threshold_BoundingBox=7;
double MyPreference::Threshold_OccludingAreaRatio=0.07;
double MyPreference::Threshold_MaxTrackingVelocity=40;
double MyPreference::Threshold_MaxTrackingAcceleration=20;
int MyPreference::Threshold_MaxPointIndex = 65536;
QString MyPreference::DefaultLayerName="Unknown";
int MyPreference::nPyrdLevels=5;

MyPreference::MyPreference(void)
{
}

MyPreference::~MyPreference(void)
{
}
