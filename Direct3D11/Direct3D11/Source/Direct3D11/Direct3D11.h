/*
	@file	Direct3D11.h
	@date	2018/02/11
	@author	番場 宥輝
	@brief	Direct3D11デバイス
	@detail	シングルトン
*/
#pragma once
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"winmm.lib")

#include <D3DX11.h>
#include <string>

/*!
	@def	Vsync
	@brief	Vsyncを使ったFPS同期(コメントアウトするとアプリケーション側でのwait)
*/
//#define Vsync

/*!
	@def	定数宣言
*/
const INT WINDOW_WIDTH	= 1920;
const INT WINDOW_HEIGHT = 1080;

/*!
	@brief	COMの解放
*/
template<class COM>
static inline void COM_RELEASE(COM* &com) {
	if (com != nullptr) {
		com->Release();
		com = nullptr;
	}
}

/*!
	@brief	Direct3D11デバイスclass
*/
class Direct3D11
{
public:
	~Direct3D11();
	static Direct3D11& GetInstance() {
		static Direct3D11 instance;
		return instance;
	}

	bool Init(HWND hWnd);
	void Release();
	void Clear();
	void Present();

	static ID3D11Device* GetDevice() { return GetInstance().m_pDevice; }
	static ID3D11DeviceContext* GetDeviceContext() { return GetInstance().m_pDeviceContext; }
private:
	Direct3D11();

	static const float c_ClearColor[4];

	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pDeviceContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pRenderTargetView;
	ID3D11DepthStencilView*		m_pDepthStencilView;
	ID3D11Texture2D*			m_pDepthStencil;
	ID3D11DepthStencilState*	m_pDepthStencilState;
	ID3D11BlendState*			m_pBlendState;
	ID3D11RasterizerState*		m_pRasterizerState;

	
};

