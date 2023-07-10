#include "Screen.h"
#include "effect.h"

void goalMessage(int x, int y) {
	ScreenPrint(x, y,   "woo hoo~! goal in~~");
	ScreenPrint(x, y+1, "(^^) *");
	ScreenPrint(x, y+2, " || //");
	ScreenPrint(x, y+3, "  L//");
}