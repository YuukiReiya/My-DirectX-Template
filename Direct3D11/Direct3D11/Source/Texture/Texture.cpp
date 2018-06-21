#include "Texture.h"
#include "../Direct3D11/Direct3D11.h"
#include "../MemoryLeaks.h"
#include "../MyGame.h"

Texture::Texture()
{
	SecureZeroMemory(this, sizeof(this));
	m_pSamplerState		= NULL;
	m_pResorceTexture	= NULL;
	m_eTileMode = TileMode::Clamp;
	m_eFilteringMode = FilteringMode::Trilinear;
}


Texture::~Texture()
{
	Finalize();
}

HRESULT Texture::Load(std::string filePath, DirectX::XMINT2 size)
{

	HRESULT hr;
	auto s_path = tString(filePath);
	const auto path = const_cast<LPTSTR>(s_path.c_str());

	m_Size = size;

	/*! �e�N�X�`���T���v���[���` */
	//D3D11_SAMPLER_DESC sd;
	//SecureZeroMemory(&sd, sizeof(sd));
	//sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sd.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	//sd.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	//sd.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	////sd.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	////sd.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	////sd.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;

	//sd.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
	//sd.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
	//sd.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
	///*! �e�N�X�`���T���v���[���쐬 */
	//hr = Direct3D11::GetInstance().GetDevice()->CreateSamplerState(
	//	&sd,
	//	&m_pSamplerState
	//);
	//if (FAILED(hr)) {
	//	std::string error = "\"" + filePath + "\" is not created texture sampler!";
	//	ErrorLog(error);
	//	return E_FAIL;
	//}
	SetTileMode(m_eTileMode, m_eFilteringMode);

	/*! �e�N�X�`���쐬 */
	hr = D3DX11CreateShaderResourceViewFromFile(
		Direct3D11::GetInstance().GetDevice(),
		path,
		NULL,
		NULL,
		&m_pResorceTexture,
		NULL
	);

	if (FAILED(hr)) {
		std::string error = "\"" + filePath + "\" is not load in texture!";
		ErrorLog(error);
		return E_FAIL;
	}

	return S_OK;
}

/*!
	@brief	�C���X�^���X�j��
*/
void Texture::Finalize()
{
	if (m_pSamplerState != NULL) {
		m_pSamplerState->Release();
		m_pSamplerState = NULL;
	}
	if (m_pResorceTexture != NULL) {
		m_pResorceTexture->Release();
		m_pResorceTexture = NULL;
	}
}

/*!
	@brief		�^�C�����O���[�h�̐ݒ�
	@param[in]	�ݒ肷��^�C�����[�h
	@param[in]	�ݒ肷��t�B���^�����O(�A�h���b�V���O���[�h)
	@detail		�T���v���[�X�e�[�g�̍쐬
*/
HRESULT Texture::SetTileMode(TileMode tile, FilteringMode filter)
{
	/*! �����̕ێ� */
	m_eTileMode			= tile;
	m_eFilteringMode	= filter;

	/*! �T���v���[�X�e�[�g�ݒ� */
	D3D11_SAMPLER_DESC sd;
	SecureZeroMemory(&sd, sizeof(sd));
	sd.Filter = static_cast<D3D11_FILTER>(m_eFilteringMode);			/*!< �t�B���^�����O */
	sd.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_eTileMode);	/*!< �A�h���b�V���O���[�h */
	sd.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_eTileMode);	/*!< �A�h���b�V���O���[�h */
	sd.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_eTileMode);	/*!< �A�h���b�V���O���[�h */

	HRESULT hr;
	/*! �T���v���[�쐬 */
	hr= Direct3D11::GetInstance().GetDevice()->CreateSamplerState(
		&sd,
		&m_pSamplerState
	);
	if (FAILED(hr)) {
		std::string error = "Sampler state is not created!";
		ErrorLog(error);
		return E_FAIL;
	}
	return S_OK;
}
