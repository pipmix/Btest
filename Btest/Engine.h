#pragma once
#include "DataTypes.h"

struct RectI {
	int x;
	int y;
	int w;
	int z;
};

struct Img {
	int w;
	int h;

	int tileW;
	int tileH;

	int tilesX;
	int tilesY;

	char* data;
};

struct Spr {

	RectI sourceRect;
	Img* image;

};

class Engine {


public:

	Engine();



	void BlitSprite(int x, int y);

private:




};