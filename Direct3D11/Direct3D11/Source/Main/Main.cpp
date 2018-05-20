/*!
	@file	MAIN.cpp
	@date	2018/02/08
	@author	番場 宥輝
	@brief	アプリケーションメインエントリーポイント
*/
#include <tchar.h>
#include "Main.h"
#include "../Direct3D11/Direct3D11.h"
#include "../MyGame.h"
#include "../Sprite/Sprite.h"
#include "../MemoryLeaks.h"
#include "../Sound/SoundManager/SoundManager.h"
#include "../Sound/Wave/Wave.h"

/*!
	@def	定数
*/
const std::string Main::c_AppName	= "App";						/*!< アプリケーションの名前 */
const int	Main::c_FPS				= 60;							/*! ゲームのターゲットFPS */
const float Main::c_MinFrameTime	= 1.0f / Main::c_FPS;			/*!< 1フレーム辺りの経過時間 */

#include "../Camera/Camera.h"
Texture* g_pTex;
//Wave* wave;
Sprite* g_pSprite;

/*!
	@brief	コンストラクタ
*/
Main::Main() :m_pWindow(nullptr)
{
}

/*!
	@brief	デストラクタ
*/
Main::~Main()
{
	//delete(wave);
	SAFE_DELETE(m_pWindow);
	Release();
}

/*!
	@brief	初期化
*/
bool Main::Init(HINSTANCE hInstance)
{
	m_pWindow = new Window;

	/*! UNICODE、マルチバイト両対応用文字列変換 */
	auto tmp = tString(c_AppName);
	const auto appName = const_cast<LPTSTR>(tmp.c_str());

	if (!m_pWindow->Init(&m_hWnd, hInstance,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, appName)) {
		return false;
	}

	/*! デバイス初期化 */
	if (!Direct3D11::GetInstance().Init(m_hWnd))	{ return false; }		/*!< デバイス */
	if (FAILED(SoundManager::GetInstance().Init())) { return false; }		/*!< サウンド */

	return true;
}

/*!
	@brief	解放
*/
void Main::Release()
{
	//SAFE_DELETE(g_pSprite);
	SAFE_DELETE(m_pWindow);
	SoundManager::GetInstance().Release();
	Direct3D11::GetInstance().Release();
	SAFE_DELETE(g_pTex);
	SAFE_DELETE(g_pSprite);
}

/*!	
	@brief	アプリケーション処理の入り口
*/
void Main::Loop()
{
	/*! FPSの初期化 */
	SetUpFPS();

	/*! カメラの初期化 */
	Camera::Initialize({ 0,0,-1 });

	/*! シーンの初期化 */
	g_pTex = new Texture;
	//g_pTex->Load("image.jpeg", {308,163});
	//g_pTex->Load("yukina.jpg", { 648,960 });
	g_pTex->Load("Resource/Texture/魔王.png", { 96,160 });
	//g_pTex->Load("Input.png", { 2048,256 });
	//g_pTex->Load("sprite.jpg", { 256,256 });

	//wave = new Wave;
	//wave->Load("Resource/Sound/titleBGM.wav");
	////wave->Play();
	g_pSprite = new Sprite;
	HRESULT hr;

	g_pSprite->Initialize();
	g_pSprite->SetSplitTexture({3,5});
	//DirectX::XMFLOAT3 i = { 100,100,0 };
	//g_pSprite->SetRot(i);
	//hr=g_pSprite->InitShader("SimpleTexture.hlsl");
	//if (FAILED(hr)) { ErrorLog("aa"); }
	//g_pSprite->InitPolygon();
	//if (FAILED(hr)) { ErrorLog("bb"); }

	/*! メッセージループ */
	MSG msg{ 0 };
	ZeroMemory(&msg, sizeof(&msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			App();/*!< App処理 */
		}
	}
}

/*
	@brief	高解像度タイマーが対応しているか確認
*/
bool Main::HighQualityTimmer()
{
	/*! QueryPerformanceFrequencyの対応を見る */
	if (!QueryPerformanceFrequency(&m_TimeFreq)) {
		ErrorLog("This PC is not fit QueryPerformanceFrequency!\n"
			"Sorry this project is exit!");
		return false;
	}

	return true;
}

/*!
	@brief	アプリケーション処理
*/
void Main::App()
{
	/*! FPS確認 */
#ifdef _DEBUG
	static DWORD time = 0;
	static int frame = 0;
	frame++;
	char str[50];
	sprintf_s(str, "fps=%d", frame);
	if (timeGetTime() - time > 1000) {
		time = timeGetTime();
		frame = 0;

		auto tmp = tString(str);
		const auto fps = const_cast<LPTSTR>(tmp.c_str());
		if (!SetWindowText(m_hWnd, fps)) {
			WindowsErrorLog();
		}
	}
#endif
	/*! FPSの調整方法で分岐 */
#ifdef Vsync
	/*! 更新処理 */
	Update();
	/*! 描画処理 */
	Render();
#else
		/*! 更新処理 */
		Update();
		/*! 描画処理 */
		Render();
	/*! FPS調整 */
		SleepApp();
#endif // Vsync
}

/*!
	@brief	更新
*/
#include "../Input/Keyboard/Keyboard.h"
DirectX::XMFLOAT3 pos = { 0,0,1 };
int index = 0;
int indey = 0;
void Main::Update()
{
	Keyboard::Update();
	if (Keyboard::GetButton('S')) {
		pos.y -= 0.1f;
	}
	else if (Keyboard::GetButton('W')) {
		pos.y += 0.1f;
	}
	else if (Keyboard::GetButtonDown('G')) {
		index++;
		//index=index==0?1:0;
	}
	else if (Keyboard::GetButtonDown('H')) {
		indey++;
		//indey = indey == 0 ? 1 : 0;
	}

	g_pSprite->SetActivateIndex({ index%3-4,indey%5 });

	std::string str =
		"x=" + std::to_string(index%3) + "," + "y=" + std::to_string(indey%5);
		//std::to_string(pos.x) + std::to_string(pos.y) + std::to_string(pos.z);
	SetWindowTextA(m_hWnd, str.c_str());
	g_pSprite->SetPos(pos);

}

/*!
	@brief	描画
*/
void Main::Render()
{
	/*! 描画ターゲットのクリア */
	Direct3D11::GetInstance().Clear();

	/*! シーンの描画 */
//	g_pSprite->Render();
	g_pSprite->Render(g_pTex);

	/*! 画面更新 */
	Direct3D11::GetInstance().Present();
}

/*!
	@brief	FPS調整の準備
*/
void Main::SetUpFPS()
{
	m_FrameTime = 0;
	/*! 初回の計算 */
	QueryPerformanceCounter(&m_TimeStart);
}
/*!
	@brief	アプリケーション処理のお休み
*/
void Main::SleepApp()
{
	/*! 今の時間を取得 */
	QueryPerformanceCounter(&m_TimeEnd);
	/*! 経過時間 */
	m_FrameTime = static_cast<float>(m_TimeEnd.QuadPart - m_TimeStart.QuadPart) / static_cast<float>(m_TimeFreq.QuadPart);
	if (m_FrameTime < c_MinFrameTime)
	{
		DWORD SleepTime = static_cast<DWORD>((c_MinFrameTime - m_FrameTime) * 1000);/*!< ズレ分 */
		timeBeginPeriod(1);	/*!< 分解性能を上げる */
		Sleep(SleepTime);	/*!< お休み */
		timeEndPeriod(1);	/*!< 戻す */
	}
	QueryPerformanceCounter(&m_TimeStart);
}
