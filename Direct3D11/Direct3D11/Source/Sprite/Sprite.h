/*
	@file	Sprite.h
	@date	2018/03/04
	@author	�ԏ� �G�P
	@brief	�X�v���C�g�֘A
	@detail	���u�����h�ɂ��ĎQ�l�T�C�g:http://neareal.net/index.php?ComputerGraphics%2FXNA%2FBlendState
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
	�X�v���C�g�������N���X
*/
class Sprite
{
public:
	Sprite();
	~Sprite();

	/*! �`�惂�[�h */
	enum Mode {
		Single,		/*!< �m�[�}���e�N�X�`�� */
		Multiple	/*!< �A�g���X�e�N�X�`�� */
	};


	HRESULT Initialize();
	void Release();

	HRESULT Render(Texture* pTexture, bool isReverse = false);
//	HRESULT Render(Texture* pTexture);

	DirectX::XMFLOAT3 GetPos()const { return m_Pos; }
	
	void SetPos(DirectX::XMFLOAT3 pos);//{ m_Pos.x = pos.x; m_Pos.y = pos.y;m_Pos.z = pos.z;}
	void SetPos(DirectX::XMFLOAT2 pos);
	void SetRot(DirectX::XMFLOAT3 rot) { m_Rot.x = rot.x, m_Rot.y = rot.y, m_Rot.z = rot.z; }
	void SetSplitTexture(DirectX::XMINT2 divNum);/*!< �e�N�X�`���̕��� */
	void SetActivateIndex(DirectX::XMINT2 index);/*!< �g�p����`�悷��e�N�X�`����ύX */
	void SetAlpha(float alpha) { m_Alpha = alpha; }
	void SetStencilMask(uint32_t mask) { m_StencilMask = mask; }

	void CreateAlphaBlendState();

private:

	static constexpr int c_VertexCount = 4;/*!< �X�v���C�g�̒��_�� */
	static const float c_VertexZ;
	HRESULT CreateVertex(DirectX::XMINT2 size);

	HRESULT CreateClampVertex(DirectX::XMINT2 size);	/*!< �ꖇ�G�^�C�����O/�������� */
	HRESULT CreateTilingVertex(DirectX::XMINT2 size);	/*!< �^�C�����O�p�̒��_���� */
	HRESULT CreateSplitVertex(DirectX::XMINT2 size);	/*!< �����p���_���� */


	

	//�X�v���C�g��
	std::string m_szShaderDataUsage;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11BlendState* m_pBlendState;
	uint32_t m_StencilMask;
	/****************************************/
	/*		�@�X�v���C�g�̃p�����[�^		*/
	/****************************************/


	/*! ���[�J�����W�n */
	DirectX::XMFLOAT3 m_Pos;
	DirectX::XMFLOAT3 m_Rot;
	DirectX::XMFLOAT3 m_Scale;
	DirectX::XMFLOAT3 m_Color;

	/*! �X�v���C�g�� */
	DirectX::XMINT2 m_DivNum;				/*!< �e�N�X�`���̕����� */
	DirectX::XMINT2 m_ActiveTextureIndex;	/*!< �g�p����e�N�X�`���̃C���f�b�N�X */
	DirectX::XMFLOAT2 m_TilingRatio;		/*!< �^�C�����O����ۂ̊��� */
	Mode m_eMode;
	float m_Alpha;


};

/****************************************/
/*		�X�v���C�g�ň����\����			*/
/****************************************/

/*!
	@brief	�X�v���C�g�̒��_�\����
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
	@brief	�X�v���C�g�̃R���X�^���g�o�b�t�@�\����
*/
struct SpriteShaderBuffer
	:CONSTANT_BUFFER_BASE
{
	ALIGN16<DirectX::XMFLOAT3>	m_Color;
	ALIGN16<float>				m_Alpha;
};