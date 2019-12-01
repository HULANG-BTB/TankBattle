#include "base.h"

//定义坦克的形状数组
const char szTankSharp[2][4][3][7] = {
	{
		{"  ■  ","■■■","■  ■"},		//上
		{"  ■■","■■  ","  ■■"},		//左
		{"■■  ","  ■■","■■  "},		//右
		{"■  ■","■■■","  ■  "}		//下
	},
	{
		{"  □  ","□□□","□  □"},		//上
		{"  □□","□□  ","  □□"},		//左
		{"□□  ","  □□","□□  "},		//右
		{"□  □","□□□","  □  "}		//下
	}
};
//const char szTankSharp[2][4][3][7] = {
//	{
//		{"╔┃╗"
//		,"╠█╣"
//		,"╚╩╝"},		//上
//		{"╔╦╗"
//		,"━█╣"
//		,"╚╩╝"},		//左
//		{"╔╦╗"
//		,"╠█━"
//		,"╚╩╝"},		//右
//		{"╔╦╗"
//		,"╠█╣"
//		,"╚┃╝"}		//下
//	},
//	{
//		{"◢┃◣"
//		,"┣●┫"
//		,"◥━◤"},		//上
//		{"◢┳◣"
//		,"━●┃"
//		,"◥┻◤"},		//左
//		{"◢┳◣"
//		,"┣●━"
//		,"◥┻◤"},		//右
//		{"◢━◣"
//		,"┣●┫"
//		,"◥┃◤"}		//下
//	}
//};

void WriteChar(short x, short y, const char * psz, WORD Color)
{
	//获取句柄
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//设置颜色
	SetConsoleTextAttribute(hOut, Color);
	//设置坐标位置
	COORD loc;
	loc.X = x * 2;
	loc.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),loc);
	//打印字符串
	printf(psz);
	//画图形
}

bool SetWindowSize(const char * pszWindowTitle, short nX, short nY)
{
	//设置控制台标题
	SetConsoleTitleA(pszWindowTitle);
	HANDLE hStdIn, hStdOut;
	hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//获取最大控制台的窗口大小
	COORD pos = GetLargestConsoleWindowSize(hStdOut);
	COORD BufferSize = { pos.X + 1,pos.Y + 1 };
	//设置控制台缓冲区大小
	if (!SetConsoleScreenBufferSize(hStdOut, BufferSize))
	{	//设置失败
		printf("buffer err(%d,%d)%d\n", BufferSize.X, BufferSize.Y, GetLastError());
		return false;
	}
	SMALL_RECT srctWindow = { 0,0,nX,nY };
	if (!SetConsoleWindowInfo(hStdOut, true, &srctWindow))
	{	//设置控制台窗口大小
		printf("size err %d\n", GetLastError());
		return false;
	}
	COORD Buffer = { nX + 1,nY + 1 };
	//设置控制台缓冲区大小
	if (!SetConsoleScreenBufferSize(hStdOut, Buffer))
	{	//设置控制台窗口缓冲区失败
		printf("buffer err(%d,%d) %d\n", BufferSize.X, BufferSize.Y, GetLastError());
		return false;
	}
	return true;
}

void ShowCursor(bool isShow)
{	//光标结构，用于隐藏光标
	CONSOLE_CURSOR_INFO stcCursorInfo;
	//隐藏光标
	stcCursorInfo.bVisible = isShow;
	stcCursorInfo.dwSize = sizeof(stcCursorInfo);
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &stcCursorInfo);
}

void ChangeEnglish()
{
	keybd_event(VK_SHIFT, 0, 0, 0);
	Sleep(100);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
}

void PlayMusic()
{
	char temp[255];
	mciSendString(("open resource\\sound\\start.mp3 alias MUSIC"), temp, 255, 0);
	mciSendString(("open resource\\sound\\Ak.wav alias MUSIC1"), temp, 255, 0);
	mciSendString(("open resource\\sound\\Death.wav alias MUSIC2"), temp, 255, 0);
	mciSendString(("open resource\\sound\\Hit.wav alias MUSIC3"), temp, 255, 0);
	mciSendString("play MUSIC repeat", NULL, 0, 0);
}

void PlayOp()
{
	mciSendString("seek MUSIC1 to start", NULL, 0, 0);
	mciSendString("play MUSIC1", NULL, 0, 0);
}

void PlayDeath()
{
	mciSendString("seek MUSIC2 to start", NULL, 0, 0);
	mciSendString("play MUSIC2", NULL, 0, 0);
}

void PlayHit()
{
	mciSendString("seek MUSIC3 to start", NULL, 0, 0);
	mciSendString("play MUSIC3", NULL, 0, 0);
}
