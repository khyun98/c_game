#include <stdio.h>

void Init();

void Update();

void Render();

void Release();

int main() {
	Init();

	while (1) {


		Update();
		Render();
	}

	Release();
	return 0;
}

void Init() {}

void Update() {}

void Render() {}

void Release() {}