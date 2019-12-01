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
	//������Ϸ��ͼ���������������2��ǽ������̹�˵�ͼ���Ը�λ��Ϊ�е㣬��һȦ�ϰ���
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
			if (nX >= 0 && nX < 40 && nY >= 0 && nY < 40)	//���ڵ�ͼ��Χ��ʱ������
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
	//��̹���ڵ�ͼ�к���Ļ�ϻ���,����̹�����ͻ�����ͬ��ɫ��̹��,��̹����������̹������С�ٶȿ�
	//���̹��Ϊ��ɫ
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
	//�������ĵ㣬�ƶϳ���㣬ֱ�����9������
	//��̹�˴���Ļ��ɾ��
	for (short nY = m_pos.Y - 1; nY < m_pos.Y + 2; nY++)
	{
		WriteChar(m_pos.X - 1, nY, "      ");
	}
}

void CTank::MoveTank(int nDir)
{
	//�ƶ�
	//ֻҪ�����ƶ�����Ȼ�ػ�̹��
	//��̹�˵�ͼ�����̹��
	ClearTank();
	//1.������ƶ�������ĵ�
	COORD TempPos = m_pos;
	if (nDir + m_nDir == 3)
	{
		//���������겻��,ֻ�ı䷽�򣬲���Ҫ��ײ���
		m_nDir = nDir;
		DrawTank();
		return;
	}
	else {
		//���Ƿ�������ݷ���ı�����
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
		//2.���ƶ���ĵ������ײ���
		if (TankCollision(TempPos))
		{
			//2.1 ��������ƶ��������ԭ�������꣬�������꣬������
			m_pos = TempPos;
			m_nDir = nDir;
			DrawTank();
		}
		else //2.2 ����������ƶ������ƶ�,ֻ�ı�̹�˵ķ���
		{
			//���������겻��,ֻ�ı䷽�򣬲���Ҫ��ײ���
			m_nDir = nDir;
			DrawTank();
		}
	}
}

bool CTank::TankCollision(COORD pos)
{
	//����̹�����飬�жϳ����Լ������̹���Ƿ�ᱻ�Լ�ײ��
	for (int i = 0; i < 6; i++)
	{
		//�ų�����
		if (this == &s_arrTank[i])
			continue;
		else
		{	//�����Ըõ�Ϊ���ĵ����е㶼������һ�����ǲ��ǻ�ײ��
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
	//û��ײ��̹��
	//����̹�˵�ͼ�У��Ѿ�û�и�̹���ˣ�����ֻ���жϵ�ǰ���ܲ��ܷžͿ�����
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
