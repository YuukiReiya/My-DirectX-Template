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

	/*! テクスチャサンプラーを定義 */
	D3D11_SAMPLER_DESC sd;
	SecureZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	//sd.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	//sd.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	//sd.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;

	sd.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
	/*! テクスチャサンプラーを作成 */
	hr = Direct3D11::GetInstance().GetDevice()->CreateSamplerState(
		&sd,
		&m_pSamplerState
	);
	if (FAILED(hr)) {
		std::string error = "\"" + filePath + "\" is not created texture sampler!";
		ErrorLog(error);
		return E_FAIL;
	}

	m_pSamplerState;

	/*! テクスチャ作成 */
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
	@brief	インスタンス破棄
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
	@brief	タイリングモードの設定
	@detail	サンプラーステートの再作成
*/
HRESULT Texture::SetTileMode(TileMode tile, FilteringMode filter)
{
	D3D11_SAMPLER_DESC sd;
	SecureZeroMemory(&sd, sizeof(sd));
	sd.Filter = static_cast<D3D11_FILTER>(filter);

}
