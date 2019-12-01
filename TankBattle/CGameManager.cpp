#include "CGameManager.h"


CGameManager::CGameManager()
{
}


CGameManager::~CGameManager()
{
}

void CGameManager::GameVarInit()
{
	//隐藏光标
	ShowCursor(false);
	//切换输入法
	ChangeEnglish();
	//设置控制台标题和大小
	SetWindowSize("坦克大战By唱跳rap", 120, 40);
	//播放背景音乐
	PlayMusic();
}

void CGameManager::GameRunInit()
{
	//初始化玩家坦克和敌方坦克
	//玩家1
	CTank TempTank1({ 15,37 }, UP, PLAYER_TANK, NOMAL_SPEED, clock(), 2, 6);
	CTank::s_arrTank[0] = TempTank1;
	CTank::s_arrTank[0].SetID(0);
	//CTank::s_arrTank[0].DrawTank();
	//双人模式才初始化第二个坦克,否则第二个坦克直接是死亡状态
	if (m_nGameMode == 2)
	{
		//玩家2
		CTank TempTank2({ 25,37 }, UP, PLAYER_TANK, NOMAL_SPEED, clock(), 2, 6);
		CTank::s_arrTank[1] = TempTank2;
		CTank::s_arrTank[1].SetID(1);
		//CTank::s_arrTank[1].DrawTank();
	}
	else {
		CTank::s_arrTank[1].SetHp(0);
	}
	//初始化敌方坦克，游戏主循环里自动初始化
	for (int i = 2; i < 6; i++)
	{
		CTank::s_arrTank[i].SetType(ENEMY_TANK);
		CTank::s_arrTank[i].SetID(i);
	}
	//初始化分数
	m_nScore1 = 0;
	m_nScore2 = 0;
}

int CGameManager::GameStart(bool bFlag)
{
	int nKill = m_nGameLevel * 10;
	//用于存放游戏状态
	int nState = -1;
	//获取玩家按键操作
	int nOper1 = -1;
	int nOper2 = -1;
	//如果不是存档模式，则初始化玩家坦克
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

		//如果玩家坦克凉了，则退出游戏循环，退出标志为0，游戏结束
		if (!CTank::s_arrTank[0].GetTankIsAlive() && !CTank::s_arrTank[1].GetTankIsAlive())
			nState = 0;
		//如果过关了，关卡自增，则退出游戏循环，退出标志为1
		if (CTank::s_arrTank[0].GetKillNumber() + CTank::s_arrTank[1].GetKillNumber() >= nKill)
		{
			m_nGameLevel++;
			nState = 1;
		}

		//显示游戏状态栏
		GameStautsBar();
		
		//如果按下z，则直接存档
		if (KEY_DOWN('z')|| KEY_DOWN('Z'))
		{
			//存档
			SaveStatus();
			//卡一下
			Sleep(200);
		}
		//获取按键输入
		nOper1 = getPlayer1Input();
		nOper2 = getPlayer2Input();
		if (nOper1 != -1)	//玩家坦克1尝试操作
		{
			//到达响应时间且坦克没凉
			if (clock() - CTank::s_arrTank[0].GetClock() > CTank::s_arrTank[0].GetSpeed() &&
				CTank::s_arrTank[0].GetTankIsAlive())
			{
				if (nOper1 != FIRE)	//移动
				{
					CTank::s_arrTank[0].MoveTank(nOper1);
					m_objMap.DrawReverAndLawn();
				}
				else	//开炮
				{
					PlayOp();
					CreateBullet(CTank::s_arrTank[0]);
				}
				//只要玩家1操作，操作状态就置为空
				nOper1 = -1;
				//重置坦克时间
				CTank::s_arrTank[0].SetClock(clock());
			}
		}
		if (nOper2 != -1)	//玩家坦克2尝试操作
		{
			
			//到达响应时间且坦克没凉
			if (clock() - CTank::s_arrTank[1].GetClock() > CTank::s_arrTank[1].GetSpeed() &&
				CTank::s_arrTank[1].GetTankIsAlive())
			{
				if (nOper2 != FIRE)		//移动
				{
					CTank::s_arrTank[1].MoveTank(nOper2);
					m_objMap.DrawReverAndLawn();
				}
				else	//开炮
				{
					PlayOp();
					CreateBullet(CTank::s_arrTank[1]);
				}
				//只要玩家2操作，操作状态就置为空
				nOper2 = -1;
				//重置坦克时间
				CTank::s_arrTank[1].SetClock(clock());
			}
		}
		//遍历敌方坦克数组，如果到达响应时间且没有死亡，则操作
		for (int i = 2; i < 6; i++)
		{
			if (clock() - CTank::s_arrTank[i].GetClock() > CTank::s_arrTank[i].GetSpeed() &&
				CTank::s_arrTank[i].GetTankIsAlive())
			{
				//根据下标进行坦克操作
				MoveNPCTank(i, 0);
				CTank::s_arrTank[i].SetClock(clock());
			}

		}
		//遍历子弹数组，看是否到达响应时间
		for (unsigned int i = 0; i < m_vecBullets.size(); i++)
		{
			if (clock() - m_vecBullets[i].GetClock() > m_vecBullets[i].GetSpeed())
			{
				//子弹移动
				m_vecBullets[i].MoveBullet();
				//子弹的碰撞处理
				BulletCollision(m_vecBullets[i]);
				m_objMap.DrawReverAndLawn();
				//重置响应时间
				m_vecBullets[i].SetClock(clock());
			}
		}
		//调用子弹数组整理函数，清空无效子弹
		BulletManage();
		//调用敌方坦克数组管理函数，置位死亡坦克，加分
		EnemyTankManage();
	}
	return nState;
}

void CGameManager::GameStautsBar()
{
	char szBuffer[40];
	if (m_nGameMode == 1)
	{
		WriteChar(45, 8, "单人模式", 0x0E);
	}
	else if (m_nGameMode == 2)
	{
		WriteChar(45, 8, "双人模式", 0x0E);

		WriteChar(42, 28, "玩家2:", 0x0E);
		
		WriteChar(42, 29, "血量:", 0x0E);
		sprintf_s(szBuffer, "%d", CTank::s_arrTank[1].GetHp());
		WriteChar(47, 29, szBuffer, 0x0B);

		WriteChar(42, 30, "速度:", 0x0E);
		sprintf_s(szBuffer, "%d", CTank::s_arrTank[1].GetSpeed());
		WriteChar(47, 30, szBuffer, 0x0B);

		WriteChar(42, 31, "攻击力:", 0x0E);
		sprintf_s(szBuffer, "%d", CTank::s_arrTank[1].GetPower());
		WriteChar(47, 31, szBuffer, 0x0B);

		WriteChar(42, 32, "杀敌数:", 0x0E);
		sprintf_s(szBuffer, "%d", CTank::s_arrTank[1].GetKillNumber());
		WriteChar(47, 32, szBuffer, 0x0B);

		WriteChar(42, 33, "得分:", 0x0E);
		sprintf_s(szBuffer, "%d", m_nScore2);
		WriteChar(47, 33, szBuffer, 0x0B);
	}
	

	WriteChar(42, 5, "敌方坦克类型:", 0x0E);
	WriteChar(42, 6, "蓝色: 迅捷者", 0x01);
	WriteChar(42, 7, "红色: 破坏者", 0x0C);

	WriteChar(42, 9, "操作指南:", 0x0E);
	WriteChar(42, 11, "坦克移动: 玩家1 玩家2", 0x0E);
	WriteChar(42, 12, "上:	      W     UP", 0x0A);
	WriteChar(42, 13, "下:	      S     DOWN", 0x0B);
	WriteChar(42, 14, "左:	      A     LEFT", 0x0C);
	WriteChar(42, 15, "右:	      D     RIGHT", 0x0D);
	WriteChar(42, 16, "游戏保存:	 Z", 0x0E);

	WriteChar(42, 18, "当前关卡:", 0x0E);
	sprintf_s(szBuffer, "%d", m_nGameLevel);
	WriteChar(47, 18, szBuffer, 0x0B);

	WriteChar(42, 20, "玩家状态:", 0x0E);
	WriteChar(42, 21, "玩家1:", 0x0E);
	WriteChar(42, 22, "血量:", 0x0E);
	sprintf_s(szBuffer, "%d", CTank::s_arrTank[0].GetHp());
	WriteChar(47, 22, szBuffer, 0x0B);

	WriteChar(42, 23, "速度:", 0x0E);
	sprintf_s(szBuffer, "%d", CTank::s_arrTank[0].GetSpeed());
	WriteChar(47, 23, szBuffer, 0x0B);

	WriteChar(42, 24, "攻击力:", 0x0E);
	sprintf_s(szBuffer, "%d", CTank::s_arrTank[0].GetPower());
	WriteChar(47, 24, szBuffer, 0x0B);

	WriteChar(42, 25, "杀敌数:", 0x0E);
	sprintf_s(szBuffer, "%d", CTank::s_arrTank[0].GetKillNumber());
	WriteChar(47, 25, szBuffer, 0x0B);

	WriteChar(42, 26, "得分:", 0x0E);
	sprintf_s(szBuffer, "%d", m_nScore1);
	WriteChar(47, 26, szBuffer, 0x0B);
}

void CGameManager::GameUI()
{
	//清屏，根据是否存档读档，初始化单人游戏的所有变量，地图，坦克位置，坦克血量
	m_objMap.ClearShowMap();
	//用于存放读档标志
	bool bFlag = false;
	WriteChar(15, 20, "您想要读档么?", 0x0E);
	WriteChar(15, 22, "j.是   k.否", 0x0E);
	while (true)
	{
		if (_kbhit())//是否按下键盘
		{
			char ch = _getch();
			if (ch == 'j' || ch == 'J')
			{
				//读档
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

	//获取当前游戏状态
	int nState = 1;
	
	//获取当前的关卡
	while (nState)
	{
		//如果不是存档状态，则加载地图
		if (!bFlag)
		{
			//根据关卡加载地图
			LoadMap(m_nGameLevel);
		}
		//放入坦克地图
		CTank::TransformMap(m_objMap);
		m_objMap.DrawMap();
		//进入游戏运行主循环
		nState = GameStart(bFlag);
		//清空子弹数组
		m_vecBullets.clear();
		//如果当前超出游戏地图范围，则退出游戏
		if (m_nGameLevel > 2)
			break;
		//无论如何，加载一次后，该标志位都为false了。
		bFlag = false;
	}
	//游戏结束画面
	if (nState)
	{
		//通关了
		m_objMap.ClearShowMap();
		WriteChar(15, 20, "恭喜你通关啦!!", 0x0E);
	}
	else
	{
		//闯关失败
		m_objMap.ClearShowMap();
		WriteChar(15, 20, "闯关失败!!", 0x0E);
	}
	system("pause");
}

void CGameManager::BeginUI()
{	
	//用于存储选择界面的字符串
	char szBuffer[4][20] = { "1.单人模式","2.双人模式","3.地图编辑","4.退出游戏" };
	//用于保存用户的选择
	int nIndex = 1;
	//用于保存选择的亮度条
	int nKeyTemp = 1;
	//判断是否有按键按下
	int nFlag = 1;

	//清屏
	system("cls");
	//加载坦克图案
	LoadMap(3);
	m_objMap.DrawMap();
	//在坦克图案下边，根据用户选择决定选项，高亮等
	//1.画出标题
	WriteChar(15, 31, "模式选择:", 0x0E);
	//2.循环检测按键，改变选项
	//3.J键确认
	while (true)
	{
		//如果有按键按下，根据当前的亮度条画出3个选项
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
		//获取用户输入
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

	//游戏当前关卡为1
	m_nGameLevel = 1;
	//显示欢迎界面
	LoadMap(0);
	m_objMap.DrawMap();
	//延时2s
	Sleep(2000);
	
	while (m_nGameMode!=4)
	{
		//执行完毕后，游戏模式改变
		BeginUI();
		switch (m_nGameMode)
		{
		case 1:
		case 2:
			//单人双人公用一套代码
			GameUI();
			break;
		case 3:
			//地图编辑
			EditMap();
			break;
		}
	}
	
}

void CGameManager::SaveStatus()
{
	//保存坦克数据
	CTank::SaveTank();
	//保存地图数据
	SaveMap(4);
	//保存子弹数据和游戏关卡
	FILE *pFile = nullptr;
	fopen_s(&pFile, ".//resource//data//BulletData.bin", "wb+");
	if (pFile == nullptr)
	{
		exit(0);
	}
	//保存游戏关卡
	fwrite(&m_nGameLevel, sizeof(int), 1, pFile);
	//保存得分
	fwrite(&m_nScore1, sizeof(int), 1, pFile);
	fwrite(&m_nScore2, sizeof(int), 1, pFile);
	//循环保存当前所有的子弹数据
	for (unsigned int i=0; i<m_vecBullets.size();i++)
	{
		char *p = (char *)&m_vecBullets[i];
		fwrite(p, sizeof(CBullet),1, pFile);
	}
	fclose(pFile);
}

void CGameManager::LoadStatus()
{
	//加载坦克状态
	CTank::LoadTank();
	//加载地图
	LoadMap(4);
	//加载游戏关卡和子弹
	FILE *pFile = nullptr;
	fopen_s(&pFile, ".//resource//data//BulletData.bin", "rb");
	if (pFile == nullptr)
	{
		exit(0);
	}
	//获取游戏关卡
	fread(&m_nGameLevel, sizeof(int), 1, pFile);
	//获取得分
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
	WriteChar(45, 8, "地图编辑",0x0E);
	WriteChar(42, 10, "当前选择:", 0x0E);
	WriteChar(42, 12, "地形选择:", 0x0E);
	WriteChar(42, 13, "1.砖墙:■", 0x0C);
	WriteChar(42, 14, "2.铁墙:■", 0x0F);
	WriteChar(42, 15, "3.草地:※", 0x0A);
	WriteChar(42, 16, "4.河流:≈", 0x03);

	WriteChar(42, 18, "鼠标单击:", 0x0E);
	WriteChar(42, 19, "放置/移除", 0x0B);
}

void CGameManager::LoadMap(int nIndex)
{
	//5个地图文件名字
	const char FileName[5][40] = { 
		".//resource//data//welcome.bin" ,
		".//resource//data//Map1.bin"  ,
		".//resource//data//Map2.bin"  ,
		".//resource//data//Image.bin"  ,
		".//resource//data//SaveData.bin" };
	char szFileNameBuffer[40];
	memcpy_s(szFileNameBuffer, sizeof(szFileNameBuffer), FileName[nIndex], sizeof(FileName[nIndex]));
	//根据所选地图文件的名字加载相应的地图
	m_objMap.LoadMap(szFileNameBuffer);
}

void CGameManager::SaveMap(int nIndex)
{
	//5个地图文件名字
	const char FileName[5][40] = {
		".//resource//data//welcome.bin" ,
		".//resource//data//Map1.bin"  ,
		".//resource//data//Map2.bin"  ,
		".//resource//data//Image.bin"  ,
		".//resource//data//SaveData.bin" };
	char szFileNameBuffer[40];
	memcpy_s(szFileNameBuffer, sizeof(szFileNameBuffer), FileName[nIndex], sizeof(FileName[nIndex]));
	//根据所选地图文件的名字保存相应的地图
	m_objMap.SaveMap(szFileNameBuffer);
}

void CGameManager::SaveMapUI()
{
	//用于存储选择界面的字符串
	char szBuffer[2][20] = { "1.关卡地图1","2.关卡地图2"};
	//用于保存用户的选择
	int nIndex = 1;
	//用于保存选择的亮度条
	int nKeyTemp = 1;
	//判断是否有按键按下
	int nFlag = 1;
	//清空地图显示,在地图中心判断当前关卡选项
	m_objMap.ClearShowMap();
	//画出标题
	WriteChar(17, 13, "地图选择:", 0x0C);
	//2.循环检测按键，改变选项
	//3.J键确认
	while (true)
	{
		//如果有按键按下，根据当前的亮度条画出2个选项
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
		//获取用户输入
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
	//清屏
	system("cls");
	//初始化并画出基础地图
	m_objMap.InitMap();
	m_objMap.DrawMap();
	//画出提示栏
	EditMapStatusBar();
	//获取键盘按键，用于选择5个地形，获取鼠标点击事件，给相应的地图位置设置值
	int nType = WALL_NO;

	//关于鼠标响应时间的获取前置条件
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD stcRecord = { 0 };
	DWORD dwRead;
	SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	while (true)
	{
		char szNum[10];
		sprintf_s(szNum, "%d", nType);
		WriteChar(48, 10, szNum,0x0B);
		//等待事件
		ReadConsoleInput(hStdin, &stcRecord, 1, &dwRead);
		
		//如果事件产生
		//如果是按键事件根据按键选择模式
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
					//将当前地图转换到坦克地图中
					CTank::TransformMap(m_objMap);
					break;
				}	
			}
		}
		else if (stcRecord.EventType == MOUSE_EVENT)
		{		
			//如果是鼠标事件，赋值
			//先获取鼠标在屏幕上的位置
			short nX = stcRecord.Event.MouseEvent.dwMousePosition.X/2;
			short nY = stcRecord.Event.MouseEvent.dwMousePosition.Y;
			switch (stcRecord.Event.MouseEvent.dwEventFlags)
			{
			case 0:
				//左键单击
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
	//清空地图显示,在地图中心判断是否存档
	m_objMap.ClearShowMap();
	WriteChar(15, 20, "您想要保存地图么?", 0x0C);
	WriteChar(15, 22, "j.保存地图 k.取消保存",0x06);
	//根据玩家输入的按键判断是否进入地图保存界面
	while (true)
	{
		if (_kbhit())//是否按下键盘
		{
			char ch = _getch();
			if (ch == 'j' || ch == 'J')
			{
				//进入地图保存界面，选择存档的地图（关卡1，关卡2）
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
	//1.根据坦克初始化子弹对象
	CBullet TempBullet;
	TempBullet = objTank; 
	//2.小型的碰撞检测，如果不在障碍物上且不越界，则加入子弹数组,画出子弹,否则函数执行结束
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
	//根据传入子弹的位置位置判断当前子弹的状态，撞到任意东西，当前子弹存活状态都为空
	//撞到障碍，撞到坦克，撞到其他子弹，或者什么都没撞到
	//1.先判断是否撞到障碍物和坦克
	COORD pos = objBullet.GetPos();
	int nType = m_objMap.getMapType(pos);
	switch (nType)
	{
	//若是草地或者河流，都不检测坦克，只检测子弹
	case RIVER:
	case LAWN:
		break;
	case WALL_1:
	{	//若是砖墙，则直接自杀，墙也死掉，把地图改入坦克地图,画出销毁后的地图
		objBullet.SetIsAlive(false);
		m_objMap.RemoveRange(pos);
		CTank::TransformMap(m_objMap);
		m_objMap.DrawRange(pos);
		return;
		break;
	}
	case WALL_2:
	{
		//若是铁墙，子弹自杀,删除子弹,画出铁墙
		objBullet.SetIsAlive(false);
		m_objMap.DrawImage(pos, WALL_2);
		return;
		break;
	}
	case WALL_NO:
	{
		//若是空地，则检测坦克和子弹
		//2.遍历坦克数组，看是否撞到坦克
		for (int i = 0; i < 6; i++)
		{
			//如果当前坦克已经死亡，则不判断它
			if (!CTank::s_arrTank[i].GetTankIsAlive())
				continue;
			if (CTank::s_arrTank[i].TankBodyExist(pos))	//在敌方坦克身上
			{

				//子弹死亡，根据阵营判断是否扣血
				objBullet.SetIsAlive(false);
				if (!(objBullet == CTank::s_arrTank[i]))//不是同一阵营，扣血
				{
					//播放被击中音效
					PlayHit();
					//扣血
					CTank::s_arrTank[i].DecHp(objBullet.GetATK());
				}
				//如果当前坦克被干死了，就直接擦掉，同时根据子弹id来增加相应玩家的杀敌数，否则画出来
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

	//3.遍历子弹数组，看是否撞到子弹，只要撞到，子弹必死
	for (unsigned int i = 0; i < m_vecBullets.size(); i++)
	{
		//把自己排除
		if (&objBullet == &m_vecBullets[i])
			continue;
		//如果已经碰撞
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
		if (!TempObj.GetIsAlive())	//如果当前子弹已经死亡
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
	//先擦掉自己
	objTank.ClearTank();
	//如果为己方坦克，则说明过关了，恢复血量，重置坦克位置，画出来即可
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
		//速度型：速度为FAST_SPEED，血量为2，攻击力为1
		//力量型：速度为LOW_SPEED，血量为4，攻击力为4
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
		//四个点，只要有一个点能放，就放下去
		if (objTank.TankCollision({ 15, 3 }))	//如果第一个点没有障碍物
		{
			CTank TempTank({ 15,3 }, DOWN, ENEMY_TANK, nSpeed, clock(), nPower, nHp);
			TempTank.SetID(objTank.GetID());
			objTank = TempTank;
		}
		else if (objTank.TankCollision({ 25,3 }))//如果第二个点没有障碍物
		{
			CTank TempTank({ 25,3 }, DOWN, ENEMY_TANK, nSpeed, clock(), nPower, nHp);
			TempTank.SetID(objTank.GetID());
			objTank = TempTank;
		}
		else if (objTank.TankCollision({ 3, 20 }))//如果第三个点没有障碍物
		{
			CTank TempTank({ 3, 20 }, RIGHT, ENEMY_TANK, nSpeed, clock(), nPower, nHp);
			TempTank.SetID(objTank.GetID());
			objTank = TempTank;
		}
		else if (objTank.TankCollision({ 37, 20 }))//如果第四个点没有障碍物
		{
			CTank TempTank({ 37, 20 }, LEFT, ENEMY_TANK, nSpeed, clock(), nPower, nHp);
			TempTank.SetID(objTank.GetID());
			objTank = TempTank;
		}
	}
	//画出自己
	objTank.DrawTank();
	m_objMap.DrawReverAndLawn();
}

void CGameManager::MoveNPCTank(int nID, int nType)
{
	if (nType == 0)//随机移动
	{
		//随机出当前是移动还是开炮，移动概率得大于开炮
		bool nFire = (rand() % 10 < 2 ? true:false);
		//随机出方向
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
	//如果坦克死亡，则重置
	for (int i = 2; i < 6; i++)
	{
		if (!CTank::s_arrTank[i].GetTankIsAlive())
		{
			//重置坦克
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

