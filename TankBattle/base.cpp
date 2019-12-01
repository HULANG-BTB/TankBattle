#include "base.h"

//����̹�˵���״����
const char szTankSharp[2][4][3][7] = {
	{
		{"  ��  ","������","��  ��"},		//��
		{"  ����","����  ","  ����"},		//��
		{"����  ","  ����","����  "},		//��
		{"��  ��","������","  ��  "}		//��
	},
	{
		{"  ��  ","������","��  ��"},		//��
		{"  ����","����  ","  ����"},		//��
		{"����  ","  ����","����  "},		//��
		{"��  ��","������","  ��  "}		//��
	}
};
//const char szTankSharp[2][4][3][7] = {
//	{
//		{"�X���["
//		,"�d���g"
//		,"�^�m�a"},		//��
//		{"�X�j�["
//		,"�����g"
//		,"�^�m�a"},		//��
//		{"�X�j�["
//		,"�d����"
//		,"�^�m�a"},		//��
//		{"�X�j�["
//		,"�d���g"
//		,"�^���a"}		//��
//	},
//	{
//		{"������"
//		,"�ǡ��"
//		,"������"},		//��
//		{"���ר�"
//		,"����"
//		,"���ߨ�"},		//��
//		{"���ר�"
//		,"�ǡ�"
//		,"���ߨ�"},		//��
//		{"������"
//		,"�ǡ��"
//		,"������"}		//��
//	}
//};

void WriteChar(short x, short y, const char * psz, WORD Color)
{
	//��ȡ���
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//������ɫ
	SetConsoleTextAttribute(hOut, Color);
	//��������λ��
	COORD loc;
	loc.X = x * 2;
	loc.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),loc);
	//��ӡ�ַ���
	printf(psz);
	//��ͼ��
}

bool SetWindowSize(const char * pszWindowTitle, short nX, short nY)
{
	//���ÿ���̨����
	SetConsoleTitleA(pszWindowTitle);
	HANDLE hStdIn, hStdOut;
	hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//��ȡ������̨�Ĵ��ڴ�С
	COORD pos = GetLargestConsoleWindowSize(hStdOut);
	COORD BufferSize = { pos.X + 1,pos.Y + 1 };
	//���ÿ���̨��������С
	if (!SetConsoleScreenBufferSize(hStdOut, BufferSize))
	{	//����ʧ��
		printf("buffer err(%d,%d)%d\n", BufferSize.X, BufferSize.Y, GetLastError());
		return false;
	}
	SMALL_RECT srctWindow = { 0,0,nX,nY };
	if (!SetConsoleWindowInfo(hStdOut, true, &srctWindow))
	{	//���ÿ���̨���ڴ�С
		printf("size err %d\n", GetLastError());
		return false;
	}
	COORD Buffer = { nX + 1,nY + 1 };
	//���ÿ���̨��������С
	if (!SetConsoleScreenBufferSize(hStdOut, Buffer))
	{	//���ÿ���̨���ڻ�����ʧ��
		printf("buffer err(%d,%d) %d\n", BufferSize.X, BufferSize.Y, GetLastError());
		return false;
	}
	return true;
}

void ShowCursor(bool isShow)
{	//���ṹ���������ع��
	CONSOLE_CURSOR_INFO stcCursorInfo;
	//���ع��
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
