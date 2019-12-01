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
	//�ӵ�����ƶ�
	void MoveBullet();
	//���ӵ�
	void DrawBullet();
	//�����ӵ�
	void ClearBullet();
	//���ӵ��Ƿ��ײ���Լ�
	bool BulletCrash(CBullet objBullet);


	//��ȡ�ӵ��ϴ��ƶ���ʱ��
	int GetClock();
	//�����ӵ��ϴ��ƶ���ʱ��
	void SetClock(int nClock);
	//��ȡ�ӵ�������
	COORD GetPos();
	//��ȡ�ӵ����״̬
	bool GetIsAlive();
	//�����ӵ����״̬
	void SetIsAlive(bool bState);
	//��ȡ�ӵ��ٶ�
	int GetSpeed();
	//��ȡ�ӵ�����
	int GetDir();
	//��ȡ�ӵ�������
	int GetATK();
	//��ȡ�ӵ�ID
	int GetID();

	//���صȺ�������������ӵ����̹����������ж�
	bool operator==(CTank objTank);
	//���ظ�ֵ�����������̹���ഴ���ӵ�
	bool operator=(CTank objTank);
private:
	COORD m_pos;
	//�ӵ��ķ���
	int m_nDir;
	//�ӵ��ϴ��ƶ���ʱ��
	int m_nClock;
	//�ӵ����ƶ��ٶ�
	int m_nSpeed;
	//�ӵ�����Ӫ
	int m_nCamp;
	//�ӵ��Ĺ�����
	int m_nATK;
	//�ӵ��Ĵ��״̬
	bool m_bAlive;
	//�ӵ���ID���̳��Է�������̹��
	int m_nID;
};

