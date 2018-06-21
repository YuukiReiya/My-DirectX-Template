/*
	@file	Direct3D11.h
	@date	2018/02/11
	@author	番場 宥輝
	@brief	Direct3D11デバイス
	@detail	シングルトン
*/
#include "Direct3D11.h"
#include "../MemoryLeaks.h"
#include "../MyGame.h"

/*!
	@def	MSAA
	@brief	マルチサンプル・アンチエイリアス(コメントアウトするとMSAA無し)
*/
//#define MSAA

/*!
	@def	定数
*/
const float Direct3D11::c_ClearColor[4]{ 0.5f,0.5f,1.5f,1.0f };		/*!< 描画ターゲットクリアカラー */

/*!
	@brief	コンストラクタ
*/
Direct3D11::Direct3D11()
{
	ZeroMemory(this, sizeof(Direct3D11));
	m_pDevice=nullptr;
	m_pDeviceContext = nullptr;
	m_pSwapChain = nullptr;
	m_pRenderTargetView = nullptr;
	m_pDepthStencilView = nullptr;
	m_pDepthStencil = nullptr;
	m_pDepthStencilState = nullptr;
	//m_pBlendState = nullptr;
	m_pRasterizerState = nullptr;
}

/*!
	@brief	デストラクタ
*/
Direct3D11::~Direct3D11()
{
	Release();
}

/*!
	@brief	初期化
*/
bool Direct3D11::Init(HWND hWnd)
{
	/*! デバイスとスワップ・チェイン作成 */
	DXGI_SWAP_CHAIN_DESC sd;
	SecureZeroMemory(&sd, sizeof(sd));
	sd.BufferCount							= 1;							/*!< バック・バッファ数 */
	sd.BufferDesc.Width						= WINDOW_WIDTH;					/*!< バック・バッファの幅 */
	sd.BufferDesc.Height					= WINDOW_HEIGHT;				/*!< バック・バッファの高さ */
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;	/*!< フォーマット */
	sd.BufferDesc.RefreshRate.Numerator		= 60;							/*!< リフレッシュ・レート(分子) */
	sd.BufferDesc.RefreshRate.Denominator	= 1;								/*!< リフレッシュ・レート(分母) */
	/*! スキャンライン描画モード */
	sd.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	/*! スケーリング・モード */
	sd.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;						/*!< バック・バッファの仕様 */
	sd.OutputWindow							= hWnd;													/*!< 関連付けるウィンドウ */

#if defined(MSAA)
	/*! アンチエイリアス処理有り */

	//http://nayuta-solution.hateblo.jp/entry/2015/01/17/142751 こちらを参考に設定を行う	※途中

	/*! 使用できるMSAAをチェック */
	DXGI_SAMPLE_DESC sampleDesc{};

	for (int i = 1; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i <<= 1) {
		UINT quality;
		if (SUCCEEDED(m_pDevice->CheckMultisampleQualityLevels(
			DXGI_FORMAT_D24_UNORM_S8_UINT, i, &quality))) {
			if (0 < quality) {
				sampleDesc.Count = i;
				sampleDesc.Quality = quality - 1;
			}
		}
	}

				
#else
	/*! アンチエイリアス処理無し */
	sd.SampleDesc.Count = 1;							/*!< マルチ・サンプルの数 */
	sd.SampleDesc.Quality = 0;							/*!< マルチ・サンプルのクオリティ */
#endif // アンチエイリアス
	sd.Windowed = true;									/*!< ウィンドウ・モード */
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	/*!< スワップ・チェイン効果指定 */

	/*! 機能レベルを指定する配列 */
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	D3D_FEATURE_LEVEL* pFeatureLevelsSupported = NULL;	/*!< 機能レベルを取得する変数 */

#if defined(DEBUG)||defined(_DEBUG)
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
//	UINT createDeviceFlags = 0;
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif//DEBUG|_DEBUG

	/*! デバイスとスワップ・チェインの作成 */
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,							/*!< 使用するIDXGIAdapter インターフェース */
		D3D_DRIVER_TYPE_HARDWARE,		/*!< Direct3D 11 デバイスの種類 */
		NULL,							
		createDeviceFlags,				/*!< 使用するAPI レイヤーの指定 */
		&featureLevels[0],				/*!< 機能レベルを指定する配列 */
		GetArraySize(featureLevels),	/*!< 上記配列の要素数 */
		D3D11_SDK_VERSION,				/*!< 使用しているSDKのバージョン */
		&sd,							/*!< スワップ・チェインの設定 */
		m_pSwapChain.GetAddressOf(),	/*!< IDXGISwapChain インターフェースの変数 */
		m_pDevice.GetAddressOf(),		/*!< ID3D11Device インターフェースの変数 */
		pFeatureLevelsSupported,		/*!< 機能レベルを取得する変数 */
		m_pDeviceContext.GetAddressOf()	/*!< ID3D11DeviceContext インターフェースの変数 */
	);
	if (FAILED(hr)) { 
		ErrorLog("SwapChain is not create!");
		return false;/*!< デバイスの作成に失敗 */
	}

	/*! スワップ・チェインから最初のバックバッファを取得する */
	ID3D11Texture2D* pBackBuffer;
	hr = m_pSwapChain->GetBuffer(
		0,								/*!< バック・バッファ番号 */
		__uuidof(ID3D11Texture2D),		/*!< バッファにアクセスするインターフェース */
		(LPVOID*)&pBackBuffer			/*!< バッファを受け取る変数 */
	);
	if (FAILED(hr)) { 
		ErrorLog("Can't take top back buffer!");
		return false; /*!< 取得失敗 */
	}
	
	/*! レンダーターゲットビューの作成 */
	hr = m_pDevice->CreateRenderTargetView(
		pBackBuffer,						/*!< ビューでアクセスするリソース */
		NULL,								/*!< レンダーターゲットビューの定義 */
		m_pRenderTargetView.GetAddressOf()	/*!< レンダーターゲットビューを受け取る変数 */
	);
	COM_RELEASE(pBackBuffer);/*!< バック・バッファは以降使わないので解放 */
	if (FAILED(hr)) { 
		ErrorLog("RenderTargetView is not create!");
		return false;/*!< レンダーターゲットビューの作成に失敗 */
	}

	/*! 深度 / ステンシル・テクスチャの設定 */
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width					= WINDOW_WIDTH;					/*!< 幅 */
	descDepth.Height				= WINDOW_HEIGHT;				/*!< 高さ */
	descDepth.MipLevels				= 1;							/*!< ミップマップ・レベル数 */
	descDepth.ArraySize				= 1;							/*!< 配列サイズ */
	descDepth.Format				= DXGI_FORMAT_D32_FLOAT;		/*!< フォーマット(深度のみ) */
#if defined(MSAA)
	descDepth.SampleDesc = sampleDesc;								/*!< MSAAの設定と同じもの */
#else
	descDepth.SampleDesc.Count		= 1;							/*!< マルチサンプリングの設定 */
	descDepth.SampleDesc.Quality	= 0;							/*!< マルチサンプリングの品質 */
#endif// アンチエイリアス
	descDepth.Usage					= D3D11_USAGE_DEFAULT;			/*!< デフォルトの仕様 */
	descDepth.BindFlags				= D3D11_BIND_DEPTH_STENCIL;		/*!< 深度 / ステンシルとして使用 */
	descDepth.CPUAccessFlags		= 0;							/*!< CPUからはアクセスしない */
	descDepth.MiscFlags				= 0;							/*!< その他 設定なし */

	/*! 深度 / ステンシル・テクスチャの作成 */
	hr = m_pDevice->CreateTexture2D(
		&descDepth,						/*!< 作成する2Dテクスチャの設定 */
		NULL,							/*!< MSAAを利用する場合 NULL にする */
		m_pDepthStencil.GetAddressOf()	/*!< 作成したテクスチャを受けとる変数 */
	);
	if (FAILED(hr)) {
		ErrorLog("StencilTexture is not create!");
		return false;/*!< ステンシルテクスチャの作成に失敗 */
	}

	/*! 深度 / ステンシル・ビュー設定 */
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format				= descDepth.Format;					/*!< ビューのフォーマット */
	descDSV.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;	/*!< リソースの種類 */
	descDSV.Flags				= 0;								/*!< テクスチャのアクセス権 0:RW(読み込み書き込み)  */
	descDSV.Texture2D.MipSlice	= 0;								

	/*! 深度 / ステンシル・テクスチャに対しビューを作成 */
	hr = m_pDevice->CreateDepthStencilView(
		m_pDepthStencil.Get(),			/*!< 深度 / ステンシル・ビューを作るテクスチャ */
		&descDSV,					/*!< 深度 / ステンシル・ビューの設定 */
		m_pDepthStencilView.GetAddressOf()		/*!< 作成したビューを受け取る変数 */
	);
	if (FAILED(hr)) {
		ErrorLog("StencilView is not create!");
		return false;
	}/*!< 深度 / ステンシル・ビュー作成に失敗 */

	/*! 描画ターゲット・ビューを出力マネージャーのターゲットとして設定 */
	m_pDeviceContext->OMSetRenderTargets(
		1,							/*!< 描画ターゲット数 */
		m_pRenderTargetView.GetAddressOf(),		/*!< 描画ターゲットビュー */
		m_pDepthStencilView.Get()			/*!< 深度 / ステンシルビュー */
	);

	/*! 深度 /ステンシル・ステート設定 */
	D3D11_DEPTH_STENCIL_DESC dc;
	SecureZeroMemory(&dc, sizeof(dc));
	dc.DepthEnable				= true;									/*!< 深度テスト有り */
	dc.DepthWriteMask			= D3D11_DEPTH_WRITE_MASK_ALL;			/*!< 書き込む */
	dc.DepthFunc				= D3D11_COMPARISON_LESS;				/*!< 手前の物体を描画 */
	dc.StencilEnable			= false;								/*!< ステンシル・テスト無し */
	dc.StencilReadMask			= D3D11_DEFAULT_STENCIL_READ_MASK;		/*!< ステンシル書き込みマスク */
	dc.StencilWriteMask			= D3D11_DEFAULT_STENCIL_WRITE_MASK;		/*!< ステンシル読み込みマスク */

	/*! 面が表を向いている場合のステンシル・テストの設定 */
	dc.FrontFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;		/*!< 維持 */
	dc.FrontFace.StencilDepthFailOp		= D3D11_STENCIL_OP_KEEP;		/*!< 維持 */
	dc.FrontFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;		/*!< 維持 */
	dc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;		/*!< 常時 */
	/*! 面が裏を向いている場合のステンシル・テストの設定 */
	dc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;		/*!< 維持 */
	dc.BackFace.StencilDepthFailOp		= D3D11_STENCIL_OP_KEEP;		/*!< 維持 */
	dc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;		/*!< 維持 */
	dc.BackFace.StencilFunc				= D3D11_COMPARISON_ALWAYS;		/*!< 常時 */

	/*! 深度 /ステンシル・ステート作成 */
	hr = m_pDevice->CreateDepthStencilState(
		&dc,								/*!< 深度 /ステンシル・ステート設定情報 */
		m_pDepthStencilState.GetAddressOf()	/*!< 作成した 深度 /ステンシル・ステートを受け取る変数 */
	);
	if (FAILED(hr)) { 
		ErrorLog("StencilState is not create!");
		return false; /*!< 深度 /ステンシル・ステート作成失敗 */
	}

	/*! 深度 /ステンシル・ステート適応 */
	m_pDeviceContext->OMSetDepthStencilState(
		m_pDepthStencilState.Get(),		/*!< 深度 /ステンシル・ステート */
		0							/*!< ステンシル・テストで参照値 */
	);

	/*! ビューポートの設定 */
	D3D11_VIEWPORT vp;
	vp.Width	= WINDOW_WIDTH;		/*!< ビューポート領域の幅 */
	vp.Height	= WINDOW_HEIGHT;	/*!< ビューポート領域の高さ */
	vp.MinDepth = 0.0f;				/*!< ビューポート領域の深度最小値(ニア・クリッピング距離) */
	vp.MaxDepth = 1.0f;				/*!< ビューポート領域の深度最大値(ファー・クリッピング距離) */
	vp.TopLeftX = 0;				/*!< ビューポート領域の左上x座標 */
	vp.TopLeftY = 0;				/*!< ビューポート領域の左上y座標 */
	m_pDeviceContext->RSSetViewports(
		1,			/*!< ビューポートの数 */
		&vp			/*!< 設定するビューポート配列 */
	);

	/*! ラスタライズ設定 */
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode					= D3D11_FILL_SOLID;	/*!< 普通に描画 */
	//rd.FillMode					= D3D11_FILL_WIREFRAME;	/*!< ワイヤーフレーム描画 */
	rd.CullMode					= D3D11_CULL_NONE;	/*!< 両面を描画 */
	rd.FrontCounterClockwise	= false;			/*!< 深度バイアス「0」 */
	rd.DepthBias				= 0;
	rd.DepthBiasClamp			= 0;
	rd.SlopeScaledDepthBias		= 0;
	rd.DepthClipEnable			= false;			/*!< 深度クリッピング無し */
	rd.ScissorEnable			= false;			/*!< シザー矩形無し */
#if defined(MSAA)
	rd.MultisampleEnable		= true;
#else
	rd.MultisampleEnable		= false;			/*!< マルチサンプリング無し */
#endif
	rd.AntialiasedLineEnable	= false;			/*!< ライン・アンチエイリアシング無し */

	/*! ラスタライズ設定の作成 */
	hr = m_pDevice->CreateRasterizerState(
		&rd,					/*!< ラスタライズ設定 */
		m_pRasterizerState.GetAddressOf()		/*!< 設定を受け取る変数 */
	);
	if (FAILED(hr)) { 
		ErrorLog("Rasterizer is not create!");
		return false;/*!< ラスタライザーステート作成失敗 */
	}
	
	/*! ラスタライズを設定 */
	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());

	/*! αブレンド用ブレンドステート作成 */
	D3D11_BLEND_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
#if defined(MSAA)
//	bd.AlphaToCoverageEnable = true;		/*!< MSAAで描画ターゲットにおいてピクセル値を設定するときα値を使う */
#else
	bd.AlphaToCoverageEnable = false;
#endif
	//bd.IndependentBlendEnable					= false;						/*!< false:RenderTarget[0]の設定のみ反映。[1]～[7]は無視 */
	//bd.RenderTarget[0].BlendEnable				= true;							/*!< ブレンディングを有効 */
	//bd.RenderTarget[0].SrcBlend					= D3D11_BLEND_SRC_ALPHA;		/*!< ソースのRGBを選択 */
	//bd.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;	/*!< ディスティネーションのRGB選択 */
	//bd.RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;			/*!< RGBのブレンド設定 */
	//bd.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;				/*!< ソースのα値を選択 */
	//bd.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;				/*!< ディスティネーションのα値を選択 */
	//bd.RenderTarget[0].BlendOpAlpha				= D3D11_BLEND_OP_ADD;			/*!< α値のブレンド設定 */
	//bd.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;	/*!< RGBAのうち書き込む値を指定する */

	///*! ブレンドステート作成 */
	//hr = m_pDevice->CreateBlendState(
	//	&bd,			/*!< ブレンドステート設定 */
	//	m_pBlendState.GetAddressOf()	/*!< 設定を受け取る変数 */
	//);
	//if (FAILED(hr)) { 
	//	ErrorLog("BlendState is not create!");
	//	return false;
	//}/*!< ブレンドステート作成失敗 */
	//
	///*! ブレンドステート設定用変数 */
	//float blendFactor[4]{ 0.0f,0.0f, 0.0f, 0.0f };
	//UINT mask=0xffffffff;

	///*! OMにブレンドステートオブジェクト設定 */
	//m_pDeviceContext->OMSetBlendState(
	//	m_pBlendState.Get(),		/*!< 設定するオブジェクト */
	//	blendFactor,		/*!< 定数値 */
	//	mask				/*!< サンプル用マスク */
	//);

	/*! 初期化終了 */
	return true;
}

/*!
	@brief	解放
*/
void Direct3D11::Release()
{
	//COM_RELEASE(m_pRasterizerState);
	//COM_RELEASE(m_pSwapChain);
	//COM_RELEASE(m_pRenderTargetView);
	//COM_RELEASE(m_pDepthStencil);
	//COM_RELEASE(m_pDepthStencilView);
	//COM_RELEASE(m_pDepthStencilState);
	//COM_RELEASE(m_pBlendState);
	//COM_RELEASE(m_pDevice);
	//COM_RELEASE(m_pDeviceContext);
}

/*!
	@brief	描画ターゲットのクリア
*/
void Direct3D11::Clear()
{
	/*! レンダーターゲットビューのクリア */
	m_pDeviceContext->ClearRenderTargetView(
		m_pRenderTargetView.Get(),	/*!< クリアするレンダーターゲットビュー */
		c_ClearColor			/*!< クリアカラー */
	);

	/*! デプス・ステンシルビューのクリア */
	m_pDeviceContext->ClearDepthStencilView(
		m_pDepthStencilView.Get(),	/*!< クリアするデプス・ステンシルビュー */
		D3D11_CLEAR_DEPTH,		/*!< クリアするデータの型 */
		1.0f,					/*!< 深度バッファのクリア値 */
		0						/*!< ステンシルバッファのクリア値 */
	);
}

/*!
	@brief	画面更新
	@detail	(バックバッファをフロントバッファに)
*/
void Direct3D11::Present()
{
	m_pSwapChain->Present(
#ifdef Vsync
		1,	/*!< 画面更新タイミング(垂直回帰の同期設定) */
#else
		0,
#endif
		0	/*!< 画面更新(DXGI_PRESENT_TESTだと更新は行わない) */
	);
}
