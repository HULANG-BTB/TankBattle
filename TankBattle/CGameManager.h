#pragma once
#include "CBullet.h"
class CGameManager
{
public:
	CGameManager();
	~CGameManager();
	//��Ϸ������ʼ��(���ع�꣬���ı��������л����뷨)
	void GameVarInit();
	
	//��Ϸ����׼����ʼ��(���ֵ�˫��ģʽ)
	void GameRunInit();
	//��Ϸ����������(����ֵΪ��ǰ��Ϸ״̬��0������1����)
	int GameStart(bool bFlag  = false);
	//��Ϸ����״̬��(���ֵ�˫��) 
	void GameStautsBar();

	//��Ϸ����,�ܹ�ʵ�ֹؿ�ת��
	void GameUI();

	//��ʼ����(��ȡ�������룬������Ϸģʽѡ��)(��ʵ��)
	//�˳������õ�ǰ��Ϸģʽ
	void BeginUI();


	//��Ϸ������ѭ��(����ʼ���棬��ӭ���棬��Ϸ���н���)(δʵ��)
	void Run();

	//�浵
	void SaveStatus();
	//����
	void LoadStatus();

	//�༭��ͼ״̬��
	void EditMapStatusBar();
	//�༭��ͼ���������ͼ��̰������е�ͼ�ı༭(�Ѿ�ʵ��)
	void EditMap();
	//���ص�ͼ(����0Ϊ��ӭ���棬����3Ϊ��Ϸģʽѡ������ͼ,1,2Ϊ��Ϸ�ؿ�������4Ϊ�浵��ͼ)(δʵ��)
	void LoadMap(int nIndex);
	//�����ͼ
	void SaveMap(int nIndex);
	//��ͼ����ҳ��
	void SaveMapUI();

	//�����ӵ�(����Ϊ���ڵ�̹��)
	void CreateBullet(CTank objTank);
	//�ӵ���ײ����(����Ϊ�ӵ�������)
	void BulletCollision(CBullet &objButton);
	//�ӵ��������(�����Ч�ӵ�)
	void BulletManage();

	//̹������(���ݴ���̹�˵Ĳ�������������һֻ��Ҳ������̹�˳�ʼ��)
	void ResetTank(CTank& objTank);
	//�з�̹���ƶ�(���ݴ������ѡ������a*�㷨���滹�������������,0Ϊ�������1Ϊa*)
	void MoveNPCTank(int nID, int nType);
	//�з�̹���������
	void EnemyTankManage();

	//��ȡ�û�1����,����ֵΪ�������ҿ���
	int getPlayer1Input();
	//��ȡ�û�2����,����ֵΪ�������ҿ���
	int getPlayer2Input();
private:
	//��ͼ���������ֻ�����ϰ�����Ϣ
	CMap m_objMap;

	//��ͼ�ϵ��ӵ�����
	vector<CBullet> m_vecBullets;

	//��ҵķ���
	int m_nScore1;
	int m_nScore2;

	//��Ϸ�ؿ�,ͨ�غ�ˢ��
	int m_nGameLevel;
	//��Ϸģʽѡ�񡢵���˫�ˡ��༭��ͼ
	int m_nGameMode;
};

