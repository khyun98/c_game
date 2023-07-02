#include <stdio.h>
#include <time.h>
#include "Screen.h"

void Init() {}
void Update() {}
void Render() {
	ScreenClear();


	ScreenFlipping();
}
void Release() {}

clock_t curTime;

int main() {
	ScreenInit();
	Init();

	clock_t oldTime=clock();
	while (1) {
		Update();
		Render();
		curTime = clock();
		while (curTime-oldTime<33) {//대기코드
			curTime = clock();
		}
		oldTime = curTime;
	}

	Release();
	ScreenRelease();
	return 0;
}