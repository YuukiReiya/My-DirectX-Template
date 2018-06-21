/*
	@file	Direct3D11.h
	@date	2018/02/11
	@author	�ԏ� �G�P
	@brief	Direct3D11�f�o�C�X
	@detail	�V���O���g��
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
	@brief	Vsync���g����FPS����(�R�����g�A�E�g����ƃA�v���P�[�V�������ł�wait)
*/
//#define Vsync

/*!
	@def	�萔�錾
*/
const INT WINDOW_WIDTH	= 1600;
const INT WINDOW_HEIGHT = 900;

/*!
	@brief	COM�̉��
*/
template<class COM>
static inline void COM_RELEASE(COM* &com) {
	if (com != NULL) {
		com->Release();
		com = NULL;
	}
}

/*!
	@brief	Direct3D11�f�o�C�Xclass
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
	void operator=(const Direct3D11& instance)	= delete;	/*!< ����֎~ */
	Direct3D11(const Direct3D11& instance)		= delete;	/*!< �R�s�[�֎~ */

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

