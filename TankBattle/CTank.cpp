#include "CTank.h"

extern const char szTankSharp[2][4][3][7];
CMap CTank::s_objTankMap;
CTank CTank::s_arrTank[6];

CTank::CTank()
{
}


CTank::~CTank()
{
}

void CTank::TransformMap(CMap objGameMap)
{
	s_objTankMap.InitMap();
	//遍历游戏地图，如果遇到河流，2种墙，则在坦克地图上以该位置为中点，画一圈障碍物
	for (short i = 0; i < 40; i++)
	{
		for (short j = 0; j < 40; j++)
		{
			int nType = objGameMap.getMapType({ i,j });
			if (nType == RIVER || nType == WALL_1 || nType == WALL_2)
				IncreaseRange({ i,j });
		}
	}
}

void CTank::IncreaseRange(COORD pos)
{
	for (short nX = pos.X - 1; nX <= pos.X + 1; nX++)
		for (short nY = pos.Y - 1; nY <= pos.Y + 1; nY++)
		{
			if (nX >= 0 && nX < 40 && nY >= 0 && nY < 40)	//当在地图范围内时，增加
				s_objTankMap.setMapType({ nX,nY }, 1);
		}
}

void CTank::LoadTank()
{
	FILE *pFile = nullptr;
	fopen_s(&pFile, ".//resource//data//TankData.bin", "r");
	if (pFile == nullptr)
	{
		exit(0);
	}
	fread(CTank::s_arrTank, sizeof(CTank), 6, pFile);
	fclose(pFile);
}

void CTank::SaveTank()
{
	FILE *pFile = nullptr;
	fopen_s(&pFile, ".//resource//data//TankData.bin", "wb+");
	if (pFile == nullptr)
	{
		exit(0);
	}
	fwrite(CTank::s_arrTank, sizeof(CTank), 6, pFile);
	fclose(pFile);
}

void CTank::DrawTank()
{
	//将坦克在地图中和屏幕上画出,根据坦克类型画出不同颜色的坦克,红坦克威力大，蓝坦克威力小速度快
	//玩家坦克为黄色
	int nColor = 0x0E;
	if (m_nPower == 4)
		nColor = 0x0C;
	else if(m_nPower == 1)
		nColor = 0x01;
	for (short nY = m_pos.Y - 1, i = 0; nY < m_pos.Y + 2; nY++, i++)
	{
		WriteChar(m_pos.X - 1, nY, szTankSharp[m_nType % 10][m_nDir][i], nColor);
	}
}

void CTank::ClearTank()
{
	//根据中心点，推断出起点，直接填充9个格子
	//将坦克从屏幕上删除
	for (short nY = m_pos.Y - 1; nY < m_pos.Y + 2; nY++)
	{
		WriteChar(m_pos.X - 1, nY, "      ");
	}
}

void CTank::MoveTank(int nDir)
{
	//移动
	//只要调用移动，必然重画坦克
	//从坦克地图中清除坦克
	ClearTank();
	//1.计算出移动后的中心点
	COORD TempPos = m_pos;
	if (nDir + m_nDir == 3)
	{
		//反向，则坐标不变,只改变方向，不需要碰撞检测
		m_nDir = nDir;
		DrawTank();
		return;
	}
	else {
		//不是反向，则根据方向改变坐标
		switch (nDir)
		{
		case UP:
			TempPos.Y--;
			break;
		case DOWN:
			TempPos.Y++;
			break;
		case LEFT:
			TempPos.X--;
			break;
		case RIGHT:
			TempPos.X++;
			break;
		}
		//2.对移动后的点进行碰撞检测
		if (TankCollision(TempPos))
		{
			//2.1 如果可以移动，则擦掉原来的坐标，更改坐标，画出来
			m_pos = TempPos;
			m_nDir = nDir;
			DrawTank();
		}
		else //2.2 如果不可以移动，则不移动,只改变坦克的方向
		{
			//反向，则坐标不变,只改变方向，不需要碰撞检测
			m_nDir = nDir;
			DrawTank();
		}
	}
}

bool CTank::TankCollision(COORD pos)
{
	//遍历坦克数组，判断除了自己以外的坦克是否会被自己撞到
	for (int i = 0; i < 6; i++)
	{
		//排除自身
		if (this == &s_arrTank[i])
			continue;
		else
		{	//否则，以该点为中心的所有点都来测试一下子是不是会撞到
			for (short nX = pos.X - 1; nX < pos.X + 2; nX++)
				for (short nY = pos.Y - 1; nY < pos.Y + 2; nY++)
				{
					if (nX <= 0 || nX >= 39 || nY <= 0 || nY >= 39)
						continue;
					if (s_arrTank[i].TankBodyExist({ nX,nY }))
						return false;
				}
					
		}
	}
	//没有撞到坦克
	//由于坦克地图中，已经没有该坦克了，所以只需判断当前点能不能放就可以了
	if (pos.X >= 0 && pos.X < 40 && pos.Y >= 0 && pos.Y < 40 &&
		CTank::s_objTankMap.getMapType(pos) == 0)
	{
		return true;
	}
	return false;
}

bool CTank::TankBodyExist(COORD pos)
{
	for(short nX = m_pos.X-1; nX < m_pos.X+2; nX++)
		for (short nY = m_pos.Y - 1; nY < m_pos.Y + 2; nY++)
		{
			if (pos.X == nX && pos.Y == nY)
				return true;
		}
	return false;
}

void CTank::AddHp(int nNum)
{
	m_nHp += nNum;
}

void CTank::DecHp(int nNum)
{
	m_nHp -= nNum;
}

int CTank::GetHp()
{
	return m_nHp;
}

bool CTank::GetTankIsAlive()
{
	return m_nHp>0;
}

int CTank::GetClock()
{
	return m_nClock;
}

void CTank::SetClock(int nClock)
{
	m_nClock = nClock;
}

int CTank::GetType()
{
	return m_nType;
}

void CTank::SetType(int nType)
{
	m_nType = nType;
}

int CTank::GetSpeed()
{
	return m_nSpeed;
}

int CTank::GetTankDir()
{
	return m_nDir;
}

COORD CTank::GetPos()
{
	return m_pos;
}

void CTank::SetPos(COORD pos)
{
	m_pos = pos;
}

void CTank::SetHp(int nHp)
{
	m_nHp = nHp;
}

int CTank::GetPower()
{
	return m_nPower;
}

int CTank::GetID()
{
	return m_nID;
}

void CTank::SetID(int nID)
{
	m_nID = nID;
}

void CTank::AddKillNumber()
{
	m_nKill++;
}

int CTank::GetKillNumber()
{
	return m_nKill;
}
