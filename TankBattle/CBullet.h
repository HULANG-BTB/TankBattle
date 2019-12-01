#pragma once
#include "base.h"
#include "CTank.h"
class CBullet
{
public:
	CBullet();
	CBullet(COORD pos, int nDir, int nSpeed, int nClock) :m_pos(pos), m_nDir(nDir),
		m_nSpeed(nSpeed), m_nClock(nClock) {}
	~CBullet();
	//子弹类的移动
	void MoveBullet();
	//画子弹
	void DrawBullet();
	//擦除子弹
	void ClearBullet();
	//该子弹是否会撞上自己
	bool BulletCrash(CBullet objBullet);


	//获取子弹上次移动的时间
	int GetClock();
	//设置子弹上次移动的时间
	void SetClock(int nClock);
	//获取子弹的坐标
	COORD GetPos();
	//获取子弹存活状态
	bool GetIsAlive();
	//设置子弹存活状态
	void SetIsAlive(bool bState);
	//获取子弹速度
	int GetSpeed();
	//获取子弹方向
	int GetDir();
	//获取子弹攻击力
	int GetATK();
	//获取子弹ID
	int GetID();

	//重载等号运算符，用于子弹类和坦克类的类型判断
	bool operator==(CTank objTank);
	//重载赋值运算符，用于坦克类创建子弹
	bool operator=(CTank objTank);
private:
	COORD m_pos;
	//子弹的方向
	int m_nDir;
	//子弹上次移动的时间
	int m_nClock;
	//子弹的移动速度
	int m_nSpeed;
	//子弹的阵营
	int m_nCamp;
	//子弹的攻击力
	int m_nATK;
	//子弹的存活状态
	bool m_bAlive;
	//子弹的ID，继承自发出它的坦克
	int m_nID;
};

