/*
	@file	Direct3D11.h
	@date	2018/02/11
	@author	�ԏ� �G�P
	@brief	Direct3D11�f�o�C�X
	@detail	�V���O���g��
*/
#include "Direct3D11.h"
#include "../MemoryLeaks.h"
#include "../MyGame.h"

/*!
	@def	MSAA
	@brief	�}���`�T���v���E�A���`�G�C���A�X(�R�����g�A�E�g�����MSAA����)
*/
//#define MSAA

/*!
	@def	�萔
*/
const float Direct3D11::c_ClearColor[4]{ 0.5f,0.5f,1.5f,1.0f };		/*!< �`��^�[�Q�b�g�N���A�J���[ */

/*!
	@brief	�R���X�g���N�^
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
	@brief	�f�X�g���N�^
*/
Direct3D11::~Direct3D11()
{
	Release();
}

/*!
	@brief	������
*/
bool Direct3D11::Init(HWND hWnd)
{
	/*! �f�o�C�X�ƃX���b�v�E�`�F�C���쐬 */
	DXGI_SWAP_CHAIN_DESC sd;
	SecureZeroMemory(&sd, sizeof(sd));
	sd.BufferCount							= 1;							/*!< �o�b�N�E�o�b�t�@�� */
	sd.BufferDesc.Width						= WINDOW_WIDTH;					/*!< �o�b�N�E�o�b�t�@�̕� */
	sd.BufferDesc.Height					= WINDOW_HEIGHT;				/*!< �o�b�N�E�o�b�t�@�̍��� */
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;	/*!< �t�H�[�}�b�g */
	sd.BufferDesc.RefreshRate.Numerator		= 60;							/*!< ���t���b�V���E���[�g(���q) */
	sd.BufferDesc.RefreshRate.Denominator	= 1;								/*!< ���t���b�V���E���[�g(����) */
	/*! �X�L�������C���`�惂�[�h */
	sd.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	/*! �X�P�[�����O�E���[�h */
	sd.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;						/*!< �o�b�N�E�o�b�t�@�̎d�l */
	sd.OutputWindow							= hWnd;													/*!< �֘A�t����E�B���h�E */

#if defined(MSAA)
	/*! �A���`�G�C���A�X�����L�� */

	//http://nayuta-solution.hateblo.jp/entry/2015/01/17/142751 ��������Q�l�ɐݒ���s��	���r��

	/*! �g�p�ł���MSAA���`�F�b�N */
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
	/*! �A���`�G�C���A�X�������� */
	sd.SampleDesc.Count = 1;							/*!< �}���`�E�T���v���̐� */
	sd.SampleDesc.Quality = 0;							/*!< �}���`�E�T���v���̃N�I���e�B */
#endif // �A���`�G�C���A�X
	sd.Windowed = true;									/*!< �E�B���h�E�E���[�h */
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	/*!< �X���b�v�E�`�F�C�����ʎw�� */

	/*! �@�\���x�����w�肷��z�� */
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	D3D_FEATURE_LEVEL* pFeatureLevelsSupported = NULL;	/*!< �@�\���x�����擾����ϐ� */

#if defined(DEBUG)||defined(_DEBUG)
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
//	UINT createDeviceFlags = 0;
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif//DEBUG|_DEBUG

	/*! �f�o�C�X�ƃX���b�v�E�`�F�C���̍쐬 */
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,							/*!< �g�p����IDXGIAdapter �C���^�[�t�F�[�X */
		D3D_DRIVER_TYPE_HARDWARE,		/*!< Direct3D 11 �f�o�C�X�̎�� */
		NULL,							
		createDeviceFlags,				/*!< �g�p����API ���C���[�̎w�� */
		&featureLevels[0],				/*!< �@�\���x�����w�肷��z�� */
		GetArraySize(featureLevels),	/*!< ��L�z��̗v�f�� */
		D3D11_SDK_VERSION,				/*!< �g�p���Ă���SDK�̃o�[�W���� */
		&sd,							/*!< �X���b�v�E�`�F�C���̐ݒ� */
		m_pSwapChain.GetAddressOf(),	/*!< IDXGISwapChain �C���^�[�t�F�[�X�̕ϐ� */
		m_pDevice.GetAddressOf(),		/*!< ID3D11Device �C���^�[�t�F�[�X�̕ϐ� */
		pFeatureLevelsSupported,		/*!< �@�\���x�����擾����ϐ� */
		m_pDeviceContext.GetAddressOf()	/*!< ID3D11DeviceContext �C���^�[�t�F�[�X�̕ϐ� */
	);
	if (FAILED(hr)) { 
		ErrorLog("SwapChain is not create!");
		return false;/*!< �f�o�C�X�̍쐬�Ɏ��s */
	}

	/*! �X���b�v�E�`�F�C������ŏ��̃o�b�N�o�b�t�@���擾���� */
	ID3D11Texture2D* pBackBuffer;
	hr = m_pSwapChain->GetBuffer(
		0,								/*!< �o�b�N�E�o�b�t�@�ԍ� */
		__uuidof(ID3D11Texture2D),		/*!< �o�b�t�@�ɃA�N�Z�X����C���^�[�t�F�[�X */
		(LPVOID*)&pBackBuffer			/*!< �o�b�t�@���󂯎��ϐ� */
	);
	if (FAILED(hr)) { 
		ErrorLog("Can't take top back buffer!");
		return false; /*!< �擾���s */
	}
	
	/*! �����_�[�^�[�Q�b�g�r���[�̍쐬 */
	hr = m_pDevice->CreateRenderTargetView(
		pBackBuffer,						/*!< �r���[�ŃA�N�Z�X���郊�\�[�X */
		NULL,								/*!< �����_�[�^�[�Q�b�g�r���[�̒�` */
		m_pRenderTargetView.GetAddressOf()	/*!< �����_�[�^�[�Q�b�g�r���[���󂯎��ϐ� */
	);
	COM_RELEASE(pBackBuffer);/*!< �o�b�N�E�o�b�t�@�͈ȍ~�g��Ȃ��̂ŉ�� */
	if (FAILED(hr)) { 
		ErrorLog("RenderTargetView is not create!");
		return false;/*!< �����_�[�^�[�Q�b�g�r���[�̍쐬�Ɏ��s */
	}

	/*! �[�x / �X�e���V���E�e�N�X�`���̐ݒ� */
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width					= WINDOW_WIDTH;					/*!< �� */
	descDepth.Height				= WINDOW_HEIGHT;				/*!< ���� */
	descDepth.MipLevels				= 1;							/*!< �~�b�v�}�b�v�E���x���� */
	descDepth.ArraySize				= 1;							/*!< �z��T�C�Y */
	descDepth.Format				= DXGI_FORMAT_D32_FLOAT;		/*!< �t�H�[�}�b�g(�[�x�̂�) */
#if defined(MSAA)
	descDepth.SampleDesc = sampleDesc;								/*!< MSAA�̐ݒ�Ɠ������� */
#else
	descDepth.SampleDesc.Count		= 1;							/*!< �}���`�T���v�����O�̐ݒ� */
	descDepth.SampleDesc.Quality	= 0;							/*!< �}���`�T���v�����O�̕i�� */
#endif// �A���`�G�C���A�X
	descDepth.Usage					= D3D11_USAGE_DEFAULT;			/*!< �f�t�H���g�̎d�l */
	descDepth.BindFlags				= D3D11_BIND_DEPTH_STENCIL;		/*!< �[�x / �X�e���V���Ƃ��Ďg�p */
	descDepth.CPUAccessFlags		= 0;							/*!< CPU����̓A�N�Z�X���Ȃ� */
	descDepth.MiscFlags				= 0;							/*!< ���̑� �ݒ�Ȃ� */

	/*! �[�x / �X�e���V���E�e�N�X�`���̍쐬 */
	hr = m_pDevice->CreateTexture2D(
		&descDepth,						/*!< �쐬����2D�e�N�X�`���̐ݒ� */
		NULL,							/*!< MSAA�𗘗p����ꍇ NULL �ɂ��� */
		m_pDepthStencil.GetAddressOf()	/*!< �쐬�����e�N�X�`�����󂯂Ƃ�ϐ� */
	);
	if (FAILED(hr)) {
		ErrorLog("StencilTexture is not create!");
		return false;/*!< �X�e���V���e�N�X�`���̍쐬�Ɏ��s */
	}

	/*! �[�x / �X�e���V���E�r���[�ݒ� */
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format				= descDepth.Format;					/*!< �r���[�̃t�H�[�}�b�g */
	descDSV.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;	/*!< ���\�[�X�̎�� */
	descDSV.Flags				= 0;								/*!< �e�N�X�`���̃A�N�Z�X�� 0:RW(�ǂݍ��ݏ�������)  */
	descDSV.Texture2D.MipSlice	= 0;								

	/*! �[�x / �X�e���V���E�e�N�X�`���ɑ΂��r���[���쐬 */
	hr = m_pDevice->CreateDepthStencilView(
		m_pDepthStencil.Get(),			/*!< �[�x / �X�e���V���E�r���[�����e�N�X�`�� */
		&descDSV,					/*!< �[�x / �X�e���V���E�r���[�̐ݒ� */
		m_pDepthStencilView.GetAddressOf()		/*!< �쐬�����r���[���󂯎��ϐ� */
	);
	if (FAILED(hr)) {
		ErrorLog("StencilView is not create!");
		return false;
	}/*!< �[�x / �X�e���V���E�r���[�쐬�Ɏ��s */

	/*! �`��^�[�Q�b�g�E�r���[���o�̓}�l�[�W���[�̃^�[�Q�b�g�Ƃ��Đݒ� */
	m_pDeviceContext->OMSetRenderTargets(
		1,							/*!< �`��^�[�Q�b�g�� */
		m_pRenderTargetView.GetAddressOf(),		/*!< �`��^�[�Q�b�g�r���[ */
		m_pDepthStencilView.Get()			/*!< �[�x / �X�e���V���r���[ */
	);

	/*! �[�x /�X�e���V���E�X�e�[�g�ݒ� */
	D3D11_DEPTH_STENCIL_DESC dc;
	SecureZeroMemory(&dc, sizeof(dc));
	dc.DepthEnable				= true;									/*!< �[�x�e�X�g�L�� */
	dc.DepthWriteMask			= D3D11_DEPTH_WRITE_MASK_ALL;			/*!< �������� */
	dc.DepthFunc				= D3D11_COMPARISON_LESS;				/*!< ��O�̕��̂�`�� */
	dc.StencilEnable			= false;								/*!< �X�e���V���E�e�X�g���� */
	dc.StencilReadMask			= D3D11_DEFAULT_STENCIL_READ_MASK;		/*!< �X�e���V���������݃}�X�N */
	dc.StencilWriteMask			= D3D11_DEFAULT_STENCIL_WRITE_MASK;		/*!< �X�e���V���ǂݍ��݃}�X�N */

	/*! �ʂ��\�������Ă���ꍇ�̃X�e���V���E�e�X�g�̐ݒ� */
	dc.FrontFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;		/*!< �ێ� */
	dc.FrontFace.StencilDepthFailOp		= D3D11_STENCIL_OP_KEEP;		/*!< �ێ� */
	dc.FrontFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;		/*!< �ێ� */
	dc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;		/*!< �펞 */
	/*! �ʂ����������Ă���ꍇ�̃X�e���V���E�e�X�g�̐ݒ� */
	dc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;		/*!< �ێ� */
	dc.BackFace.StencilDepthFailOp		= D3D11_STENCIL_OP_KEEP;		/*!< �ێ� */
	dc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;		/*!< �ێ� */
	dc.BackFace.StencilFunc				= D3D11_COMPARISON_ALWAYS;		/*!< �펞 */

	/*! �[�x /�X�e���V���E�X�e�[�g�쐬 */
	hr = m_pDevice->CreateDepthStencilState(
		&dc,								/*!< �[�x /�X�e���V���E�X�e�[�g�ݒ��� */
		m_pDepthStencilState.GetAddressOf()	/*!< �쐬���� �[�x /�X�e���V���E�X�e�[�g���󂯎��ϐ� */
	);
	if (FAILED(hr)) { 
		ErrorLog("StencilState is not create!");
		return false; /*!< �[�x /�X�e���V���E�X�e�[�g�쐬���s */
	}

	/*! �[�x /�X�e���V���E�X�e�[�g�K�� */
	m_pDeviceContext->OMSetDepthStencilState(
		m_pDepthStencilState.Get(),		/*!< �[�x /�X�e���V���E�X�e�[�g */
		0							/*!< �X�e���V���E�e�X�g�ŎQ�ƒl */
	);

	/*! �r���[�|�[�g�̐ݒ� */
	D3D11_VIEWPORT vp;
	vp.Width	= WINDOW_WIDTH;		/*!< �r���[�|�[�g�̈�̕� */
	vp.Height	= WINDOW_HEIGHT;	/*!< �r���[�|�[�g�̈�̍��� */
	vp.MinDepth = 0.0f;				/*!< �r���[�|�[�g�̈�̐[�x�ŏ��l(�j�A�E�N���b�s���O����) */
	vp.MaxDepth = 1.0f;				/*!< �r���[�|�[�g�̈�̐[�x�ő�l(�t�@�[�E�N���b�s���O����) */
	vp.TopLeftX = 0;				/*!< �r���[�|�[�g�̈�̍���x���W */
	vp.TopLeftY = 0;				/*!< �r���[�|�[�g�̈�̍���y���W */
	m_pDeviceContext->RSSetViewports(
		1,			/*!< �r���[�|�[�g�̐� */
		&vp			/*!< �ݒ肷��r���[�|�[�g�z�� */
	);

	/*! ���X�^���C�Y�ݒ� */
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode					= D3D11_FILL_SOLID;	/*!< ���ʂɕ`�� */
	//rd.FillMode					= D3D11_FILL_WIREFRAME;	/*!< ���C���[�t���[���`�� */
	rd.CullMode					= D3D11_CULL_NONE;	/*!< ���ʂ�`�� */
	rd.FrontCounterClockwise	= false;			/*!< �[�x�o�C�A�X�u0�v */
	rd.DepthBias				= 0;
	rd.DepthBiasClamp			= 0;
	rd.SlopeScaledDepthBias		= 0;
	rd.DepthClipEnable			= false;			/*!< �[�x�N���b�s���O���� */
	rd.ScissorEnable			= false;			/*!< �V�U�[��`���� */
#if defined(MSAA)
	rd.MultisampleEnable		= true;
#else
	rd.MultisampleEnable		= false;			/*!< �}���`�T���v�����O���� */
#endif
	rd.AntialiasedLineEnable	= false;			/*!< ���C���E�A���`�G�C���A�V���O���� */

	/*! ���X�^���C�Y�ݒ�̍쐬 */
	hr = m_pDevice->CreateRasterizerState(
		&rd,					/*!< ���X�^���C�Y�ݒ� */
		m_pRasterizerState.GetAddressOf()		/*!< �ݒ���󂯎��ϐ� */
	);
	if (FAILED(hr)) { 
		ErrorLog("Rasterizer is not create!");
		return false;/*!< ���X�^���C�U�[�X�e�[�g�쐬���s */
	}
	
	/*! ���X�^���C�Y��ݒ� */
	m_pDeviceContext->RSSetState(m_pRasterizerState.Get());

	/*! ���u�����h�p�u�����h�X�e�[�g�쐬 */
	D3D11_BLEND_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
#if defined(MSAA)
//	bd.AlphaToCoverageEnable = true;		/*!< MSAA�ŕ`��^�[�Q�b�g�ɂ����ăs�N�Z���l��ݒ肷��Ƃ����l���g�� */
#else
	bd.AlphaToCoverageEnable = false;
#endif
	//bd.IndependentBlendEnable					= false;						/*!< false:RenderTarget[0]�̐ݒ�̂ݔ��f�B[1]�`[7]�͖��� */
	//bd.RenderTarget[0].BlendEnable				= true;							/*!< �u�����f�B���O��L�� */
	//bd.RenderTarget[0].SrcBlend					= D3D11_BLEND_SRC_ALPHA;		/*!< �\�[�X��RGB��I�� */
	//bd.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;	/*!< �f�B�X�e�B�l�[�V������RGB�I�� */
	//bd.RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;			/*!< RGB�̃u�����h�ݒ� */
	//bd.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;				/*!< �\�[�X�̃��l��I�� */
	//bd.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;				/*!< �f�B�X�e�B�l�[�V�����̃��l��I�� */
	//bd.RenderTarget[0].BlendOpAlpha				= D3D11_BLEND_OP_ADD;			/*!< ���l�̃u�����h�ݒ� */
	//bd.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;	/*!< RGBA�̂����������ޒl���w�肷�� */

	///*! �u�����h�X�e�[�g�쐬 */
	//hr = m_pDevice->CreateBlendState(
	//	&bd,			/*!< �u�����h�X�e�[�g�ݒ� */
	//	m_pBlendState.GetAddressOf()	/*!< �ݒ���󂯎��ϐ� */
	//);
	//if (FAILED(hr)) { 
	//	ErrorLog("BlendState is not create!");
	//	return false;
	//}/*!< �u�����h�X�e�[�g�쐬���s */
	//
	///*! �u�����h�X�e�[�g�ݒ�p�ϐ� */
	//float blendFactor[4]{ 0.0f,0.0f, 0.0f, 0.0f };
	//UINT mask=0xffffffff;

	///*! OM�Ƀu�����h�X�e�[�g�I�u�W�F�N�g�ݒ� */
	//m_pDeviceContext->OMSetBlendState(
	//	m_pBlendState.Get(),		/*!< �ݒ肷��I�u�W�F�N�g */
	//	blendFactor,		/*!< �萔�l */
	//	mask				/*!< �T���v���p�}�X�N */
	//);

	/*! �������I�� */
	return true;
}

/*!
	@brief	���
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
	@brief	�`��^�[�Q�b�g�̃N���A
*/
void Direct3D11::Clear()
{
	/*! �����_�[�^�[�Q�b�g�r���[�̃N���A */
	m_pDeviceContext->ClearRenderTargetView(
		m_pRenderTargetView.Get(),	/*!< �N���A���郌���_�[�^�[�Q�b�g�r���[ */
		c_ClearColor			/*!< �N���A�J���[ */
	);

	/*! �f�v�X�E�X�e���V���r���[�̃N���A */
	m_pDeviceContext->ClearDepthStencilView(
		m_pDepthStencilView.Get(),	/*!< �N���A����f�v�X�E�X�e���V���r���[ */
		D3D11_CLEAR_DEPTH,		/*!< �N���A����f�[�^�̌^ */
		1.0f,					/*!< �[�x�o�b�t�@�̃N���A�l */
		0						/*!< �X�e���V���o�b�t�@�̃N���A�l */
	);
}

/*!
	@brief	��ʍX�V
	@detail	(�o�b�N�o�b�t�@���t�����g�o�b�t�@��)
*/
void Direct3D11::Present()
{
	m_pSwapChain->Present(
#ifdef Vsync
		1,	/*!< ��ʍX�V�^�C�~���O(������A�̓����ݒ�) */
#else
		0,
#endif
		0	/*!< ��ʍX�V(DXGI_PRESENT_TEST���ƍX�V�͍s��Ȃ�) */
	);
}
