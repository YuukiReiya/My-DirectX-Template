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
#include "../Shader/ShaderManager.h"
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
	Sprite();
	~Sprite();

	/*! 描画モード */
	enum Mode {
		Single,		/*!< ノーマルテクスチャ */
		Multiple	/*!< アトラステクスチャ */
	};


	HRESULT Initialize();
	void Release();

	HRESULT Render(Texture* pTexture, bool isReverse = false);

	DirectX::XMFLOAT3 GetPos()const { return m_Pos; }
	void SetPos(DirectX::XMFLOAT3 pos);//{ m_Pos.x = pos.x; m_Pos.y = pos.y;m_Pos.z = pos.z;}
	void SetPos(DirectX::XMFLOAT2 pos);
	void SetRot(DirectX::XMFLOAT3 rot) { m_Rot.x = rot.x, m_Rot.y = rot.y, m_Rot.z = rot.z; }
	void SetSplitTexture(DirectX::XMINT2 divNum);/*!< テクスチャの分割 */
	void SetActivateIndex(DirectX::XMINT2 index);/*!< 使用する描画するテクスチャを変更 */
private:

	static constexpr int c_VertexCount = 4;/*!< スプライトの頂点数 */
	static const float c_VertexZ;
	HRESULT CreateVertex(Texture* pTexture);

	//スプライト毎
	std::string m_szShaderDataUsage;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11BlendState* m_pBlendState;

	/****************************************/
	/*		　スプライトのパラメータ		*/
	/****************************************/


	/*! ローカル座標系 */
	DirectX::XMFLOAT3 m_Pos;
	DirectX::XMFLOAT3 m_Rot;
	DirectX::XMFLOAT3 m_Scale;
	DirectX::XMFLOAT3 m_Color;

	/*! スプライトの */
	DirectX::XMINT2 m_DivNum;				/*!< テクスチャの分割数 */
	DirectX::XMINT2 m_ActiveTextureIndex;	/*!< 使用するテクスチャのインデックス */
	Mode m_eMode;
	float m_Alpha;


};

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