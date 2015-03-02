#ifndef __FILEOPERATE_H__
#define __FILEOPERATE_H__

#include <list>

class NodeData;
class LineletData;
class CurveData;
class RegionDO;
class PhysicalObjectDO;


/********************************************************************/
/*							FileOperate Class					    */
/********************************************************************/

class FileOperate
{
public:
	static BOOL OpenDir(char *szPath);
	static BOOL LoadAllFiles(CString path);
	static BOOL SaveLabelmap( CString strSavePath );
	static BOOL NodeFileLoad(CString NodeLoadFilename, std::list<NodeData> &NodeDatalist);
	static BOOL LineletFileLoad(CString LineletLoadFilename, std::list<LineletData> &Lineletlist, bool isModel = true);
	static BOOL CurveFileLoad(CString curveLoadFilename, std::list<CurveData> &curveList, bool isModel = true);
	static BOOL ContourFileLoad(CString contourLoadFilename, std::list<ContourDO> &contourList);
	static BOOL RegionFileLoad(CString regionLoadFilename, std::list<RegionDO> &regionList);
	static BOOL PhysicalObjectLoad(CString physicalObjectLoadFilename, std::list<PhysicalObjectDO> &physicalobjectList);

	static BOOL PreProcess();

	static void	Clear();
};



#endif