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
	//SecureZeroMemory(this, sizeof(this));
	m_pVertexBuffer		= NULL;
	m_pBlendState		= NULL;
	m_ActiveTextureIndex = { 0,0 };
	m_Pos = { 0,0,0 };
	m_DivNum = { 1,1 };
	m_Scale = { 1,1 ,1 };
	m_Rot = {0,0,0};
	m_szShaderDataUsage = ShaderManager::GetInstance().c_SpriteDefault;
}
const float Sprite::c_VertexZ = 0;

Sprite::~Sprite()
{
	Release();
}

HRESULT Sprite::Initialize()
{
	HRESULT hr;

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

	hr = Direct3D11::GetInstance().GetDevice()->CreateBlendState(
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
	if (m_pVertexBuffer!=NULL) {
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}
	if (m_pBlendState != NULL) {
		m_pBlendState->Release();
		m_pBlendState = NULL;
	}
}

/*!
	@brief	描画
	@detail	スプライトの中心は中心座標
*/
HRESULT Sprite::Render(Texture * texture, bool isReverse)
{
	/*! テクスチャデータの取得 */
	auto& size = texture->GetSize();		/*!< テクスチャサイズ */
	auto pSampler = texture->GetSamplerState();
	auto pTex = texture->GetTexture();	/*!< テクスチャデータ */

	auto large = size.x < size.y ? size.y : size.x;

	///*! 板ポリゴンの座標 */
	DirectX::XMFLOAT2 leftTop;		
	DirectX::XMFLOAT2 rightBottom;	
	
	//leftTop.x = -0.5f*size.x / m_DivNum.x / large;
	//leftTop.y = 0.5f*size.y / m_DivNum.y / large;

	//rightBottom.x = 0.5f*size.x / m_DivNum.x / large;
	//rightBottom.y = -0.5f*size.y / m_DivNum.y / large;

	leftTop.x = rightBottom.y = -0.5f;
	leftTop.y = rightBottom.x = 0.5f;

	/*! UV(テクスチャ)座標 */
	DirectX::XMFLOAT2 uvLeftTop;
	DirectX::XMFLOAT2 uvRightBottom;

	uvLeftTop.x = (float)m_ActiveTextureIndex.x / (float)m_DivNum.x;
	uvLeftTop.y = (float)m_ActiveTextureIndex.y / (float)m_DivNum.y;

	uvRightBottom.x = (float)(m_ActiveTextureIndex.x + 1) / (float)m_DivNum.x;
	uvRightBottom.y = (float)(m_ActiveTextureIndex.y + 1) / (float)m_DivNum.y;

	uvLeftTop.x = uvLeftTop.y = 0;
	uvRightBottom.x = 1;
	uvRightBottom.y = 1;
	/****************************************/
	/*				 ※注意					*/
	/****************************************/
	/*
		テクスチャは左上基準(0,0)
		頂点座標は中央基準(0,0)
		かつ上方:正、下方:負
		(UVと座標系の違い)
	*/


	/*! 頂点定義 */
	SpriteVertex vertices[] =
	{
		/*! 右上頂点 */
		{
			DirectX::XMFLOAT3(
				rightBottom.x,
				leftTop.y,
				c_VertexZ
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
				c_VertexZ
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
				c_VertexZ
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
				c_VertexZ
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
	//hr = CreateVertex();
	//if (FAILED(hr)) { ErrorLog("残念でした"); }

	/*! 頂点バッファ生成 */
	hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(
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
	Direct3D11::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);

	auto shaderData = ShaderManager::GetInstance().GetShaderData(m_szShaderDataUsage);

	/*! 頂点インプットレイアウトセット */
	Direct3D11::GetInstance().GetDeviceContext()->IASetInputLayout(shaderData->m_pVertexLayout);

	/*! シェーダーの登録 */
	Direct3D11::GetInstance().GetDeviceContext()->VSSetShader(shaderData->m_pVertexShader, NULL, NULL);
	Direct3D11::GetInstance().GetDeviceContext()->PSSetShader(shaderData->m_pPixelShader, NULL, NULL);

	/*! コンスタントバッファの登録 */
	Direct3D11::GetInstance().GetDeviceContext()->VSSetConstantBuffers(0, 1, &shaderData->m_pConstantBuffer);
	Direct3D11::GetInstance().GetDeviceContext()->PSSetConstantBuffers(0, 1, &shaderData->m_pConstantBuffer);


	/*! テクスチャ */
	Direct3D11::GetInstance().GetDeviceContext()->PSSetSamplers(0, 1, &pSampler);
	Direct3D11::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &pTex);

	/*! 座標変換 */
	DirectX::XMMATRIX mWorld, mTran, mRot, mScale;
	mWorld = DirectX::XMMatrixIdentity();
	mTran = DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	mRot = DirectX::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	mScale = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

	/*! ワールド変換 */
	mWorld = mScale*mRot*mTran;

	/*! マッピング用変数定義 */
	D3D11_MAPPED_SUBRESOURCE pData;
	SecureZeroMemory(&pData, sizeof(pData));
	SpriteShaderBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));

	/*! バッファへのアクセス許可(書き換え) */
	hr = Direct3D11::GetInstance().GetDeviceContext()->Map(
		shaderData->m_pConstantBuffer,
		NULL,
		D3D11_MAP_WRITE_DISCARD,
		NULL,
		&pData
	);
	if (FAILED(hr)) {
		std::string error = "Texture mapping is failed!";
		ErrorLog(error);
		Direct3D11::GetInstance().GetDeviceContext()->Unmap(shaderData->m_pConstantBuffer, NULL);/*!< アクセス権を閉じて抜ける */
		return E_FAIL;
	}

	/*! コンスタントバッファにデータを送る */
	auto camera = &Camera::GetInstance();
	DirectX::XMMATRIX m = mWorld*camera->GetViewMatrix()*camera->GetProjMatrix();
	m = DirectX::XMMatrixTranspose(m);	/*!< 転置行列 */
	cb.m_WVP = m;						/*!< ワールド行列 */



	cb.m_Color = m_Color;
	cb.m_Alpha = m_Alpha;

	/*! メモリコピー */
	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

	/*! アクセス許可終了 */
	Direct3D11::GetInstance().GetDeviceContext()->Unmap(shaderData->m_pConstantBuffer, NULL);

	/*! 頂点バッファセット */
	uint32_t stride = sizeof(SpriteVertex);
	uint32_t offset = 0;
	Direct3D11::GetInstance().GetDeviceContext()->IASetVertexBuffers(
		0,
		1,
		&m_pVertexBuffer,
		&stride,
		&offset
	);

	/*! αブレンドの設定 */
	//無くても透過される…
	//uint32_t blendColor= 0xffffffff;
	//Direct3D11::GetInstance().GetDeviceContext()->OMSetBlendState(
	//	m_pBlendState,
	//	NULL,
	//	blendColor
	//);

	/*! 透過色 */
	//uint32_t blendColor= 0xffffffff;
	//Direct3D11::GetInstance().GetDeviceContext()->OMSetBlendState(
	//	m_pBlendState,
	//	NULL,
	//	blendColor
	//);

	/*! 描画 */
	Direct3D11::GetInstance().GetDeviceContext()->Draw(
		4,		/*!< 頂点数(板ポリゴンなので頂点数は4つ) */
		NULL
	);

	/*! αブレンド反映 */
	//Direct3D11::GetInstance().GetDeviceContext()->OMSetBlendState(NULL, NULL, blendColor);

	return S_OK;
}

HRESULT Sprite::CreateVertex()
{
	DirectX::XMFLOAT2 leftTop, rightBottom;

	SpriteVertex vertices[] = {
		/*! 右上 */
		{
			/*! 頂点 */
			DirectX::XMFLOAT3(
				rightBottom.x,
				leftTop.y,
				m_Pos.z
			),
			/*! UV座標 */
			DirectX::XMFLOAT2(
				1,
				0
			),
		},
		/*! 右下 */
		{
			/*! 頂点 */
			DirectX::XMFLOAT3(
				rightBottom.x,
				rightBottom.y,
				m_Pos.z
			),
			/*! UV座標 */
			DirectX::XMFLOAT2(
				1,
				1
			),
		},
		/*! 左上 */
		{
			/*! 頂点 */
			DirectX::XMFLOAT3(
				leftTop.x,
				leftTop.y,
				m_Pos.z
			),
			/*! UV座標 */
			DirectX::XMFLOAT2(
				0,
				0
			),
		},
		/*! 左下 */
		{
			/*! 頂点 */
			DirectX::XMFLOAT3(
				leftTop.x,
				rightBottom.y,
				m_Pos.z
			),
			/*! UV座標 */
			DirectX::XMFLOAT2(
				0,
				1
			),

		}
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
	hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(
		&bd,
		&subResourceData,
		&m_pVertexBuffer
	);
	if (FAILED(hr)) {
		std::string error = "SpriteBuffer is not Create!";
		ErrorLog(error);
		return E_FAIL;
	}
	return S_OK;
}

void Sprite::SetPos(DirectX::XMFLOAT3 pos)
{
#ifdef DEBUG_SPRITE

	float clip = Camera::GetInstance().GetEyePt().z;


	//pos.z<clip
#endif // DEBUG_SPRITE

	m_Pos = pos;

	//auto a = Camera::GetInstance();
	//std::string e = "x=" + std::to_string(a.GetEyePt().x) + ",y=" + std::to_string(a.GetEyePt().y) + ",z=" + std::to_string(a.GetEyePt().z)+"\n"+
	//	"x=" + std::to_string(m_Pos.x) + ",y=" + std::to_string(m_Pos.y) + ",z=" + std::to_string(m_Pos.z);

	//ErrorLog(e);

	//DirectX::XMFLOAT3 
}

void Sprite::SetPos(DirectX::XMFLOAT2 pos)
{
	m_Pos = { pos.x,pos.y,m_Pos.z };
}

