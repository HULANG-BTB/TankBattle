#include "CGameManager.h"
int main()
{
	srand((unsigned int)time(NULL));
	
	CGameManager objGame;
	//初始化游戏环境
	objGame.GameVarInit();
	//开始游戏
	objGame.Run();
}