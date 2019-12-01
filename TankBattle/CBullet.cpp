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
	//����Ҫ��ײ�жϣ�����Ҫд���ͼ��ֻ��Ҫ���ݷ���ı����꣬����������
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
	//3����ɫ���ӵ�����ͨ�ӵ�Ϊ��ɫ��������Ϊ2,����С��Ϊ��ɫ���������Ϊ��ɫ
	int nColor = 0x0E;
	if (m_nATK == 4)
		nColor = 0x04;
	else if (m_nATK == 1)
		nColor = 0x0F;
	WriteChar(m_pos.X, m_pos.Y, "��", nColor);
}

void CBullet::ClearBullet()
{
	WriteChar(m_pos.X, m_pos.Y, "  ");
}

bool CBullet::BulletCrash(CBullet objBullet)
{
	//����ж����������ӵ��Ƿ���Լ��ҵ��ӵ���ײ��?
	//��ײ����true��û����ײ�򷵻�false
	//ֱ����ײ
	if (m_pos.X == objBullet.GetPos().X&&
		m_pos.Y == objBullet.GetPos().Y)
		return true;

	//1.���ж��ӵ��Ƿ����Լ��ӵ��ĸ���λ
	int nArrDir[4][2] = { {0,-1},{0,1},{-1,0},{1,0} };
	for (int i = 0; i < 4; i++)
	{
		if (((m_pos.X + nArrDir[i][0]) == objBullet.GetPos().X) &&
			((m_pos.Y + nArrDir[i][0]) == objBullet.GetPos().Y))
		{
			//2.�����Լ��ӵ������жϳ�·���Ƿ���һ��ֱ����
			switch (m_nDir)
			{
			case UP:
			case DOWN:
			{
				if (m_pos.X == objBullet.GetPos().X)
				{
					//3.��������㣬���ж�2���ӵ��ķ����Ƿ��෴
					return (m_nDir + objBullet.GetDir() == 3) ? true : false;
				}
			}
			case LEFT:
			case RIGHT:
			{
				if (m_pos.Y == objBullet.GetPos().Y)
				{
					//3.��������㣬���ж�2���ӵ��ķ����Ƿ��෴
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

