#include "CMap.h"



CMap::CMap()
{
}


CMap::~CMap()
{
}

void CMap::setMapType(COORD pos, int nType)
{
	m_nMap[pos.X][pos.Y] = nType;
}

int CMap::getMapType(COORD pos)
{
	return m_nMap[pos.X][pos.Y];
}

void CMap::LoadMap(const char * szFileName)
{
	FILE *pFile = nullptr;
	fopen_s(&pFile, szFileName, "r");
	if (pFile == nullptr)
	{
		exit(0);
	}
	fread(m_nMap, sizeof(m_nMap), 1, pFile);
	fclose(pFile);
}

void CMap::SaveMap(const char * szFileName)
{
	FILE *pFile = nullptr;
	fopen_s(&pFile, szFileName, "wb+");
	if (pFile == nullptr)
	{
		exit(0);
	}
	fwrite(m_nMap, sizeof(m_nMap), 1, pFile);
	fclose(pFile);
}

void CMap::ClearShowMap()
{
	for (short i = 0; i < 40; i++)
		for (short j = 0; j < 40; j++)
		{
			if (i == 0 || j == 0 || i == 39 || j == 39)
				DrawImage({ i,j }, WALL_2);
			else
				DrawImage({ i,j }, WALL_NO);
		}
}

void CMap::DrawMap()
{
	for (short i = 0; i < 40; i++)
		for (short j = 0; j < 40; j++)
		{
			DrawImage({ i,j }, m_nMap[i][j]);
		}
}

void CMap::DrawImage(COORD pos, int nType)
{
	switch (nType)
	{
	case WALL_NO:				//空地
		WriteChar(pos.X, pos.Y, "  ");
		break;
	case WALL_1:				//砖墙
		WriteChar(pos.X, pos.Y, "■", 0x0C);
		break;
	case WALL_2:				//铁墙
		WriteChar(pos.X, pos.Y, "■", 0x0F);
		break;
	case LAWN:					//草地
		WriteChar(pos.X, pos.Y, "※", 0x0A);
		break;
	case RIVER:					//河流
		WriteChar(pos.X, pos.Y, "≈", 0x03);
		break;
	}
}

void CMap::InitMap()
{
	for(int i = 0;i<40;i++)
		for (int j = 0; j < 40; j++)
		{
			if (i == 0 || j == 0 || i == 39 || j == 39)
				m_nMap[i][j] = WALL_2;
			else
				m_nMap[i][j] = WALL_NO;
		}
}

void CMap::DrawReverAndLawn()
{
	for(short i = 0;i<40;i++)
		for (short j = 0; j < 40; j++)
		{
			if (m_nMap[i][j] == LAWN)
				DrawImage({ i,j }, LAWN);
			if (m_nMap[i][j] == RIVER)
				DrawImage({ i,j }, RIVER);
		}
}

void CMap::RemoveRange(COORD pos)
{
	for (short nX = pos.X - 1; nX < pos.X + 2; nX++)
		for (short nY = pos.Y - 1; nY < pos.Y + 2; nY++)
		{
			if (nX >= 0 && nX < 40 && nY >= 0 && nY < 40)	//当在地图范围内且不是铁墙时，爆破
				m_nMap[nX][nY] = (m_nMap[nX][nY]==WALL_2? WALL_2:WALL_NO);
		}
}

void CMap::DrawRange(COORD pos)
{
	for (short nX = pos.X - 1; nX < pos.X + 1; nX++)
		for (short nY = pos.Y - 1; nY < pos.Y + 1; nY++)
		{
			if (nX >= 0 && nX < 40 && nY >= 0 && nY < 40)	//当在地图范围内时，画
				DrawImage({ nX,nY }, m_nMap[nX][nY]);
		}
}

