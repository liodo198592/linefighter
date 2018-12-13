#include "MapGround.h"


CMapGround::CMapGround(void):CAbstractMap("map/background.png")
{
}


CMapGround::~CMapGround(void)
{
}

void CMapGround::initCoordinate()
{

	m_mapPosCoordinate[1][LEFT_UP].setPoint(75,342);
 	m_mapPosCoordinate[1][RIGHT_UP].setPoint(220,342);
 	m_mapPosCoordinate[1][CENTER].setPoint(147,340);
 	m_mapPosCoordinate[1][LEFT_DOWN].setPoint(100,325);
 	m_mapPosCoordinate[1][RIGHT_DOWN].setPoint(205,325);
// 
	m_mapPosCoordinate[2][LEFT_UP].setPoint(320,342);
	m_mapPosCoordinate[2][RIGHT_UP].setPoint(465,342);
	m_mapPosCoordinate[2][CENTER].setPoint(392,340);
	m_mapPosCoordinate[2][LEFT_DOWN].setPoint(345,325);
	m_mapPosCoordinate[2][RIGHT_DOWN].setPoint(450,325);
// 
	m_mapPosCoordinate[3][LEFT_UP].setPoint(570,342);
	m_mapPosCoordinate[3][RIGHT_UP].setPoint(715,342);
	m_mapPosCoordinate[3][CENTER].setPoint(642,340);
	m_mapPosCoordinate[3][LEFT_DOWN].setPoint(595,325);
	m_mapPosCoordinate[3][RIGHT_DOWN].setPoint(700,325);
// 
	m_mapPosCoordinate[4][LEFT_UP].setPoint(75,200);
	m_mapPosCoordinate[4][RIGHT_UP].setPoint(220,200);
	m_mapPosCoordinate[4][CENTER].setPoint(147,198);
	m_mapPosCoordinate[4][LEFT_DOWN].setPoint(100,183);
	m_mapPosCoordinate[4][RIGHT_DOWN].setPoint(205,183);
// 
	m_mapPosCoordinate[5][LEFT_UP].setPoint(320,200);
	m_mapPosCoordinate[5][RIGHT_UP].setPoint(465,200);
	m_mapPosCoordinate[5][CENTER].setPoint(392,198);
	m_mapPosCoordinate[5][LEFT_DOWN].setPoint(345,183);
	m_mapPosCoordinate[5][RIGHT_DOWN].setPoint(450,183);
// 
	m_mapPosCoordinate[6][LEFT_UP].setPoint(570,200);
	m_mapPosCoordinate[6][RIGHT_UP].setPoint(715,200);
	m_mapPosCoordinate[6][CENTER].setPoint(642,198);
	m_mapPosCoordinate[6][LEFT_DOWN].setPoint(595,183);
	m_mapPosCoordinate[6][RIGHT_DOWN].setPoint(700,183);


	m_mapPlayerStatShowPos[1].setPoint(75,415);
	m_mapPlayerStatShowPos[2].setPoint(270,415);
	m_mapPlayerStatShowPos[3].setPoint(470,415);
	m_mapPlayerStatShowPos[4].setPoint(670,415);
}
