#include "CGameManager.h"


CGameManager::CGameManager()
{
}


CGameManager::~CGameManager()
{
}

void CGameManager::GameVarInit()
{
	//���ع��
	ShowCursor(false);
	//�л����뷨
	ChangeEnglish();
	//���ÿ���̨����ʹ�С
	SetWindowSize("̹�˴�սBy����rap", 120, 40);
	//���ű�������
	PlayMusic();
}

void CGameManager::GameRunInit()
{
	//��ʼ�����̹�˺͵з�̹��
	//���1
	CTank TempTank1({ 15,37 }, UP, PLAYER_TANK, NOMAL_SPEED, clock(), 2, 6);
	CTank::s_arrTank[0] = TempTank1;
	CTank::s_arrTank[0].SetID(0);
	//CTank::s_arrTank[0].DrawTank();
	//˫��ģʽ�ų�ʼ���ڶ���̹��,����ڶ���̹��ֱ��������״̬
	if (m_nGameMode == 2)
	{
		//���2
		CTank TempTank2({ 25,37 }, UP, PLAYER_TANK, NOMAL_SPEED, clock(), 2, 6);
		CTank::s_arrTank[1] = TempTank2;
		CTank::s_arrTank[1].SetID(1);
		//CTank::s_arrTank[1].DrawTank();
	}
	else {
		CTank::s_arrTank[1].SetHp(0);
	}
	//��ʼ���з�̹�ˣ���Ϸ��ѭ�����Զ���ʼ��
	for (int i = 2; i < 6; i++)
	{
		CTank::s_arrTank[i].SetType(ENEMY_TANK);
		CTank::s_arrTank[i].SetID(i);
	}
	//��ʼ������
	m_nScore1 = 0;
	m_nScore2 = 0;
}

int CGameManager::GameStart(bool bFlag)
{
	int nKill = m_nGameLevel * 10;
	//���ڴ����Ϸ״̬
	int nState = -1;
	//��ȡ��Ұ�������
	int nOper1 = -1;
	int nOper2 = -1;
	//������Ǵ浵ģʽ�����ʼ�����̹��
	if (!bFlag) {
		ResetTank(CTank::s_arrTank[0]);
		if (m_nGameMode == 2)
		{
			ResetTank(CTank::s_arrTank[1]);
		}
	}
	CTank::s_arrTank[0].DrawTank();
	if (m_nGameMode == 2)
	{
		CTank::s_arrTank[1].DrawTank();
	}
	while (nState == -1)
	{

		//������̹�����ˣ����˳���Ϸѭ�����˳���־Ϊ0����Ϸ����
		if (!CTank::s_arrTank[0].GetTankIsAlive() && !CTank::s_arrTank[1].GetTankIsAlive())
			nState = 0;
		//��������ˣ��ؿ����������˳���Ϸѭ�����˳���־Ϊ1
		if (CTank::s_arrTank[0].GetKillNumber() + CTank::s_arrTank[1].GetKillNumber() >= nKill)
		{
			m_nGameLevel++;
			nState = 1;
		}

		//��ʾ��Ϸ״̬��
		GameStautsBar();
		
		//�������z����ֱ�Ӵ浵
		if (KEY_DOWN('z')|| KEY_DOWN('Z'))
		{
			//�浵
			SaveStatus();
			//��һ��
			Sleep(200);
		}
		//��ȡ��������
		nOper1 = getPlayer1Input();
		nOper2 = getPlayer2Input();
		if (nOper1 != -1)	//���̹��1���Բ���
		{
			//������Ӧʱ����̹��û��
			if (clock() - CTank::s_arrTank[0].GetClock() > CTank::s_arrTank[0].GetSpeed() &&
				CTank::s_arrTank[0].GetTankIsAlive())
			{
				if (nOper1 != FIRE)	//�ƶ�
				{
					CTank::s_arrTank[0].MoveTank(nOper1);
					m_objMap.DrawReverAndLawn();
				}
				else	//����
				{
					PlayOp();
					CreateBullet(CTank::s_arrTank[0]);
				}
				//ֻҪ���1����������״̬����Ϊ��
				nOper1 = -1;
				//����̹��ʱ��
				CTank::s_arrTank[0].SetClock(clock());
			}
		}
		if (nOper2 != -1)	//���̹��2���Բ���
		{
			
			//������Ӧʱ����̹��û��
			if (clock() - CTank::s_arrTank[1].GetClock() > CTank::s_arrTank[1].GetSpeed() &&
				CTank::s_arrTank[1].GetTankIsAlive())
			{
				if (nOper2 != FIRE)		//�ƶ�
				{
					CTank::s_arrTank[1].MoveTank(nOper2);
					m_objMap.DrawReverAndLawn();
				}
				else	//����
				{
					PlayOp();
					CreateBullet(CTank::s_arrTank[1]);
				}
				//ֻҪ���2����������״̬����Ϊ��
				nOper2 = -1;
				//����̹��ʱ��
				CTank::s_arrTank[1].SetClock(clock());
			}
		}
		//�����з�̹�����飬���������Ӧʱ����û�������������
		for (int i = 2; i < 6; i++)
		{
			if (clock() - CTank::s_arrTank[i].GetClock() > CTank::s_arrTank[i].GetSpeed() &&
				CTank::s_arrTank[i].GetTankIsAlive())
			{
				//�����±����̹�˲���
				MoveNPCTank(i, 0);
				CTank::s_arrTank[i].SetClock(clock());
			}

		}
		//�����ӵ����飬���Ƿ񵽴���Ӧʱ��
		for (unsigned int i = 0; i < m_vecBullets.size(); i++)
		{
			if (clock() - m_vecBullets[i].GetClock() > m_vecBullets[i].GetSpeed())
			{
				//�ӵ��ƶ�
				m_vecBullets[i].MoveBullet();
				//�ӵ�����ײ����
				BulletCollision(m_vecBullets[i]);
				m_objMap.DrawReverAndLawn();
				//������Ӧʱ��
				m_vecBullets[i].SetClock(clock());
			}
		}
		//�����ӵ������������������Ч�ӵ�
		BulletManage();
		//���õз�̹���������������λ����̹�ˣ��ӷ�
		EnemyTankManage();
	}
	return nState;
}

void CGameManager::GameStautsBar()
{
	char szBuffer[40];
	if (m_nGameMode == 1)
	{
		WriteChar(45, 8, "����ģʽ", 0x0E);
	}
	else if (m_nGameMode == 2)
	{
		WriteChar(45, 8, "˫��ģʽ", 0x0E);

		WriteChar(42, 28, "���2:", 0x0E);
		
		WriteChar(42, 29, "Ѫ��:", 0x0E);
		sprintf_s(szBuffer, "%d", CTank::s_arrTank[1].GetHp());
		WriteChar(47, 29, szBuffer, 0x0B);

		WriteChar(42, 30, "�ٶ�:", 0x0E);
		sprintf_s(szBuffer, "%d", CTank::s_arrTank[1].GetSpeed());
		WriteChar(47, 30, szBuffer, 0x0B);

		WriteChar(42, 31, "������:", 0x0E);
		sprintf_s(szBuffer, "%d", CTank::s_arrTank[1].GetPower());
		WriteChar(47, 31, szBuffer, 0x0B);

		WriteChar(42, 32, "ɱ����:", 0x0E);
		sprintf_s(szBuffer, "%d", CTank::s_arrTank[1].GetKillNumber());
		WriteChar(47, 32, szBuffer, 0x0B);

		WriteChar(42, 33, "�÷�:", 0x0E);
		sprintf_s(szBuffer, "%d", m_nScore2);
		WriteChar(47, 33, szBuffer, 0x0B);
	}
	

	WriteChar(42, 5, "�з�̹������:", 0x0E);
	WriteChar(42, 6, "��ɫ: Ѹ����", 0x01);
	WriteChar(42, 7, "��ɫ: �ƻ���", 0x0C);

	WriteChar(42, 9, "����ָ��:", 0x0E);
	WriteChar(42, 11, "̹���ƶ�: ���1 ���2", 0x0E);
	WriteChar(42, 12, "��:	      W     UP", 0x0A);
	WriteChar(42, 13, "��:	      S     DOWN", 0x0B);
	WriteChar(42, 14, "��:	      A     LEFT", 0x0C);
	WriteChar(42, 15, "��:	      D     RIGHT", 0x0D);
	WriteChar(42, 16, "��Ϸ����:	 Z", 0x0E);

	WriteChar(42, 18, "��ǰ�ؿ�:", 0x0E);
	sprintf_s(szBuffer, "%d", m_nGameLevel);
	WriteChar(47, 18, szBuffer, 0x0B);

	WriteChar(42, 20, "���״̬:", 0x0E);
	WriteChar(42, 21, "���1:", 0x0E);
	WriteChar(42, 22, "Ѫ��:", 0x0E);
	sprintf_s(szBuffer, "%d", CTank::s_arrTank[0].GetHp());
	WriteChar(47, 22, szBuffer, 0x0B);

	WriteChar(42, 23, "�ٶ�:", 0x0E);
	sprintf_s(szBuffer, "%d", CTank::s_arrTank[0].GetSpeed());
	WriteChar(47, 23, szBuffer, 0x0B);

	WriteChar(42, 24, "������:", 0x0E);
	sprintf_s(szBuffer, "%d", CTank::s_arrTank[0].GetPower());
	WriteChar(47, 24, szBuffer, 0x0B);

	WriteChar(42, 25, "ɱ����:", 0x0E);
	sprintf_s(szBuffer, "%d", CTank::s_arrTank[0].GetKillNumber());
	WriteChar(47, 25, szBuffer, 0x0B);

	WriteChar(42, 26, "�÷�:", 0x0E);
	sprintf_s(szBuffer, "%d", m_nScore1);
	WriteChar(47, 26, szBuffer, 0x0B);
}

void CGameManager::GameUI()
{
	//�����������Ƿ�浵��������ʼ��������Ϸ�����б�������ͼ��̹��λ�ã�̹��Ѫ��
	m_objMap.ClearShowMap();
	//���ڴ�Ŷ�����־
	bool bFlag = false;
	WriteChar(15, 20, "����Ҫ����ô?", 0x0E);
	WriteChar(15, 22, "j.��   k.��", 0x0E);
	while (true)
	{
		if (_kbhit())//�Ƿ��¼���
		{
			char ch = _getch();
			if (ch == 'j' || ch == 'J')
			{
				//����
				LoadStatus();
				bFlag = true;
				break;
			}
			else if (ch == 'k' || ch == 'K')
			{
				GameRunInit();
				break;
			}
		}
	}

	//��ȡ��ǰ��Ϸ״̬
	int nState = 1;
	
	//��ȡ��ǰ�Ĺؿ�
	while (nState)
	{
		//������Ǵ浵״̬������ص�ͼ
		if (!bFlag)
		{
			//���ݹؿ����ص�ͼ
			LoadMap(m_nGameLevel);
		}
		//����̹�˵�ͼ
		CTank::TransformMap(m_objMap);
		m_objMap.DrawMap();
		//������Ϸ������ѭ��
		nState = GameStart(bFlag);
		//����ӵ�����
		m_vecBullets.clear();
		//�����ǰ������Ϸ��ͼ��Χ�����˳���Ϸ
		if (m_nGameLevel > 2)
			break;
		//������Σ�����һ�κ󣬸ñ�־λ��Ϊfalse�ˡ�
		bFlag = false;
	}
	//��Ϸ��������
	if (nState)
	{
		//ͨ����
		m_objMap.ClearShowMap();
		WriteChar(15, 20, "��ϲ��ͨ����!!", 0x0E);
	}
	else
	{
		//����ʧ��
		m_objMap.ClearShowMap();
		WriteChar(15, 20, "����ʧ��!!", 0x0E);
	}
	system("pause");
}

void CGameManager::BeginUI()
{	
	//���ڴ洢ѡ�������ַ���
	char szBuffer[4][20] = { "1.����ģʽ","2.˫��ģʽ","3.��ͼ�༭","4.�˳���Ϸ" };
	//���ڱ����û���ѡ��
	int nIndex = 1;
	//���ڱ���ѡ���������
	int nKeyTemp = 1;
	//�ж��Ƿ��а�������
	int nFlag = 1;

	//����
	system("cls");
	//����̹��ͼ��
	LoadMap(3);
	m_objMap.DrawMap();
	//��̹��ͼ���±ߣ������û�ѡ�����ѡ�������
	//1.��������
	WriteChar(15, 31, "ģʽѡ��:", 0x0E);
	//2.ѭ����ⰴ�����ı�ѡ��
	//3.J��ȷ��
	while (true)
	{
		//����а������£����ݵ�ǰ������������3��ѡ��
		if (nFlag)
		{
			nFlag = 0;
			for (int i = 0; i < 4; i++)
			{
				if (i + 1 == nKeyTemp)
					WriteChar(15, 33 + i, szBuffer[i], 0x06);
				else
					WriteChar(15, 33 + i, szBuffer[i]);
			}
		}
		//��ȡ�û�����
		if (_kbhit()) {
			char ch = _getch();
			if (ch == 'j' || ch == 'J')
				break;
			else if (ch == 'w' || ch == 'W')
			{
				nKeyTemp--;
				nFlag = 1;
			}
			else if (ch == 's' || ch == 'S')
			{
				nKeyTemp++;
				nFlag = 1;
			}
		}
		if (nKeyTemp < 1)
			nKeyTemp += 4;
		else if (nKeyTemp > 4)
			nKeyTemp -= 4;
	}
	m_nGameMode = nKeyTemp;
}

void CGameManager::Run()
{

	//��Ϸ��ǰ�ؿ�Ϊ1
	m_nGameLevel = 1;
	//��ʾ��ӭ����
	LoadMap(0);
	m_objMap.DrawMap();
	//��ʱ2s
	Sleep(2000);
	
	while (m_nGameMode!=4)
	{
		//ִ����Ϻ���Ϸģʽ�ı�
		BeginUI();
		switch (m_nGameMode)
		{
		case 1:
		case 2:
			//����˫�˹���һ�״���
			GameUI();
			break;
		case 3:
			//��ͼ�༭
			EditMap();
			break;
		}
	}
	
}

void CGameManager::SaveStatus()
{
	//����̹������
	CTank::SaveTank();
	//�����ͼ����
	SaveMap(4);
	//�����ӵ����ݺ���Ϸ�ؿ�
	FILE *pFile = nullptr;
	fopen_s(&pFile, ".//resource//data//BulletData.bin", "wb+");
	if (pFile == nullptr)
	{
		exit(0);
	}
	//������Ϸ�ؿ�
	fwrite(&m_nGameLevel, sizeof(int), 1, pFile);
	//����÷�
	fwrite(&m_nScore1, sizeof(int), 1, pFile);
	fwrite(&m_nScore2, sizeof(int), 1, pFile);
	//ѭ�����浱ǰ���е��ӵ�����
	for (unsigned int i=0; i<m_vecBullets.size();i++)
	{
		char *p = (char *)&m_vecBullets[i];
		fwrite(p, sizeof(CBullet),1, pFile);
	}
	fclose(pFile);
}

void CGameManager::LoadStatus()
{
	//����̹��״̬
	CTank::LoadTank();
	//���ص�ͼ
	LoadMap(4);
	//������Ϸ�ؿ����ӵ�
	FILE *pFile = nullptr;
	fopen_s(&pFile, ".//resource//data//BulletData.bin", "rb");
	if (pFile == nullptr)
	{
		exit(0);
	}
	//��ȡ��Ϸ�ؿ�
	fread(&m_nGameLevel, sizeof(int), 1, pFile);
	//��ȡ�÷�
	fread(&m_nScore1, sizeof(int), 1, pFile);
	fread(&m_nScore2, sizeof(int), 1, pFile);
	CBullet temp;
	while (fread(&temp, sizeof(CBullet), 1, pFile) != 0)
	{
		m_vecBullets.push_back(temp);
	}
	fclose(pFile);
}

void CGameManager::EditMapStatusBar()
{
	WriteChar(45, 8, "��ͼ�༭",0x0E);
	WriteChar(42, 10, "��ǰѡ��:", 0x0E);
	WriteChar(42, 12, "����ѡ��:", 0x0E);
	WriteChar(42, 13, "1.שǽ:��", 0x0C);
	WriteChar(42, 14, "2.��ǽ:��", 0x0F);
	WriteChar(42, 15, "3.�ݵ�:��", 0x0A);
	WriteChar(42, 16, "4.����:��", 0x03);

	WriteChar(42, 18, "��굥��:", 0x0E);
	WriteChar(42, 19, "����/�Ƴ�", 0x0B);
}

void CGameManager::LoadMap(int nIndex)
{
	//5����ͼ�ļ�����
	const char FileName[5][40] = { 
		".//resource//data//welcome.bin" ,
		".//resource//data//Map1.bin"  ,
		".//resource//data//Map2.bin"  ,
		".//resource//data//Image.bin"  ,
		".//resource//data//SaveData.bin" };
	char szFileNameBuffer[40];
	memcpy_s(szFileNameBuffer, sizeof(szFileNameBuffer), FileName[nIndex], sizeof(FileName[nIndex]));
	//������ѡ��ͼ�ļ������ּ�����Ӧ�ĵ�ͼ
	m_objMap.LoadMap(szFileNameBuffer);
}

void CGameManager::SaveMap(int nIndex)
{
	//5����ͼ�ļ�����
	const char FileName[5][40] = {
		".//resource//data//welcome.bin" ,
		".//resource//data//Map1.bin"  ,
		".//resource//data//Map2.bin"  ,
		".//resource//data//Image.bin"  ,
		".//resource//data//SaveData.bin" };
	char szFileNameBuffer[40];
	memcpy_s(szFileNameBuffer, sizeof(szFileNameBuffer), FileName[nIndex], sizeof(FileName[nIndex]));
	//������ѡ��ͼ�ļ������ֱ�����Ӧ�ĵ�ͼ
	m_objMap.SaveMap(szFileNameBuffer);
}

void CGameManager::SaveMapUI()
{
	//���ڴ洢ѡ�������ַ���
	char szBuffer[2][20] = { "1.�ؿ���ͼ1","2.�ؿ���ͼ2"};
	//���ڱ����û���ѡ��
	int nIndex = 1;
	//���ڱ���ѡ���������
	int nKeyTemp = 1;
	//�ж��Ƿ��а�������
	int nFlag = 1;
	//��յ�ͼ��ʾ,�ڵ�ͼ�����жϵ�ǰ�ؿ�ѡ��
	m_objMap.ClearShowMap();
	//��������
	WriteChar(17, 13, "��ͼѡ��:", 0x0C);
	//2.ѭ����ⰴ�����ı�ѡ��
	//3.J��ȷ��
	while (true)
	{
		//����а������£����ݵ�ǰ������������2��ѡ��
		if (nFlag)
		{
			nFlag = 0;
			for (int i = 0; i < 2; i++)
			{
				if (i + 1 == nKeyTemp)
					WriteChar(17, 15 + i * 2, szBuffer[i], 0x06);
				else
					WriteChar(17, 15 + i * 2, szBuffer[i]);
			}
		}
		//��ȡ�û�����
		if (_kbhit()) {
			char ch = _getch();
			if (ch == 'j'||ch == 'J')
				break;
			else if (ch == 'w' || ch == 'W')
			{
				nKeyTemp--;
				nFlag = 1;
			}
			else if (ch == 's' || ch == 'S')
			{
				nKeyTemp++;
				nFlag = 1;
			}
		}
		if (nKeyTemp < 1)
			nKeyTemp += 2;
		else if (nKeyTemp > 2)
			nKeyTemp -= 2;
	}
	SaveMap(nKeyTemp);
}

void CGameManager::EditMap()
{
	//����
	system("cls");
	//��ʼ��������������ͼ
	m_objMap.InitMap();
	m_objMap.DrawMap();
	//������ʾ��
	EditMapStatusBar();
	//��ȡ���̰���������ѡ��5�����Σ���ȡ������¼�������Ӧ�ĵ�ͼλ������ֵ
	int nType = WALL_NO;

	//���������Ӧʱ��Ļ�ȡǰ������
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD stcRecord = { 0 };
	DWORD dwRead;
	SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	while (true)
	{
		char szNum[10];
		sprintf_s(szNum, "%d", nType);
		WriteChar(48, 10, szNum,0x0B);
		//�ȴ��¼�
		ReadConsoleInput(hStdin, &stcRecord, 1, &dwRead);
		
		//����¼�����
		//����ǰ����¼����ݰ���ѡ��ģʽ
		if (stcRecord.EventType == KEY_EVENT)
		{
			if (stcRecord.Event.KeyEvent.bKeyDown) {
				switch (stcRecord.Event.KeyEvent.uChar.AsciiChar)
				{
				case '1':
					nType = WALL_1;
					break;
				case '2':
					nType = WALL_2;
					break;
				case '3':
					nType = LAWN;
					break;
				case '4':
					nType = RIVER;
				default:
					break;
				}
				if (stcRecord.Event.KeyEvent.uChar.AsciiChar == '\r')
				{
					//����ǰ��ͼת����̹�˵�ͼ��
					CTank::TransformMap(m_objMap);
					break;
				}	
			}
		}
		else if (stcRecord.EventType == MOUSE_EVENT)
		{		
			//���������¼�����ֵ
			//�Ȼ�ȡ�������Ļ�ϵ�λ��
			short nX = stcRecord.Event.MouseEvent.dwMousePosition.X/2;
			short nY = stcRecord.Event.MouseEvent.dwMousePosition.Y;
			switch (stcRecord.Event.MouseEvent.dwEventFlags)
			{
			case 0:
				//�������
				if (stcRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					if (m_objMap.getMapType({ nX,nY }) == WALL_NO)
					{
						m_objMap.setMapType({ nX,nY }, nType);
						m_objMap.DrawRange({ nX,nY });
					}
					else
					{
						m_objMap.setMapType({ nX,nY }, WALL_NO);
						m_objMap.DrawRange({ nX,nY });
					}
				}
			case MOUSE_MOVED:
				nX = stcRecord.Event.MouseEvent.dwMousePosition.X;
				nY = stcRecord.Event.MouseEvent.dwMousePosition.Y;
				break;
			}
		}
	}
	//��յ�ͼ��ʾ,�ڵ�ͼ�����ж��Ƿ�浵
	m_objMap.ClearShowMap();
	WriteChar(15, 20, "����Ҫ�����ͼô?", 0x0C);
	WriteChar(15, 22, "j.�����ͼ k.ȡ������",0x06);
	//�����������İ����ж��Ƿ�����ͼ�������
	while (true)
	{
		if (_kbhit())//�Ƿ��¼���
		{
			char ch = _getch();
			if (ch == 'j' || ch == 'J')
			{
				//�����ͼ������棬ѡ��浵�ĵ�ͼ���ؿ�1���ؿ�2��
				SaveMapUI();
				break;
			}
			else if (ch == 'k' || ch == 'K')
			{
				break;
			}
		}
	}
}

void CGameManager::CreateBullet(CTank objTank)
{
	//1.����̹�˳�ʼ���ӵ�����
	CBullet TempBullet;
	TempBullet = objTank; 
	//2.С�͵���ײ��⣬��������ϰ������Ҳ�Խ�磬������ӵ�����,�����ӵ�,������ִ�н���
	COORD pos = TempBullet.GetPos();
	int nType = m_objMap.getMapType(pos);
	if (nType != WALL_1 && nType != WALL_2 &&
		pos.X > 0 && pos.X < 39 && pos.Y>0 && pos.Y < 39)
	{
		m_vecBullets.push_back(TempBullet);
		TempBullet.DrawBullet();
	}
}

void CGameManager::BulletCollision(CBullet &objBullet)
{
	//���ݴ����ӵ���λ��λ���жϵ�ǰ�ӵ���״̬��ײ�����ⶫ������ǰ�ӵ����״̬��Ϊ��
	//ײ���ϰ���ײ��̹�ˣ�ײ�������ӵ�������ʲô��ûײ��
	//1.���ж��Ƿ�ײ���ϰ����̹��
	COORD pos = objBullet.GetPos();
	int nType = m_objMap.getMapType(pos);
	switch (nType)
	{
	//���ǲݵػ��ߺ������������̹�ˣ�ֻ����ӵ�
	case RIVER:
	case LAWN:
		break;
	case WALL_1:
	{	//����שǽ����ֱ����ɱ��ǽҲ�������ѵ�ͼ����̹�˵�ͼ,�������ٺ�ĵ�ͼ
		objBullet.SetIsAlive(false);
		m_objMap.RemoveRange(pos);
		CTank::TransformMap(m_objMap);
		m_objMap.DrawRange(pos);
		return;
		break;
	}
	case WALL_2:
	{
		//������ǽ���ӵ���ɱ,ɾ���ӵ�,������ǽ
		objBullet.SetIsAlive(false);
		m_objMap.DrawImage(pos, WALL_2);
		return;
		break;
	}
	case WALL_NO:
	{
		//���ǿյأ�����̹�˺��ӵ�
		//2.����̹�����飬���Ƿ�ײ��̹��
		for (int i = 0; i < 6; i++)
		{
			//�����ǰ̹���Ѿ����������ж���
			if (!CTank::s_arrTank[i].GetTankIsAlive())
				continue;
			if (CTank::s_arrTank[i].TankBodyExist(pos))	//�ڵз�̹������
			{

				//�ӵ�������������Ӫ�ж��Ƿ��Ѫ
				objBullet.SetIsAlive(false);
				if (!(objBullet == CTank::s_arrTank[i]))//����ͬһ��Ӫ����Ѫ
				{
					//���ű�������Ч
					PlayHit();
					//��Ѫ
					CTank::s_arrTank[i].DecHp(objBullet.GetATK());
				}
				//�����ǰ̹�˱������ˣ���ֱ�Ӳ�����ͬʱ�����ӵ�id��������Ӧ��ҵ�ɱ���������򻭳���
				if (CTank::s_arrTank[i].GetTankIsAlive())
					CTank::s_arrTank[i].DrawTank();
				else
				{
					CTank::s_arrTank[i].ClearTank();
					PlayDeath();
					int nId = objBullet.GetID();
					if (nId == 0)
					{
						CTank::s_arrTank[0].AddKillNumber();
						m_nScore1 += CTank::s_arrTank[i].GetPower() * 100;
					}
					else if (nId == 1)
					{
						CTank::s_arrTank[1].AddKillNumber();
						m_nScore2 += CTank::s_arrTank[i].GetPower() * 100;
					}
					
				}
			}
		}
	}
	}

	//3.�����ӵ����飬���Ƿ�ײ���ӵ���ֻҪײ�����ӵ�����
	for (unsigned int i = 0; i < m_vecBullets.size(); i++)
	{
		//���Լ��ų�
		if (&objBullet == &m_vecBullets[i])
			continue;
		//����Ѿ���ײ
		if (objBullet.BulletCrash(m_vecBullets[i]))
		{
			objBullet.SetIsAlive(false);
			objBullet.ClearBullet();
			m_vecBullets[i].SetIsAlive(false);
			m_objMap.DrawReverAndLawn();
		}
	}
}

void CGameManager::BulletManage()
{
	CBullet TempObj;
	for (auto item = m_vecBullets.cbegin(); item != m_vecBullets.cend();)
	{
		TempObj = *item;
		if (!TempObj.GetIsAlive())	//�����ǰ�ӵ��Ѿ�����
		{
			item = m_vecBullets.erase(item);
		}
		else
		{
			++item;
		}
	}
}

void CGameManager::ResetTank(CTank& objTank)
{
	//�Ȳ����Լ�
	objTank.ClearTank();
	//���Ϊ����̹�ˣ���˵�������ˣ��ָ�Ѫ��������̹��λ�ã�����������
	if (objTank.GetType() == PLAYER_TANK)
	{
		objTank.SetHp(6);
		if(objTank.GetID()==0)
			objTank.SetPos({ 15,37 });
		else
			objTank.SetPos({ 25,37 });
	}
	else
	{
		//�ٶ��ͣ��ٶ�ΪFAST_SPEED��Ѫ��Ϊ2��������Ϊ1
		//�����ͣ��ٶ�ΪLOW_SPEED��Ѫ��Ϊ4��������Ϊ4
		int nHp = 0;
		int nSpeed = 0;
		int nPower = 0;
		int nType = rand() % 2;
		if (nType)
		{
			nHp = 2;
			nSpeed = FAST_SPEED;
			nPower = 1;
		}
		else
		{
			nHp = 4;
			nSpeed = LOW_SPEED;
			nPower = 4;
		}
		//�ĸ��㣬ֻҪ��һ�����ܷţ��ͷ���ȥ
		if (objTank.TankCollision({ 15, 3 }))	//�����һ����û���ϰ���
		{
			CTank TempTank({ 15,3 }, DOWN, ENEMY_TANK, nSpeed, clock(), nPower, nHp);
			TempTank.SetID(objTank.GetID());
			objTank = TempTank;
		}
		else if (objTank.TankCollision({ 25,3 }))//����ڶ�����û���ϰ���
		{
			CTank TempTank({ 25,3 }, DOWN, ENEMY_TANK, nSpeed, clock(), nPower, nHp);
			TempTank.SetID(objTank.GetID());
			objTank = TempTank;
		}
		else if (objTank.TankCollision({ 3, 20 }))//�����������û���ϰ���
		{
			CTank TempTank({ 3, 20 }, RIGHT, ENEMY_TANK, nSpeed, clock(), nPower, nHp);
			TempTank.SetID(objTank.GetID());
			objTank = TempTank;
		}
		else if (objTank.TankCollision({ 37, 20 }))//������ĸ���û���ϰ���
		{
			CTank TempTank({ 37, 20 }, LEFT, ENEMY_TANK, nSpeed, clock(), nPower, nHp);
			TempTank.SetID(objTank.GetID());
			objTank = TempTank;
		}
	}
	//�����Լ�
	objTank.DrawTank();
	m_objMap.DrawReverAndLawn();
}

void CGameManager::MoveNPCTank(int nID, int nType)
{
	if (nType == 0)//����ƶ�
	{
		//�������ǰ���ƶ����ǿ��ڣ��ƶ����ʵô��ڿ���
		bool nFire = (rand() % 10 < 2 ? true:false);
		//���������
		int nDir = rand() % 4;
		if (nFire)
		{
			CreateBullet(CTank::s_arrTank[nID]);
		}
		else
		{
			CTank::s_arrTank[nID].MoveTank(nDir);
			m_objMap.DrawReverAndLawn();
		}
	}
}

void CGameManager::EnemyTankManage()
{
	//���̹��������������
	for (int i = 2; i < 6; i++)
	{
		if (!CTank::s_arrTank[i].GetTankIsAlive())
		{
			//����̹��
			ResetTank(CTank::s_arrTank[i]);
		}
	}
}

int CGameManager::getPlayer1Input()
{
	if (KEY_DOWN('W') || KEY_DOWN('w'))
		return UP;
	if (KEY_DOWN('S') || KEY_DOWN('s'))
		return DOWN;
	if (KEY_DOWN('A') || KEY_DOWN('a'))
		return LEFT;
	if (KEY_DOWN('D') || KEY_DOWN('d'))
		return RIGHT;
	if (KEY_DOWN('J') || KEY_DOWN('j'))
		return FIRE;
	return -1;
}

int CGameManager::getPlayer2Input()
{
	if (KEY_DOWN(VK_UP))
		return UP;
	if (KEY_DOWN(VK_DOWN))
		return DOWN;
	if (KEY_DOWN(VK_LEFT))
		return LEFT;
	if (KEY_DOWN(VK_RIGHT))
		return RIGHT;
	if (KEY_DOWN(VK_RETURN))
		return FIRE;
	return -1;
}

