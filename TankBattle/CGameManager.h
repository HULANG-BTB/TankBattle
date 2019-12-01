#pragma once
#include "CBullet.h"
class CGameManager
{
public:
	CGameManager();
	~CGameManager();
	//游戏环境初始化(隐藏光标，更改标题名，切换输入法)
	void GameVarInit();
	
	//游戏运行准备初始化(区分单双人模式)
	void GameRunInit();
	//游戏运行主界面(返回值为当前游戏状态，0结束，1过关)
	int GameStart(bool bFlag  = false);
	//游戏运行状态栏(区分单双人) 
	void GameStautsBar();

	//游戏界面,能够实现关卡转换
	void GameUI();

	//开始界面(获取按键输入，进行游戏模式选择)(已实现)
	//退出后，设置当前游戏模式
	void BeginUI();


	//游戏管理主循环(管理开始界面，欢迎界面，游戏运行界面)(未实现)
	void Run();

	//存档
	void SaveStatus();
	//读档
	void LoadStatus();

	//编辑地图状态栏
	void EditMapStatusBar();
	//编辑地图，根据鼠标和键盘按键进行地图的编辑(已经实现)
	void EditMap();
	//加载地图(参数0为欢迎界面，参数3为游戏模式选择界面地图,1,2为游戏关卡，参数4为存档地图)(未实现)
	void LoadMap(int nIndex);
	//保存地图
	void SaveMap(int nIndex);
	//地图保存页面
	void SaveMapUI();

	//创建子弹(参数为开炮的坦克)
	void CreateBullet(CTank objTank);
	//子弹碰撞处理(参数为子弹的引用)
	void BulletCollision(CBullet &objButton);
	//子弹数组管理(清除无效子弹)
	void BulletManage();

	//坦克重置(根据传入坦克的参数决定重置哪一只，也可用于坦克初始化)
	void ResetTank(CTank& objTank);
	//敌方坦克移动(根据传入参数选择是用a*算法跟随还是用随机数奔跑,0为随机数，1为a*)
	void MoveNPCTank(int nID, int nType);
	//敌方坦克数组管理
	void EnemyTankManage();

	//获取用户1输入,返回值为上下左右开火
	int getPlayer1Input();
	//获取用户2输入,返回值为上下左右开火
	int getPlayer2Input();
private:
	//地图类对象，里面只保存障碍物信息
	CMap m_objMap;

	//地图上的子弹数组
	vector<CBullet> m_vecBullets;

	//玩家的分数
	int m_nScore1;
	int m_nScore2;

	//游戏关卡,通关后刷新
	int m_nGameLevel;
	//游戏模式选择、单人双人、编辑地图
	int m_nGameMode;
};

