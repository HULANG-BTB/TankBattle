#include "CGameManager.h"
int main()
{
	srand((unsigned int)time(NULL));
	
	CGameManager objGame;
	//��ʼ����Ϸ����
	objGame.GameVarInit();
	//��ʼ��Ϸ
	objGame.Run();
}