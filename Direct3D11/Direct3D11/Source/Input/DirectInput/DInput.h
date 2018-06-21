/*!
	@file	DInput.h
	@date	2018/02/09
	@author	番場 宥輝
	@brief	DirectInputを用いたキーボード入力を定義
	@detail	※コントローラーはXInputを用いる
*/
#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

class DInput
{
public:
	DInput();
	~DInput();

	bool Init(HWND hWnd, HINSTANCE hInstance);
private:
	HWND					m_hWnd;
	HINSTANCE				m_hInstance;
	LPDIRECTINPUT8			m_pDInput;
	LPDIRECTINPUTDEVICE8	m_pDIDevice;
};

