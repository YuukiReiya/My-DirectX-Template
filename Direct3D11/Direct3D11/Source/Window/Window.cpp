#include "Window.h"
#include "../MemoryLeaks.h"

/*!
	@def	プロトタイプ宣言
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/*!
	@var	g_pWindow
*/
Window*	g_pWindow = NULL;

/*!
	@brief	ウィンドウプロシージャ
	@detail	コールバック関数
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return g_pWindow->MsgProc(hWnd, msg, wParam, lParam);
}

/*!
	@brief	デストラクタ
*/
Window::~Window()
{
}

/*!
	@brief	メッセージプロシージャ
	@detail	メンバ関数
*/
LRESULT Window::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
			/*! Escキーを押されたら */
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
		/*! ウィンドウが破棄されたとき */
	case WM_DESTROY:
		PostQuitMessage(0);	/*!< WM_QUITメッセージをメッセージキューに送る */
		break;
	}
	/*! デフォルトのメッセージ処理を行う */
	return DefWindowProc(hWnd, msg, wParam, lParam);
}



/*!
	@brief	初期化
*/
bool Window::Init(HWND* hWnd, HINSTANCE hInstance, INT iX, INT iY, INT iWidth, INT iHeight, LPCTSTR WindowName)
{
	g_pWindow = this;
	/*! ウィンドウの定義 */
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = WindowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);
	/*! ウィンドウの作成 */
	*hWnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW,
		0, 0, iWidth, iHeight, 0, 0, hInstance, 0);
	if (!hWnd)
	{
		return false;
	}
	/*! ウィンドウの表示 */
	ShowWindow(*hWnd, SW_SHOW);
	UpdateWindow(*hWnd);

	return true;
}