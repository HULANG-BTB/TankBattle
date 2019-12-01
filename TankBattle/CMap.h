#pragma once
#include "CMap.h"
#include "base.h"
/*已测试完毕，正常运行*/
class CMap
{
public:
	CMap();
	~CMap();
	//存入某一点的数据
	void setMapType(COORD pos, int nType);
	//读取某一点的数据
	int getMapType(COORD pos);
	//从文件加载地图
	void LoadMap(const char * szFileName);
	//保存地图到文件
	void SaveMap(const char * szFileName);
	//清除地图中的所有图案
	void ClearShowMap();

	//根据当前坐标点画地图
	void DrawMap();
	//根据类型画出当前坐标的图像，仅限障碍
	void DrawImage(COORD pos, int nType);
	//初始化地图，加上边框，也就是铁墙围一圈
	void InitMap();
	//画出草地和河流
	void DrawReverAndLawn();

	//用于子弹爆炸的函数
	//在某一点删除一圈障碍
	void RemoveRange(COORD pos);
	//画出9格的地图
	void DrawRange(COORD pos);
private:
	int m_nMap[40][40];
};

