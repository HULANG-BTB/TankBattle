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
	//��̬��������������Ϸ�����潫��Ϸ��ͼת��Ϊ̹�˵�ͼ
	static void TransformMap(CMap objGameMap);
	//��̬������������̹�˵�ͼ��ĳһ�㻭��һȦ�ϰ�
	static void IncreaseRange(COORD pos);
	//��̬���������ڴ��ļ�װ��̹�˵����ݺ͵�ͼ
	static void LoadTank();
	//��̬���������ڱ��浱ǰ��̹�����ݺ͵�ͼ
	static void SaveTank();

	//��̹��
	void DrawTank();
	//����̹��
	void ClearTank();
	//̹���ƶ�
	void MoveTank(int nDir);
	//̹����ײ���
	bool TankCollision(COORD pos);

	//�������Ƿ���̹������
	bool TankBodyExist(COORD pos);

	//����̹��Ѫ��
	void AddHp(int nNum);
	//����̹��Ѫ��
	void DecHp(int nNum);
	//��ȡ̹��Ѫ��
	int GetHp();
	//��ȡ̹�˴��״̬
	bool GetTankIsAlive();


	/*���ڻ�ȡ��������̹�˵ĳ�Ա����*/

	//��ȡ̹�����һ����Ӧʱ��
	int GetClock();
	//����̹�����һ����Ӧʱ��
	void SetClock(int nClock);
	//��ȡ̹������
	int GetType();
	//����̹�����ͣ�����̹�˳�ʼ��
	void SetType(int nType);
	//��ȡ̹���ٶ�
	int GetSpeed();
	//��ȡ̹�˷���
	int GetTankDir();
	//��ȡ̹������
	COORD GetPos();
	//����̹������
	void SetPos(COORD pos);
	//����̹��Ѫ��
	void SetHp(int nHp);
	//��ȡ̹�˹�����
	int GetPower();
	//��ȡ̹��ID
	int GetID();
	//����̹��ID
	void SetID(int nID);

	//̹��ɱ��������
	void AddKillNumber();
	//��ȡ̹��ɱ����
	int GetKillNumber();

	//ȫ�ֵ�һ����ͼ
	static CMap s_objTankMap;
	//ȫ�ֵ�һ��̹�����飬ǰ2���±������
	static CTank s_arrTank[6];
private:
	//̹������
	COORD m_pos;
	//̹�˵ķ���
	int m_nDir;
	//̹�˵�����(���̹��PLAYER_TANK������̹��ENEMY_TANK)
	int m_nType;
	//̹�˵����һ����Ӧʱ��
	int m_nClock;
	//̹�˵��ƶ��ٶ�
	int m_nSpeed;
	//̹�˵Ĺ�����
	int m_nPower;
	//̹�˵�Ѫ��
	int m_nHp;
	//̹�˵�ID
	int m_nID;
	//̹�˵�ɱ����
	int m_nKill;
};
