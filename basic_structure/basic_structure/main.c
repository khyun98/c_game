#include <stdio.h>
#include <time.h>
#include "Screen.h"

void Init() {}
void Update() {}
void Render() {
	ScreenClear();
	//출력코드

	ScreenFlipping();
}
void Release() {}


int main() {
	ScreenInit();
	Init();

	clock_t oldTime=clock();
	while (1) {
		Update();
		Render();
	}

	Release();
	ScreenRelease();
	return 0;
}