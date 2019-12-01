#include "CBullet.h"



CBullet::CBullet()
{
}


CBullet::~CBullet()
{
}

void CBullet::MoveBullet()
{
	ClearBullet();
	//不需要碰撞判断，不需要写入地图，只需要根据方向改变坐标，画出来即可
	switch (m_nDir)
	{
	case UP:
		m_pos.Y--;
		break;
	case DOWN:
		m_pos.Y++;
		break;
	case LEFT:
		m_pos.X--;
		break;
	case RIGHT:
		m_pos.X++;
		break;
	default:
		break;
	}
	DrawBullet();
}

void CBullet::DrawBullet()
{
	//3种颜色的子弹，普通子弹为黄色，攻击力为2,威力小的为白色，威力大的为红色
	int nColor = 0x0E;
	if (m_nATK == 4)
		nColor = 0x04;
	else if (m_nATK == 1)
		nColor = 0x0F;
	WriteChar(m_pos.X, m_pos.Y, "●", nColor);
}

void CBullet::ClearBullet()
{
	WriteChar(m_pos.X, m_pos.Y, "  ");
}

bool CBullet::BulletCrash(CBullet objBullet)
{
	//如何判断所传对象子弹是否和自己家的子弹相撞呢?
	//碰撞返回true，没有碰撞则返回false
	//直接碰撞
	if (m_pos.X == objBullet.GetPos().X&&
		m_pos.Y == objBullet.GetPos().Y)
		return true;

	//1.先判断子弹是否在自家子弹四个方位
	int nArrDir[4][2] = { {0,-1},{0,1},{-1,0},{1,0} };
	for (int i = 0; i < 4; i++)
	{
		if (((m_pos.X + nArrDir[i][0]) == objBullet.GetPos().X) &&
			((m_pos.Y + nArrDir[i][0]) == objBullet.GetPos().Y))
		{
			//2.根据自家子弹方向，判断出路径是否在一条直线上
			switch (m_nDir)
			{
			case UP:
			case DOWN:
			{
				if (m_pos.X == objBullet.GetPos().X)
				{
					//3.如果都满足，则判断2个子弹的方向是否相反
					return (m_nDir + objBullet.GetDir() == 3) ? true : false;
				}
			}
			case LEFT:
			case RIGHT:
			{
				if (m_pos.Y == objBullet.GetPos().Y)
				{
					//3.如果都满足，则判断2个子弹的方向是否相反
					return (m_nDir + objBullet.GetDir() == 3) ? true : false;
				}
			}
			default:
				break;
			}
		}
	}
	return false;
}

int CBullet::GetClock()
{
	return m_nClock;
}

void CBullet::SetClock(int nClock)
{
	m_nClock = nClock;
}

COORD CBullet::GetPos()
{
	return m_pos;
}

bool CBullet::GetIsAlive()
{
	return m_bAlive;
}

void CBullet::SetIsAlive(bool bState)
{
	m_bAlive = bState;
}

int CBullet::GetSpeed()
{
	return m_nSpeed;
}

int CBullet::GetDir()
{
	return m_nDir;
}

int CBullet::GetATK()
{
	return m_nATK;
}

int CBullet::GetID()
{
	return m_nID;
}

bool CBullet::operator==(CTank objTank)
{
	return m_nCamp == objTank.GetType();
}

bool CBullet::operator=(CTank objTank)
{
	m_nCamp = objTank.GetType();
	m_nClock = clock();
	m_nDir = objTank.GetTankDir();
	m_nSpeed = objTank.GetSpeed()/6 *5;
	m_pos = objTank.GetPos();
	m_bAlive = true;
	m_nATK = objTank.GetPower();
	m_nID = objTank.GetID();

	switch (m_nDir)
	{
	case UP:
		m_pos.Y -= 2;
		break;
	case DOWN:
		m_pos.Y += 2;
		break;
	case LEFT:
		m_pos.X -= 2;
		break;
	case RIGHT:
		m_pos.X += 2;
		break;
	}
	return true;
}

