#pragma once

struct Point {
	int x;
	int y;
};

struct Line {
	Point p1;
	Point p2;
};

struct Sprite {
	int x;
	int y;
	int width;
	int height;
	unsigned int* data;
};

struct Rect {
	int x;
	int y;
	int width;
	int height;
};

struct GameObject {

	int x;
	int y;

	Rect collisionRect;
	Rect sourceRect;

	bool flipX;
	bool flipY;
	bool isVisible;
	Sprite* sprite;


};