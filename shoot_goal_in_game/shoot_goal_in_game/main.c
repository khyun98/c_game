#include <stdio.h>
#include <conio.h>
#include "solid.h"
#include <string.h>
#include "Screen.h"

PLAYER g_sPlayer;
char g_strPlayer[] = "�����ܦ���";
int g_nLength;

void Init() {
	g_nLength = strlen(g_strPlayer);
	g_sPlayer.nCenterX = (g_nLength+1)/2;
	g_sPlayer.nCenterY = 0;
	g_sPlayer.nPosX = 20;
	g_sPlayer.nPosY = 22;
	g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
}

void Update() {}

void Render() {
	char tempBuffer[100] = { 0, };
	ScreenClear();
	

	if (g_sPlayer.nX < 0) {
		ScreenPrint(0, g_sPlayer.nPosY, &g_strPlayer[-1 * g_sPlayer.nX]);
	}
	else if (g_sPlayer.nX + g_nLength > 79) {
		strncpy_s(tempBuffer, sizeof(tempBuffer), g_strPlayer, g_nLength - (g_sPlayer.nX + g_nLength - 79));
		ScreenPrint(g_sPlayer.nX, g_sPlayer.nPosY, tempBuffer);
	}
	else if(g_sPlayer.nX>=0&&g_sPlayer.nX<=79){
		ScreenPrint(g_sPlayer.nX,g_sPlayer.nPosY,g_strPlayer);
	}
	memset(tempBuffer, 0, sizeof(tempBuffer));
	sprintf_s(tempBuffer, sizeof(tempBuffer),"player position x:%d y:%d", g_sPlayer.nPosX, g_sPlayer.nPosY);
	ScreenPrint(0, 0, tempBuffer);
	

	ScreenFlipping();
}

void Release() {}

int main() {
	int nKey, nRemain;

	ScreenInit();
	Init();

	while (1) {
		if (_kbhit()) {//Ű����
			nKey = _getch();
			if (nKey == 'q')
				break;
			switch (nKey) {
			case 'j':
				if (g_sPlayer.nPosX <= 1)
					break;
				g_sPlayer.nPosX--;//�̵�
				nRemain = g_nLength - (g_sPlayer.nCenterX + 1);
				if (g_sPlayer.nPosX + nRemain > 79 || g_sPlayer.nPosX - g_sPlayer.nCenterX < 0)//����
					g_sPlayer.nPosX--;
				//Ư����ȣ�� 2����Ʈ�̹Ƿ� 1����Ʈ�� �߸� �� ����.

				g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
				break;
			case 'l':
				if (g_sPlayer.nPosX >= 77)
					break;
				g_sPlayer.nPosX++;//�̵�
				nRemain = g_nLength - (g_sPlayer.nCenterX + 1);
				//�߽���ǥ�� ���� ĳ���� ũ��. ����������� Ȯ�ο�
				if (g_sPlayer.nPosX + nRemain > 79|| g_sPlayer.nPosX - g_sPlayer.nCenterX < 0)//����
					g_sPlayer.nPosX++;//1����Ʈ �� �Ű���

				g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
				break;
			case 'k':
				break;
			}
		}
		Update();
		Render();
	}


	Release();
	ScreenRelease();
	return 0;
}