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
	g_sPlayer.nCenterX = 0;
	g_sPlayer.nPosX = 20;
	g_sPlayer.nPosY = 22;
	g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
}

void Update() {}

void Render() {
	
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
				g_sPlayer.nPosX--;//�̵�
				if (g_sPlayer.nPosX - g_sPlayer.nCenterX < 0)//����
					g_sPlayer.nPosX--;
				//Ư����ȣ�� 2����Ʈ�̹Ƿ� 1����Ʈ�� �߸� �� ����.

				g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
				break;
			case 'l':
				g_sPlayer.nPosX++;//�̵�
				nRemain = g_nLength - (g_sPlayer.nCenterX + 1);
				//�߽���ǥ�� ���� ĳ���� ũ��. ����������� Ȯ�ο�
				if (g_sPlayer.nPosX + nRemain > 79)//����
					g_sPlayer.nPosX++;//1����Ʈ �� �Ű���

				g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
				break;
			case 'k':
				break;
			}
		}
	}

	Release();
	ScreenRelease();
	return 0;
}