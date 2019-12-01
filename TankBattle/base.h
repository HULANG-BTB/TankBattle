#pragma once
//���ڽ���ͼ�λ��ƣ��ṩͷ�ļ����޸Ŀ���̨����
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <vector>
using std::vector;
//�����ӿ��������ֲ���
#pragma comment(lib,"winmm.lib")

//�ж�ָ���İ����Ƿ���
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)
//�ж�ָ���İ����Ƿ���
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)

//�궨���ͼ�ϵ�Ԫ��
#define WALL_NO		0
#define WALL_1		1
#define WALL_2		2
#define LAWN		3
#define RIVER		4

//�궨�尴������
#define UP			0
#define DOWN		3
#define LEFT		1
#define RIGHT		2
#define FIRE		4

//�궨��̹������
#define PLAYER_TANK		10
#define ENEMY_TANK		11

//�궨����Ӧ�ٶ�
#define LOW_SPEED		600
#define NOMAL_SPEED		350
#define FAST_SPEED		200
#define GOD_SPEED		30


extern const char szTankSharp[2][4][3][7];

//	�������ܣ����ÿ���̨������Ϣ
bool SetWindowSize(const char *pszWindowTitle, short nX, short nY);

//	�������ܣ����ع��
void ShowCursor(bool isShow);

//��ָ�������ɫ��Ĭ��Ϊ��ɫ
void WriteChar(short x, short y, const char* psz, WORD Color = 0x0F);

/*	�������ܣ��л���Ӣ�����뷨
*/
void ChangeEnglish();


/*	�������ܣ����ű�������
*/
void PlayMusic();

/*�������ܣ������ڵ�������Ч*/
void PlayOp();

/*�������ܣ�����̹��������Ч*/
void PlayDeath();

/*�������ܣ�����̹�˱�������Ч*/
void PlayHit();