#pragma once
#include "DataTypes.h"

extern unsigned int gScreenX;
extern unsigned int gScreenY;
extern int* gScreen;

void DrawSprite(Sprite* spr){

}


void DrawImage(Sprite* spr, int x, int y) {

	gScreen[ (gScreenX * y) + x] = 5;


	for (int y = 0; y < spr->y; y++) {

	}
	//for (int y = 0; y<sprite.height; y++)


}

void LoadImage() {


}

void DrawBox(int x, int y, int w, int h) {


	int lineStart = (y * gScreenX) + x;
	for (int H = 0; H < h; H++){
		for (int L = 0; L < w; L++) {
			gScreen[lineStart + L] = 1;
		}
		lineStart += gScreenX;
	}

}