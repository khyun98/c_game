#include <stdio.h>
#include <conio.h>
#include "solid.h"
#include <string.h>
#include "Screen.h"

int g_nScore=0;

PLAYER g_sPlayer;
char g_strPlayer[] = "�����ܦ���";
int g_nLength;

BALL g_sBall;
char g_strBall[] = "0";

GOALDAE g_sGoalDae;

void Init() {
	g_nLength = strlen(g_strPlayer);
	g_sPlayer.nCenterX = (g_nLength+1)/2;
	g_sPlayer.nCenterY = 0;
	g_sPlayer.nPosX = 20;
	g_sPlayer.nPosY = 22;
	g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;

	g_sBall.nIsReady = 1;
	g_sBall.nPosX = g_sPlayer.nPosX;
	g_sBall.nPosY = g_sPlayer.nPosY-1;
	g_sBall.MoveTime = 50;

	g_sGoalDae.nPosX = 20;
	g_sGoalDae.nPosY = 4;
	g_sGoalDae.nLength = 1;
	g_sGoalDae.moveTime = 100;
	g_sGoalDae.nMovement = 1;
	int nGoalDaeLength = g_sGoalDae.nLength * 2 + 1;
	for (int i = 0; i < nGoalDaeLength; i++) {
		g_sGoalDae.nLineX[i] = g_sGoalDae.nPosX + 2 * (i + 1);
		//���ϱ� 2�� ��� ǥ�� ���ڰ� 2����Ʈ�� ���� Ư�������̱� ����
	}

}

void Update() {//player�� ������� ���������� ���� ��ü������ ������Ʈ�� ����
	clock_t curTime = clock();//�ð����� ���� ����ȭ ���

	//��� ������ ������Ʈ
	int nGoalDaeLength = g_sGoalDae.nLength * 2 + 1;
	if (curTime - g_sGoalDae.oldTime > g_sGoalDae.moveTime) {
		if (g_sGoalDae.nPosX <= 0|| g_sGoalDae.nPosX + 2 + g_sGoalDae.nLength * 2 + 1 + 2 >= 79) {
			g_sGoalDae.nMovement = g_sGoalDae.nMovement * -1;
		}

		g_sGoalDae.nPosX = g_sGoalDae.nPosX + g_sGoalDae.nMovement;
		for (int i = 0; i < nGoalDaeLength; i++) {
			g_sGoalDae.nLineX[i]= g_sGoalDae.nPosX + 2 * (i + 1);
		}
		g_sGoalDae.oldTime = curTime;
	}

	//�� ������ ������Ʈ
	if (g_sBall.nIsReady == 1) {
		g_sBall.nPosX = g_sPlayer.nPosX;
		g_sBall.nPosY = g_sPlayer.nPosY-1;
	}
	else {
		if (curTime - g_sBall.OldTime > g_sBall.MoveTime) {//�ð����� ���� ����ȭ ���
			if (g_sBall.nPosY-1>0) {//��� �浹 ����
				g_sBall.nPosY--;
				g_sBall.OldTime = curTime;// 'k'Ű ������ oldTime �ð� �����.
			}
			else {
				g_sBall.nIsReady = 1;
				g_sBall.nPosX = g_sPlayer.nPosX;
				g_sBall.nPosY = g_sPlayer.nPosY - 1;
			}
		}

		//���� ��� �浹 ����
		if (g_sBall.nPosY == g_sGoalDae.nPosY) {
			if (g_sGoalDae.nPosX <= g_sBall.nPosX && g_sGoalDae.nLineX[nGoalDaeLength - 1] + 2 + 2 >= g_sBall.nPosX) {
				g_sBall.nIsReady = 1;
				g_sBall.nPosX = g_sPlayer.nPosX;
				g_sBall.nPosY = g_sPlayer.nPosY - 1;
				if (g_sGoalDae.nPosX+2 <= g_sBall.nPosX && g_sGoalDae.nLineX[nGoalDaeLength - 1] + 2 +2 -2 >= g_sBall.nPosX) {
					g_nScore++;
					//���� ����
				}
			}
		}
	}
}

void Render() {
	char tempBuffer[100] = { 0, };
	ScreenClear();
	
	//��� ����
	int nGoalDaeLen=g_sGoalDae.nLength*2+1;
	ScreenPrint(g_sGoalDae.nPosX, g_sGoalDae.nPosY, "��");
	for (int i = 0; i < nGoalDaeLen; i++)
		ScreenPrint(g_sGoalDae.nLineX[i], g_sGoalDae.nPosY, "��");
	ScreenPrint(g_sGoalDae.nLineX[nGoalDaeLen - 1]/*����� ������ ��ǥ*/ + 2, g_sGoalDae.nPosY, "��");


	//�÷��̾� ��ġ�� ����
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

	//�� ����
	ScreenPrint(g_sBall.nPosX, g_sBall.nPosY, g_strBall);
	
	//�÷��̾� ��ġ ���� ���
	memset(tempBuffer, 0, sizeof(tempBuffer));
	sprintf_s(tempBuffer, sizeof(tempBuffer),"player position x:%d y:%d", g_sPlayer.nPosX, g_sPlayer.nPosY);
	ScreenPrint(0, 0, tempBuffer);
	
	//�� ��ġ ���� ���
	memset(tempBuffer, 0, sizeof(tempBuffer));
	sprintf_s(tempBuffer, sizeof(tempBuffer), "ball position x:%d y:%d", g_sBall.nPosX, g_sBall.nPosY);
	ScreenPrint(0, 50, tempBuffer);

	//��� ��ġ ���� ���
	memset(tempBuffer, 0, sizeof(tempBuffer));
	sprintf_s(tempBuffer, sizeof(tempBuffer), "ball position x:%d y:%d\n", g_sGoalDae.nPosX + 2 + g_sGoalDae.nLength * 2 + 1 + 2,g_sGoalDae.nPosY);
	ScreenPrint(0, 150, tempBuffer);

	//���ھ� ��ġ ���� ���
	memset(tempBuffer, 0, sizeof(tempBuffer));
	sprintf_s(tempBuffer, sizeof(tempBuffer), "goal score x:%d\n", g_nScore);
	ScreenPrint(0, 200, tempBuffer);

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
					break;//���� ��� �浹

				g_sPlayer.nPosX--;//�̵�
				nRemain = g_nLength - (g_sPlayer.nCenterX + 1);
				if (g_sPlayer.nPosX + nRemain > 79 || g_sPlayer.nPosX - g_sPlayer.nCenterX < 0)//����
					g_sPlayer.nPosX--;
				//Ư����ȣ�� 2����Ʈ�̹Ƿ� 1����Ʈ�� �߸� �� ����.

				g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
				break;
			case 'l':
				if (g_sPlayer.nPosX >= 77)
					break;//������ ��� �浹

				g_sPlayer.nPosX++;//�̵�
				nRemain = g_nLength - (g_sPlayer.nCenterX + 1);
				//�߽���ǥ�� ���� ĳ���� ũ��. ����������� Ȯ�ο�
				if (g_sPlayer.nPosX + nRemain > 79|| g_sPlayer.nPosX - g_sPlayer.nCenterX < 0)//����
					g_sPlayer.nPosX++;//1����Ʈ �� �Ű���

				g_sPlayer.nX = g_sPlayer.nPosX - g_sPlayer.nCenterX;
				break;
			case 'k'://�� ����
				if (g_sBall.nIsReady==1) {
					//g_sBall.nPosX = g_sPlayer.nPosX;
					//g_sBall.nPosY = g_sPlayer.nPosY - 1;
					g_sBall.OldTime = clock();
					g_sBall.nIsReady = 0;
				}
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