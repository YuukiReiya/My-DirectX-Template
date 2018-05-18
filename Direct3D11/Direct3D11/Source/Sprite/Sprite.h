/*
	@file	Sprite.h
	@date	2018/03/04
	@author	番場 宥輝
	@brief	スプライト関連
	@detail	αブレンドについて参考サイト:http://neareal.net/index.php?ComputerGraphics%2FXNA%2FBlendState
*/
#pragma once
#include <string>
#include <D3D11.h>
#include "../Shader/StructShaderBase.h"
#include <DirectXMath.h>

#include "../Texture/Texture.h"

#ifdef _DEBUG
#define DEBUG_SPRITE
#endif // _DEBUG

/*!
	スプライトを扱うクラス
*/
class Sprite
{
public:
	explicit Sprite();
	~Sprite();

	/*! 描画モード */
	enum Mode {
		Single,		/*!< ノーマルテクスチャ */
		Multiple	/*!< アトラステクスチャ */
	};


	HRESULT Initialize();
	//HRESULT InitPolygon();
	//HRESULT InitShader(std::string hlslPath);
	void Release();

	void Render();
	HRESULT Render(Texture*texture, bool isReverse = false);
	//void RenderSprite(DirectX::XMMATRIX WVP);

	void SetPos(DirectX::XMFLOAT3 pos) { m_Pos.x = pos.x; m_Pos.y = pos.y;m_Pos.z = pos.z;}
	void SetRot(DirectX::XMFLOAT3 rot) { m_Rot.x = rot.x, m_Rot.y = rot.y, m_Rot.z = rot.z; }
	void SetSplitTexture(DirectX::XMINT2 devNum);/*!< テクスチャの分割 */
	void SetActivateIndex(DirectX::XMINT2 index);
private:
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;

	//スプライト毎
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11SamplerState*m_pSampleLinear;
	ID3D11ShaderResourceView* m_pTexture;

	//
	DirectX::XMFLOAT3 m_Pos;
	DirectX::XMFLOAT3 m_Rot;
	DirectX::XMFLOAT3 m_Scale;
	DirectX::XMFLOAT3 m_Color;
	DirectX::XMINT2 m_DevNum;				/*!< テクスチャの分割数 */
	DirectX::XMINT2 m_ActiveTextureIndex;	/*!< 使用するテクスチャのインデックス */
	Mode m_eMode;

	float m_Alpha;
	ID3D11BlendState* m_pBlendState;



	/****************************************/
	/*		スプライトで扱う構造体			*/
	/****************************************/

	/*!
		@brief	スプライトの頂点構造体
	*/
	struct SpriteVertex
		:public VARTEX_BASE
	{
		SpriteVertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 uv) {
			m_Pos = pos, m_UV = uv;
		}
		DirectX::XMFLOAT2 m_UV;
	};

	/*!
		@brief	スプライトのコンスタントバッファ構造体
	*/
	struct SpriteShaderBuffer
		:CONSTANT_BUFFER_BASE
	{
		ALIGN16<DirectX::XMFLOAT3>	m_Color;
		ALIGN16<float>				m_Alpha;
	};
};