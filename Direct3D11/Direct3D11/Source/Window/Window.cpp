#include "Window.h"
#include "../MemoryLeaks.h"

/*!
	@def	�v���g�^�C�v�錾
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/*!
	@var	g_pWindow
*/
Window*	g_pWindow = NULL;

/*!
	@brief	�E�B���h�E�v���V�[�W��
	@detail	�R�[���o�b�N�֐�
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return g_pWindow->MsgProc(hWnd, msg, wParam, lParam);
}

/*!
	@brief	�f�X�g���N�^
*/
Window::~Window()
{
}

/*!
	@brief	���b�Z�[�W�v���V�[�W��
	@detail	�����o�֐�
*/
LRESULT Window::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
			/*! Esc�L�[�������ꂽ�� */
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
		/*! �E�B���h�E���j�����ꂽ�Ƃ� */
	case WM_DESTROY:
		PostQuitMessage(0);	/*!< WM_QUIT���b�Z�[�W�����b�Z�[�W�L���[�ɑ��� */
		break;
	}
	/*! �f�t�H���g�̃��b�Z�[�W�������s�� */
	return DefWindowProc(hWnd, msg, wParam, lParam);
}



/*!
	@brief	������
*/
bool Window::Init(HWND* hWnd, HINSTANCE hInstance, INT iX, INT iY, INT iWidth, INT iHeight, LPCTSTR WindowName)
{
	g_pWindow = this;
	/*! �E�B���h�E�̒�` */
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
	/*! �E�B���h�E�̍쐬 */
	*hWnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW,
		0, 0, iWidth, iHeight, 0, 0, hInstance, 0);
	if (!hWnd)
	{
		return false;
	}
	/*! �E�B���h�E�̕\�� */
	ShowWindow(*hWnd, SW_SHOW);
	UpdateWindow(*hWnd);

	return true;
}