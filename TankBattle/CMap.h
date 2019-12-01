#pragma once
#include "CMap.h"
#include "base.h"
/*�Ѳ�����ϣ���������*/
class CMap
{
public:
	CMap();
	~CMap();
	//����ĳһ�������
	void setMapType(COORD pos, int nType);
	//��ȡĳһ�������
	int getMapType(COORD pos);
	//���ļ����ص�ͼ
	void LoadMap(const char * szFileName);
	//�����ͼ���ļ�
	void SaveMap(const char * szFileName);
	//�����ͼ�е�����ͼ��
	void ClearShowMap();

	//���ݵ�ǰ����㻭��ͼ
	void DrawMap();
	//�������ͻ�����ǰ�����ͼ�񣬽����ϰ�
	void DrawImage(COORD pos, int nType);
	//��ʼ����ͼ�����ϱ߿�Ҳ������ǽΧһȦ
	void InitMap();
	//�����ݵغͺ���
	void DrawReverAndLawn();

	//�����ӵ���ը�ĺ���
	//��ĳһ��ɾ��һȦ�ϰ�
	void RemoveRange(COORD pos);
	//����9��ĵ�ͼ
	void DrawRange(COORD pos);
private:
	int m_nMap[40][40];
};

