#include <stdio.h>
#include <time.h>
#include "Screen.h"

void Init() {}
void Update() {}
void Render() {
	ScreenClear();
	//����ڵ�

	ScreenFlipping();
}
void Release() {}


int main() {
	ScreenInit();
	Init();

	while (1) {
		Update();
		Render();
	}

	Release();
	ScreenRelease();
	return 0;
}