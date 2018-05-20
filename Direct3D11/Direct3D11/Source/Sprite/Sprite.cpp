/*
	@file	Sprite.h
	@date	2018/03/04
	@author	番場 宥輝
	@brief	スプライト
*/
#include <D3D11.h>
#include "Sprite.h"
#include "../Direct3D11/Direct3D11.h"
#include "../MyGame.h"
#include "../MemoryLeaks.h"
#include "../Camera/Camera.h"
#include "../Shader/ShaderManager.h"
#include <math.h>

Sprite::Sprite()
{
	SecureZeroMemory(this, sizeof(this));
	m_ActiveTextureIndex = { 0,0 };
	m_Pos = { 0,0,1 };
	m_DivNum = { 1,1 };
	m_Scale = { 1,1 ,1 };
	m_Rot = {0,0,0};
}


Sprite::~Sprite()
{
	//Release();
}

HRESULT Sprite::Initialize()
{
	HRESULT hr;

	/*! 頂点定義 */
	//SpriteVertex vertices[] =
	//{
	//	/*! 左下頂点 */
	//	{
	//		DirectX::XMFLOAT3(-0.5,-0.5,1),
	//		DirectX::XMFLOAT2(0,1)
	//	},

	//	/*! 左上頂点 */
	//	{
	//		DirectX::XMFLOAT3(-0.5, 0.5,1),
	//		DirectX::XMFLOAT2(0,0)
	//	},

	//	/*! 右下頂点 */
	//	{
	//		DirectX::XMFLOAT3(0.5,-0.5,1),
	//		DirectX::XMFLOAT2(1,1)
	//	},

	//	/*! 右上頂点 */
	//	{
	//		DirectX::XMFLOAT3(0.5, 0.5,1),
	//		DirectX::XMFLOAT2(1,0)
	//	},
	//};

	/*! 板ポリゴン(四角形ポリゴン)のバッファを定義 */
	//D3D11_BUFFER_DESC bd;
	//SecureZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DEFAULT;				/*!< GPUから読み込みと書き込みを許可 */
	//bd.ByteWidth = sizeof(vertices);			/*!< バッファのサイズ */
	//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	/*!< 頂点バッファとしてレンダリングパイプラインにバインド */

	/*! サブリソースのデータを定義 */
	//D3D11_SUBRESOURCE_DATA subResourceData;
	//SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	//subResourceData.pSysMem = vertices;			/*!< 初期化データへのポインタ */


	/*! 頂点バッファ生成 */
	//hr = Direct3D11::GetDevice()->CreateBuffer(
	//	&bd,
	//	&subResourceData,
	//	&m_pVertexBuffer
	//);
	//if (FAILED(hr)) {
	//	std::string error = "SpriteBuffer is not Create!";
	//	ErrorLog(error);
	//	return E_FAIL;
	//}

	/*! バーテックスバッファを設定 */
	//uint32_t stride = sizeof(SpriteVertex);
	//uint32_t offset = 0;
	//Direct3D11::GetDeviceContext()->IASetVertexBuffers(
	//	0,
	//	1,
	//	&m_pVertexBuffer,
	//	&stride,
	//	&offset
	//);

	///*! テクスチャサンプラーを定義 */
	//D3D11_SAMPLER_DESC sd;
	//SecureZeroMemory(&sd, sizeof(sd));
	//sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	///*! テクスチャサンプラーを作成 */
	//hr = Direct3D11::GetDevice()->CreateSamplerState(
	//	&sd,
	//	&m_pSampleLinear
	//);
	//if (FAILED(hr)) { ErrorLog("da"); }

	///*! マルチバイト・ワイド文字の両対応させるために変換 */
	//std::string filePath = "Sprite.jpg";
	//auto s_path = tString(filePath);
	//const auto path = const_cast<LPTSTR>(s_path.c_str());

	///*! テクスチャ作成 */
	//hr = D3DX11CreateShaderResourceViewFromFile(
	//	Direct3D11::GetDevice(),
	//	path,
	//	NULL,
	//	NULL,
	//	&m_pTexture,
	//	NULL
	//);
	//if (FAILED(hr)) {
	//	std::string error = "\"" + filePath + "\" is not texture loading!";
	//	ErrorLog(error);
	//	return E_FAIL;
	//}

	ID3DBlob* pCompiledShader = NULL;/*!< コンパイル用ブロブ */

	/*! バーテックスシェーダーの作成 */
	hr = ShaderManager::MakeShader("SimpleTexture.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader, &pCompiledShader);
	if (FAILED(hr)) {
		return E_FAIL;
	}

	/*! 頂点インプットレイアウト定義 */
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	uint32_t numElements = sizeof(layout) / sizeof(layout[0]);/*!< ポインタ */

	/*! 頂点インプットレイアウトを作成 */
	hr = Direct3D11::GetDevice()->CreateInputLayout(
		layout,
		numElements,
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		&m_pVertexLayout
	);
	if (FAILED(hr)) {
		return E_FAIL;
	}

	/*! ピクセルシェーダーの作成 */
	hr = ShaderManager::MakeShader("SimpleTexture.hlsl", "PS", "ps_5_0", (void**)&m_pPixelShader, &pCompiledShader);
	if (FAILED(hr)) {
		return E_FAIL;
	}
	
	if (pCompiledShader != nullptr) {
		pCompiledShader->Release();
	}

	/*! コンスタントバッファ定義 */
	D3D11_BUFFER_DESC cb;
	SecureZeroMemory(&cb, sizeof(cb));
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SpriteShaderBuffer);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	/*! コンスタントバッファ作成 */
	hr = Direct3D11::GetDevice()->CreateBuffer(&cb, NULL, &m_pConstantBuffer);
	if (FAILED(hr)) {
		std::string error = "ConstantBuffer is not create!";
		ErrorLog(error);
		return E_FAIL;
	}

	/*! αブレンド */
	D3D11_BLEND_DESC alphaBlend;
	SecureZeroMemory(&alphaBlend, sizeof(alphaBlend));
	alphaBlend.RenderTarget[0].BlendEnable = true;
	alphaBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	alphaBlend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	alphaBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	alphaBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	alphaBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	alphaBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	alphaBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = Direct3D11::GetDevice()->CreateBlendState(
		&alphaBlend,
		&m_pBlendState
	);
	if (FAILED(hr)) {
		std::string error = "BlendState is not create!";
		ErrorLog(error);
		return E_FAIL;
	}

	return S_OK;
}

/*!
	@brief	描画モードにアトラステクスチャを指定(テクスチャを分割)
	@detail	横にx個、縦にy個になるようテクスチャを分割する
*/
void Sprite::SetSplitTexture(DirectX::XMINT2 divNum)
{
#ifdef DEBUG_SPRITE
	/*! エラー検知 */
	try
	{
		if (divNum.x < 0) { throw "x=" + std::to_string(divNum.x); }
		if (divNum.y < 0) { throw "y=" + std::to_string(divNum.y); }
	}
	catch (std::string xORy) 
	{
		std::string error = "範囲外参照\nSetSplitTexture関数の引数に0以下の値を渡すことが出来ません。\n" + xORy;
		ErrorLog(error);
	}
#endif // DEBUG_SPRITE

	m_DivNum = divNum;
}

/*!
	@brief	描画モードがアトラステクスチャのものの描画インデックスを変更
	@detail	インデックスの範囲{0<=index.x<m_divNum.x,0<=index.y<m_divNum.y}
*/
void Sprite::SetActivateIndex(DirectX::XMINT2 index)
{
#ifdef DEBUG_SPRITE
	try
	{
		if (index.x < 0) { throw -1;}
		if (index.y < 0) { throw -1;}
		if (m_DivNum.x <= index.x) { throw 0; }
		if (m_DivNum.y <= index.y) { throw 0; }
	}
	catch (int error_code)
	{

		std::string category;	/*!< カテゴリ */
		std::string cause;		/*!< 原因 */
		std::string input;		/*!< 入力値 */
		std::string error;		/*!< ログに表示するエラー格納変数 */

		switch (error_code)
		{
		case -1:
		{
			std::string detail;	/*!< 詳細 */
			std::string range;	/*!< 範囲 */

			category = "範囲外参照\n";
			cause = "描画インデックスに対して無効な負の値が入力されました。\n";
			detail = "インデックスに負の値を入力することは出来ません。\n";
			range = "\n0<=x<" + std::to_string(m_DivNum.x) + " , 0<=y<" + std::to_string(m_DivNum.y) + "\n\n";
			input = "入力値｛ " + std::to_string(index.x) + " , " + std::to_string(index.y) + " ｝";
			error = category + cause + detail + range + input;
		}
		break;
		case 0:
		{
			std::string division;	/*!< 分割数 */

			category = "範囲外参照\n";
			cause = "分割したサイズに対して分割サイズを超える無効な値が入力されました。\n";
			division = "分割数｛ " + std::to_string(m_DivNum.x) + " , " + std::to_string(m_DivNum.y) + " ｝\n";
			input = "入力値｛ " + std::to_string(index.x) + " , " + std::to_string(index.y) + " ｝";
			error = category + cause + division + input;
		}
		break;
		}
		ErrorLog(error);
	}
#endif // DEBUG_SPRITE
	m_ActiveTextureIndex = index;
}


/*!
	@brief	解放
*/
void Sprite::Release()
{
	//m_pSampleLinear->Release();
	//m_pTexture->Release();

	if (m_pConstantBuffer != nullptr) {
		m_pConstantBuffer->Release();
	}
	if (m_pVertexShader != nullptr) {
		m_pVertexShader->Release();
	}
	if (m_pPixelShader != nullptr) {
		m_pPixelShader->Release();
	}
	if (m_pVertexBuffer!=nullptr) {
		m_pVertexBuffer->Release();
	}
	if (m_pVertexLayout != nullptr) {
		m_pVertexLayout->Release();
	}

	m_pConstantBuffer	= nullptr;
	m_pVertexShader		= nullptr;
	m_pPixelShader		= nullptr;
	m_pVertexBuffer		= nullptr;
	m_pVertexLayout		= nullptr;
}

/*!
	@brief	描画
	@detail	スプライトの中心は中心座標
*/
HRESULT Sprite::Render(Texture * texture, bool isReverse)
{
	/*! テクスチャデータの取得 */
	auto size = texture->GetSize();		/*!< テクスチャサイズ */
	auto pSampler = texture->GetSamplerState();
	auto pTex = texture->GetTexture();	/*!< テクスチャデータ */

	auto large = size.x < size.y ? size.y : size.x;

	/*! 板ポリゴンの座標 */
	DirectX::XMFLOAT2 leftTop;		
	DirectX::XMFLOAT2 rightBottom;	
	
	//leftTop.x = -0.5f*size.x / large;
	//leftTop.y = 0.5f*size.y / large;

	leftTop.x = -0.5f*size.x / m_DivNum.x / large;
	leftTop.y = 0.5f*size.y / m_DivNum.y / large;

	//rightBottom.x = 0.5f*size.x / large;
	//rightBottom.y = -0.5f*size.y / large;

	rightBottom.x = 0.5f*size.x / m_DivNum.x / large;
	rightBottom.y = -0.5f*size.y / m_DivNum.y / large;

	/*! UV(テクスチャ)座標 */
	DirectX::XMFLOAT2 uvLeftTop;
	DirectX::XMFLOAT2 uvRightBottom;

	//uvLeftTop.x = 0;
	//uvLeftTop.y = 0;
	uvLeftTop.x = (float)m_ActiveTextureIndex.x / (float)m_DivNum.x;
	uvLeftTop.y = (float)m_ActiveTextureIndex.y / (float)m_DivNum.y;

	//uvRightBottom.x = 1;
	//uvRightBottom.y = 1;

	uvRightBottom.x = (float)(m_ActiveTextureIndex.x + 1) / (float)m_DivNum.x;
	uvRightBottom.y = (float)(m_ActiveTextureIndex.y + 1) / (float)m_DivNum.y;

	/****************************************/
	/*				 ※注意					*/
	/****************************************/
	/*
		テクスチャは左上基準(0,0)
		頂点座標は中央基準(0,0)
		かつ上方:正、下方:負
	*/


	/*! 頂点定義 */
	SpriteVertex vertices[] =
	{
		/*! 右上頂点 */
		{
			DirectX::XMFLOAT3(
				rightBottom.x,
				leftTop.y,
				1
			),
			DirectX::XMFLOAT2(
				uvRightBottom.x,
				uvLeftTop.y
			),
		},
		/*! 右下頂点 */
		{
			DirectX::XMFLOAT3(
				rightBottom.x,
				rightBottom.y,
				1
			),
			DirectX::XMFLOAT2(
				uvRightBottom.x,
				uvRightBottom.y
			),
		},
		/*! 左上頂点 */
		{
			DirectX::XMFLOAT3(
				leftTop.x,
				leftTop.y,
				1
			),
			DirectX::XMFLOAT2(
				uvLeftTop.x,
				uvLeftTop.y
			),
		},
		/*! 左下頂点 */
		{
			DirectX::XMFLOAT3(
				leftTop.x,
				rightBottom.y,
				1
			),
			DirectX::XMFLOAT2(
				uvLeftTop.x,
				uvRightBottom.y
			),
		},
	};

	/*! 板ポリゴン(四角形ポリゴン)のバッファを定義 */
	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				/*!< GPUから読み込みと書き込みを許可 */
	bd.ByteWidth = sizeof(vertices);			/*!< バッファのサイズ */
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	/*!< 頂点バッファとしてレンダリングパイプラインにバインド */

	/*! サブリソースのデータを定義 */
	D3D11_SUBRESOURCE_DATA subResourceData;
	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = vertices;			/*!< 初期化データへのポインタ */

	HRESULT hr;

	/*! 頂点バッファ生成 */
	hr = Direct3D11::GetDevice()->CreateBuffer(
		&bd,
		&subResourceData,
		&m_pVertexBuffer
	);
	if (FAILED(hr)) {
		std::string error = "SpriteBuffer is not Create!";
		ErrorLog(error);
		return E_FAIL;
	}

	/*! トポロジーセット */
	Direct3D11::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);

	/*! 頂点インプットレイアウトセット */
	Direct3D11::GetDeviceContext()->IASetInputLayout(m_pVertexLayout);

	/*! シェーダーの登録 */
	Direct3D11::GetDeviceContext()->VSSetShader(m_pVertexShader, NULL, NULL);
	Direct3D11::GetDeviceContext()->PSSetShader(m_pPixelShader, NULL, NULL);

	/*! コンスタントバッファの登録 */
	Direct3D11::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	Direct3D11::GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);


	/*! テクスチャ */
	Direct3D11::GetDeviceContext()->PSSetSamplers(0, 1, &pSampler);
	Direct3D11::GetDeviceContext()->PSSetShaderResources(0, 1, &pTex);

	/*! 座標変換 */
	DirectX::XMMATRIX mWorld, mTran, mRot, mScale;
	mWorld = DirectX::XMMatrixIdentity();
	mTran = DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	mRot = DirectX::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	mScale = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

	mWorld = mScale*mRot*mTran;

	/*! マッピング用変数定義 */
	D3D11_MAPPED_SUBRESOURCE pData;
	SecureZeroMemory(&pData, sizeof(pData));
	SpriteShaderBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));

	/*! バッファへのアクセス許可(書き換え) */
	hr = Direct3D11::GetDeviceContext()->Map(
		m_pConstantBuffer,
		NULL,
		D3D11_MAP_WRITE_DISCARD,
		NULL,
		&pData
	);
	if (FAILED(hr)) {
		std::string error = "Texture mapping is failed!";
		ErrorLog(error);
		Direct3D11::GetDeviceContext()->Unmap(m_pConstantBuffer, NULL);/*!< アクセス権を閉じて抜ける */
		return E_FAIL;
	}

	/*! コンスタントバッファにデータを送る */
	DirectX::XMMATRIX m = mWorld*Camera::GetViewMatrix()*Camera::GetProjMatrix();
	m = DirectX::XMMatrixTranspose(m);/*!< 行列を転置行列にする */
	cb.m_WVP = m;/*!< ワールド行列 */
	cb.m_Color = m_Color;
	cb.m_Alpha = m_Alpha;

	/*! メモリコピー */
	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

	/*! アクセス許可終了 */
	Direct3D11::GetDeviceContext()->Unmap(m_pConstantBuffer, NULL);

	/*! 頂点バッファセット */
	uint32_t stride = sizeof(SpriteVertex);
	uint32_t offset = 0;
	Direct3D11::GetDeviceContext()->IASetVertexBuffers(
		0,
		1,
		&m_pVertexBuffer,
		&stride,
		&offset
	);

	/*! αブレンドの設定 */
	//無くても透過される…
	//uint32_t blendColor= 0xffffffff;
	//Direct3D11::GetDeviceContext()->OMSetBlendState(
	//	m_pBlendState,
	//	NULL,
	//	blendColor
	//);

	/*! 透過色 */
	//uint32_t blendColor= 0xffffffff;
	//Direct3D11::GetDeviceContext()->OMSetBlendState(
	//	m_pBlendState,
	//	NULL,
	//	blendColor
	//);

	/*! 描画 */
	Direct3D11::GetDeviceContext()->Draw(
		4,		/*!< 頂点数(板ポリゴンなので頂点数は4つ) */
		NULL
	);

	/*! αブレンド反映 */
	//Direct3D11::GetDeviceContext()->OMSetBlendState(NULL, NULL, blendColor);

	return S_OK;
}

