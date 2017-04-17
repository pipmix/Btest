#pragma once
#include "Engine.h"


class Game {




public:
	Game();

	void Update();
	void Draw();

private:

	int xScroll;
	int yScroll;



	int m_surfaceWidth;
	int m_surfaceHeight;
	int m_surfaceArea;

};