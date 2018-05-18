/*!
	@file	MAIN.h
	@date	2018/02/08
	@author	�ԏ� �G�P
	@brief	�A�v���P�[�V�������C���G���g���[�|�C���g
*/
#pragma once

#include <Windows.h>
#include <mmsystem.h>
#include <string>
#include "../Window/Window.h"

class Main
{
public:
	Main();
	~Main();

	bool Init(HINSTANCE hInstance);
	void Release();
	void Loop();
	bool HighQualityTimmer();
	void SetUpFPS();
private:
	void App();
	void Update();
	void Render();
	void SleepApp();

	static const std::string c_AppName;
	static const int	c_FPS;
	static const float	c_MinFrameTime;

	HWND m_hWnd;
	float m_FrameTime;
	float m_Fps;
	LARGE_INTEGER m_TimeStart;
	LARGE_INTEGER m_TimeEnd;
	LARGE_INTEGER m_TimeFreq;
	Window* m_pWindow;
};

