#pragma once
//用于进行图形绘制，提供头文件，修改控制台窗口
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <vector>
using std::vector;
//该链接库用于音乐播放
#pragma comment(lib,"winmm.lib")

//判断指定的按键是否按下
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)
//判断指定的按键是否弹起
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)

//宏定义地图上的元素
#define WALL_NO		0
#define WALL_1		1
#define WALL_2		2
#define LAWN		3
#define RIVER		4

//宏定义按键操作
#define UP			0
#define DOWN		3
#define LEFT		1
#define RIGHT		2
#define FIRE		4

//宏定义坦克类型
#define PLAYER_TANK		10
#define ENEMY_TANK		11

//宏定义响应速度
#define LOW_SPEED		600
#define NOMAL_SPEED		350
#define FAST_SPEED		200
#define GOD_SPEED		30


extern const char szTankSharp[2][4][3][7];

//	函数功能：设置控制台窗口信息
bool SetWindowSize(const char *pszWindowTitle, short nX, short nY);

//	函数功能：隐藏光标
void ShowCursor(bool isShow);

//画指定点的颜色，默认为白色
void WriteChar(short x, short y, const char* psz, WORD Color = 0x0F);

/*	函数功能：切换至英文输入法
*/
void ChangeEnglish();


/*	函数功能：播放背景音乐
*/
void PlayMusic();

/*函数功能：播放炮弹发射音效*/
void PlayOp();

/*函数功能：播放坦克死亡音效*/
void PlayDeath();

/*函数功能：播放坦克被击中音效*/
void PlayHit();