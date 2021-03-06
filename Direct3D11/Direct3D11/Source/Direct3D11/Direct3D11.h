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
#include <wrl/client.h>

/*!
	@def	Vsync
	@brief	Vsyncを使ったFPS同期(コメントアウトするとアプリケーション側でのwait)
*/
//#define Vsync

/*!
	@def	定数宣言
*/
const INT WINDOW_WIDTH	= 1600;
const INT WINDOW_HEIGHT = 900;

/*!
	@brief	COMの解放
*/
template<class COM>
static inline void COM_RELEASE(COM* &com) {
	if (com != NULL) {
		com->Release();
		com = NULL;
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

	ID3D11Device* GetDevice() { return m_pDevice.Get(); }
	ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext.Get(); }

	
private:
	Direct3D11();
	void operator=(const Direct3D11& instance)	= delete;	/*!< 代入禁止 */
	Direct3D11(const Direct3D11& instance)		= delete;	/*!< コピー禁止 */

	static const float c_ClearColor[4];

	Microsoft::WRL::ComPtr<ID3D11Device>m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>m_pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>m_pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>m_pDepthStencil;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>m_pDepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11BlendState>m_pBlendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>m_pRasterizerState;
};

