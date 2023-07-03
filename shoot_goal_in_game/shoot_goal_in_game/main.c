#include <stdio.h>
#include <conio.h>
#include "solid.h"
#include <string.h>

PLAYER g_sPlayer;
char g_strPlayer[] = "曲收收≒收收旭";
int g_nLength;

void Init() {
	g_nLength = strlen(g_strPlayer);
	g_sPlayer.nCenterX = (g_nLength+1)/2;
	g_sPlayer.nCenterX = 0;
	g_sPlayer.nMoveX = 20;
	g_sPlayer.nMoveY = 22;
	g_sPlayer.nX = g_sPlayer.nMoveX - g_sPlayer.nCenterX;
}

int main() {



	return 0;
}