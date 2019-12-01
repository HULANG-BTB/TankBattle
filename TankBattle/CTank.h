#pragma once
#include "CMap.h"

class CTank
{
public:
	CTank();
	CTank(COORD pos, int nDir, int nType, int nSpeed, int nClock,int nPower,int nHp,int nKill = 0) :
		m_pos(pos), m_nDir(nDir),m_nType(nType),
		m_nSpeed(nSpeed), m_nClock(nClock),m_nPower(nPower),m_nHp(nHp), m_nKill(nKill){}
	~CTank();
	//静态方法，用于在游戏类里面将游戏地图转换为坦克地图
	static void TransformMap(CMap objGameMap);
	//静态方法，用于在坦克地图上某一点画出一圈障碍
	static void IncreaseRange(COORD pos);
	//静态方法，用于从文件装入坦克的数据和地图
	static void LoadTank();
	//静态方法，用于保存当前的坦克数据和地图
	static void SaveTank();

	//画坦克
	void DrawTank();
	//擦除坦克
	void ClearTank();
	//坦克移动
	void MoveTank(int nDir);
	//坦克碰撞检测
	bool TankCollision(COORD pos);

	//该坐标是否在坦克身上
	bool TankBodyExist(COORD pos);

	//增加坦克血量
	void AddHp(int nNum);
	//减少坦克血量
	void DecHp(int nNum);
	//获取坦克血量
	int GetHp();
	//获取坦克存活状态
	bool GetTankIsAlive();


	/*用于获取或者设置坦克的成员变量*/

	//获取坦克最后一次响应时间
	int GetClock();
	//设置坦克最后一次响应时间
	void SetClock(int nClock);
	//获取坦克类型
	int GetType();
	//设置坦克类型，用于坦克初始化
	void SetType(int nType);
	//获取坦克速度
	int GetSpeed();
	//获取坦克方向
	int GetTankDir();
	//获取坦克坐标
	COORD GetPos();
	//设置坦克坐标
	void SetPos(COORD pos);
	//设置坦克血量
	void SetHp(int nHp);
	//获取坦克攻击力
	int GetPower();
	//获取坦克ID
	int GetID();
	//设置坦克ID
	void SetID(int nID);

	//坦克杀敌数增加
	void AddKillNumber();
	//获取坦克杀敌数
	int GetKillNumber();

	//全局的一个地图
	static CMap s_objTankMap;
	//全局的一个坦克数组，前2个下标是玩家
	static CTank s_arrTank[6];
private:
	//坦克坐标
	COORD m_pos;
	//坦克的方向
	int m_nDir;
	//坦克的类型(玩家坦克PLAYER_TANK，敌人坦克ENEMY_TANK)
	int m_nType;
	//坦克的最后一次响应时间
	int m_nClock;
	//坦克的移动速度
	int m_nSpeed;
	//坦克的攻击力
	int m_nPower;
	//坦克的血量
	int m_nHp;
	//坦克的ID
	int m_nID;
	//坦克的杀敌数
	int m_nKill;
};
