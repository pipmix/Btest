#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Vfw.h>
#include <tchar.h>
#include "TempData.h"

#include "Game.h"

static TCHAR szWindowClass[] = _T("MainWindowClass");
static TCHAR szTitle[] = _T("MainWindowTitle");
static TCHAR szWindowClass2[] = _T("DirectXWindowClass");
static TCHAR szTitle2[] = _T("DirectXWindowTitle");

HINSTANCE hInst = nullptr;

HWND ghWnd;
HDC window_hdc;

INT64 cTime = 0.0f;

static BITMAPINFO *bitmap_header;

#define WIDTH 320
#define HEIGHT 240
#define SIZE WIDTH*HEIGHT
static int noise;
static int carry;
static int index;
static int seed = 0x12345;
static int pixel[SIZE];

int count1 = 0;

bool startFrame = 1;

unsigned int gScreenX = 320;
unsigned int gScreenY = 240;
unsigned int gScreenSize = 320 * 240;

static void *cached_buffer = 0;
static char bitmapbuffer[sizeof(BITMAPINFO) + 16];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	CoInitializeEx(NULL, COINIT_MULTITHREADED);



	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = GetSysColorBrush(COLOR_GRAYTEXT);//  (HBRUSH)(COLOR_GRAYTEXT);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	RegisterClassEx(&wcex);
	hInst = hInstance;


	RECT windowRect;
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = WIDTH;
	windowRect.bottom = HEIGHT;

	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, 0);


	//windowRect.right -= windowRect.left;
	//windowRect.bottom -= windowRect.top;



	ghWnd = CreateWindowEx(WS_EX_CLIENTEDGE, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right, windowRect.bottom, NULL, NULL, hInstance, NULL);


	ShowWindow(ghWnd, nCmdShow);




	for (int cc = 0; cc < sizeof(BITMAPINFOHEADER) + 16; cc++)
		bitmapbuffer[cc] = 0;
	bitmap_header = (BITMAPINFO *)&bitmapbuffer;
	bitmap_header->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmap_header->bmiHeader.biPlanes = 1;
	bitmap_header->bmiHeader.biBitCount = 32;
	bitmap_header->bmiHeader.biCompression = BI_BITFIELDS;
	bitmap_header->bmiHeader.biWidth = WIDTH;
	bitmap_header->bmiHeader.biHeight = -HEIGHT;	// note well
	((unsigned long *)bitmap_header->bmiColors)[0] = 0x00FF0000;
	((unsigned long *)bitmap_header->bmiColors)[1] = 0x0000FF00;
	((unsigned long *)bitmap_header->bmiColors)[2] = 0x000000FF;

	window_hdc = GetDC(ghWnd);









	Game game;




	UpdateWindow(ghWnd);

	int offset = 0;
	int color = 0;

	MSG msg = { 0 };
	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {

			INT64 loopStartTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&loopStartTime);

			if (startFrame) {

				game.Update();
				game.Draw();

				int count = 0;
				for (index = 0; index < SIZE; index++)
				{
					noise = seed;
					noise >>= 3;
					noise ^= seed;
					carry = noise & 1;
					noise >>= 1;
					seed >>= 1;
					seed |= (carry << 30);
					noise &= 0xFF;
					pixel[index] = (noise << 16) | (noise << 8) | noise;
				}


				int x = 75 + offset;
				int y = 120 + offset;
				int w = 200;
				int h = 60;

				int lineStart = (y * gScreenX) + x;
				for (int H = 0; H < h; H++) {
					if (lineStart > gScreenSize)break;
					for (int L = 0; L < w; L++) {
						if ((lineStart + L) % gScreenX == 0)break;
						pixel[lineStart + L] = color;
					}
					lineStart += gScreenX;
				}


				offset++;
				if (offset > 80)offset = 0;

				color++;
				if (color > 2500000000) color = 0;




				int spriteSize = sizeof(tDat);
				int sprDimX = 8;
				int sprDimY = 8;
				int placeX = 50;
				int placeY = 70;
				int placeStart = 0;
				placeStart = (placeX * placeY) + placeX;

				for (int i = 0; i < spriteSize; i++) {

					if (tDat[i] != 0)pixel[placeStart] = tDat2[i];
					//if (tDat[i] != 0)pixel[placeStart] = 255 << 0 << 255;

					//255 241 180 (0xff) | (0xf1) | 0xb4;
					placeStart++;
					if ((i % sprDimX) == 0) placeStart = (placeStart - sprDimX) + WIDTH;


					// Get next sprite pixel


				}

				placeX = 150;
				placeY = 170;
				placeStart = (placeX * placeY) + placeX;

				for (int i = 0; i < spriteSize; i++) {

					//if (tDat[i] != 0)pixel[placeStart] = tDat[i];
					if (tDat[i] != 0)pixel[placeStart] = (0 << 24) +  (255 <<  16) + (255 << 8 ) + 0;



					placeStart++;
					if ((i % sprDimX) == 0) placeStart = (placeStart - sprDimX) + WIDTH;


					// Get next sprite pixel


				}

				count1++;
				if (count1 > 250)count1 = 0;



				cached_buffer = pixel;

				SendMessage(ghWnd, WM_PAINT, 0, 0);
				startFrame = 0;
				cTime = 0;

			}

			INT64 loopEndTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&loopEndTime);


			INT64 timeDifference = loopEndTime - loopStartTime;

			cTime += timeDifference;

			float framesNeeded = (1000.0f / 60.0f);
			framesNeeded *= 1000;
			if (cTime > framesNeeded)startFrame = 1; // hold framesNeeded - timeDifference



		}
	}





	return (int)msg.wParam;



}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {

	case WM_PAINT: {

			RECT clientRect;
			GetClientRect(hWnd, &clientRect);
			StretchDIBits(window_hdc, 0, 0, clientRect.right, clientRect.bottom, 0, 0, WIDTH, HEIGHT, cached_buffer, bitmap_header, DIB_RGB_COLORS, SRCCOPY);

			//StretchDIBits(window_hdc, 0, 0, surface_width, surface_height, 0, 0, surface_width, surface_height, cached_buffer, bitmap_header, DIB_RGB_COLORS, SRCCOPY);

			ValidateRect(ghWnd, NULL);
		


	}
				   break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

