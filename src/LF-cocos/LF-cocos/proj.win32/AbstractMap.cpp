#include "AbstractMap.h"


CAbstractMap::CAbstractMap(string MapFilePath):m_MapFilePath(MapFilePath)
{
}


CAbstractMap::~CAbstractMap(void)
{
}

string CAbstractMap::getMapFilePath()
{
	return m_MapFilePath;
}

CCPoint CAbstractMap::getPosCoordinate(int posIndex,PerMapPos posDirect)
{
	return m_mapPosCoordinate[posIndex][posDirect];
}
//获取人物状态区域坐标，参数为第几个
CCPoint CAbstractMap::getPlayerStatCoordinate(int iIndex)
{
	return m_mapPlayerStatShowPos[iIndex];
}