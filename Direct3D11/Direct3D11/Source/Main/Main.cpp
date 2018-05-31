/*!
	@file	MAIN.cpp
	@date	2018/02/08
	@author	�ԏ� �G�P
	@brief	�A�v���P�[�V�������C���G���g���[�|�C���g
*/
#include <tchar.h>
#include "Main.h"
#include "../Direct3D11/Direct3D11.h"
#include "../MyGame.h"
#include "../Sprite/Sprite.h"
#include "../MemoryLeaks.h"
#include "../Shader/ShaderManager.h"
#include "../Sound/SoundManager/SoundManager.h"
#include "../Sound/Wave/Wave.h"

/*!
	@def	�萔
*/
const std::string Main::c_AppName	= "App";						/*!< �A�v���P�[�V�����̖��O */
const int	Main::c_FPS				= 60;							/*! �Q�[���̃^�[�Q�b�gFPS */
const float Main::c_MinFrameTime	= 1.0f / Main::c_FPS;			/*!< 1�t���[���ӂ�̌o�ߎ��� */

#include "../Camera/Camera.h"
Texture* g_pTex;
Sprite* g_pSprite;

/*!
	@brief	�R���X�g���N�^
*/
Main::Main() :m_pWindow(nullptr)
{
}

/*!
	@brief	�f�X�g���N�^
*/
Main::~Main()
{
	//delete(wave);
	Release();
	//SAFE_DELETE(m_pWindow);
}

/*!
	@brief	������
*/

bool Main::Init(HINSTANCE hInstance)
{
	m_pWindow = new Window;

	/*! UNICODE�A�}���`�o�C�g���Ή��p������ϊ� */
	auto tmp = tString(c_AppName);
	const auto appName = const_cast<LPTSTR>(tmp.c_str());

	if (!m_pWindow->Init(&m_hWnd, hInstance,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, appName)) {
		return false;
	}

	/*! �f�o�C�X������ */
	auto device = &Direct3D11::GetInstance();
	auto sound = &SoundManager::GetInstance();
	auto shader = &ShaderManager::GetInstance();
	if (!device->Init(m_hWnd)) { return false; }		/*!< �f�o�C�X */
	if (FAILED(sound->Init())) { return false; }		/*!< �T�E���h */
	if (FAILED(shader->Initialize())) { return false; }

	return true;
}

/*!
	@brief	���
*/
void Main::Release()
{
	SAFE_DELETE(g_pTex);
	SAFE_DELETE(g_pSprite);
	SAFE_DELETE(m_pWindow);
	SoundManager::GetInstance().Release();
	ShaderManager::GetInstance().Finalize();
	Direct3D11::GetInstance().Release();
}

/*!	
	@brief	�A�v���P�[�V���������̓����
*/
#include "../Input/Keyboard/Keyboard.h"
void Main::Loop()
{
	/*! FPS�̏����� */
	SetUpFPS();

	/*! �J�����̏����� */
	Camera::GetInstance().Initialize({ 0,0,-1 });

	/*! �V�[���̏����� */
	g_pTex = new Texture;
	//g_pTex->Load("Resource/Texture/sprite.jpg", {308,163});
	g_pTex->Load("Resource/Texture/yukina.jpg", { 648,960 });
	//g_pTex->Load("Resource/Texture/����.png", { 96,160 });


	//wave = new Wave;
	//wave->Load("Resource/Sound/titleBGM.wav");
	////wave->Play();
	HRESULT hr;

	g_pSprite = new Sprite;
	hr = g_pSprite->Initialize();
	if (FAILED(hr)) { ErrorLog(""); }
	g_pSprite->SetPos({ 0,0,1 });

	//auto a = Camera::GetInstance();
	//std::string e = "x=" + std::to_string(a.GetEyePt().x) + ",y=" + std::to_string(a.GetEyePt().y) + ",z=" + std::to_string(a.GetEyePt().z);

	//ErrorLog(e);

	//g_pSprite->SetSplitTexture({3,5});
	//DirectX::XMFLOAT3 i = { 100,100,0 };
	//g_pSprite->SetRot(i);
	//hr=g_pSprite->InitShader("SimpleTexture.hlsl");
	//if (FAILED(hr)) { ErrorLog("aa"); }
	//g_pSprite->InitPolygon();
	//if (FAILED(hr)) { ErrorLog("bb"); }

	/*! ���b�Z�[�W���[�v */
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
			App();/*!< App���� */
		}
	}

}

/*
	@brief	���𑜓x�^�C�}�[���Ή����Ă��邩�m�F
*/
bool Main::HighQualityTimmer()
{
	/*! QueryPerformanceFrequency�̑Ή������� */
	if (!QueryPerformanceFrequency(&m_TimeFreq)) {
		ErrorLog("This PC is not fit QueryPerformanceFrequency!\n"
			"Sorry this project is exit!");
		return false;
	}

	return true;
}

/*!
	@brief	�A�v���P�[�V��������
*/
void Main::App()
{
	/*! FPS�m�F */
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
	/*! FPS�̒������@�ŕ��� */
#ifdef Vsync
	/*! �X�V���� */
	Update();
	/*! �`�揈�� */
	Render();
#else
		/*! �X�V���� */
		Update();
		/*! �`�揈�� */
		Render();
	/*! FPS���� */
		SleepApp();
#endif // Vsync
}

/*!
	@brief	�X�V
*/
float x = 0;
void Main::Update()
{
	Keyboard::Update();

	if ((Keyboard::GetButton(Keyboard::UP))) {
		x += 0.001f;
		g_pSprite->SetPos({ x,0,g_pSprite->GetPos().z });
	}

	if (Keyboard::GetButton(Keyboard::DOWN)) {

		g_pSprite->SetPos({ x,0,g_pSprite->GetPos().z });
	}

	if (Keyboard::GetButton(Keyboard::LEFT)) {
		x -= 0.001f;
		g_pSprite->SetPos({ x,0,g_pSprite->GetPos().z });
	}

	if (Keyboard::GetButton(Keyboard::RIGHT)) {
		x += 0.001f;
		g_pSprite->SetPos({ x,0,g_pSprite->GetPos().z });

	}

	//static float cx = 0;
	//if (Keyboard::GetButton('A')) {
	//	cx -= 0.1f;
	//	Camera::GetInstance().Initialize({ x,0,-1 });
	//}

	//if (Keyboard::GetButton('D')) {
	//	cx += 0.1f;
	//	Camera::GetInstance().Initialize({ x,0,-1 });
	//}
	//g_pSprite->SetPos(pos);

}

/*!
	@brief	�`��
*/
void Main::Render()
{
	/*! �`��^�[�Q�b�g�̃N���A */
	Direct3D11::GetInstance().Clear();

	/*! �V�[���̕`�� */
	g_pSprite->Render(g_pTex);

	/*! ��ʍX�V */
	Direct3D11::GetInstance().Present();
}

/*!
	@brief	FPS�����̏���
*/
void Main::SetUpFPS()
{
	m_FrameTime = 0;
	/*! ����̌v�Z */
	QueryPerformanceCounter(&m_TimeStart);
}
/*!
	@brief	�A�v���P�[�V���������̂��x��
*/
void Main::SleepApp()
{
	/*! ���̎��Ԃ��擾 */
	QueryPerformanceCounter(&m_TimeEnd);
	/*! �o�ߎ��� */
	m_FrameTime = static_cast<float>(m_TimeEnd.QuadPart - m_TimeStart.QuadPart) / static_cast<float>(m_TimeFreq.QuadPart);
	if (m_FrameTime < c_MinFrameTime)
	{
		DWORD SleepTime = static_cast<DWORD>((c_MinFrameTime - m_FrameTime) * 1000);/*!< �Y���� */
		timeBeginPeriod(1);	/*!< ���𐫔\���グ�� */
		Sleep(SleepTime);	/*!< ���x�� */
		timeEndPeriod(1);	/*!< �߂� */
	}
	QueryPerformanceCounter(&m_TimeStart);
}
