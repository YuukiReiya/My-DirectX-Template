/*
	@file	Sprite.h
	@date	2018/03/04
	@author	�ԏ� �G�P
	@brief	�X�v���C�g
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

	/*! ���u�����h */
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
	@brief	�`�惂�[�h�ɃA�g���X�e�N�X�`�����w��(�e�N�X�`���𕪊�)
	@detail	����x�A�c��y�ɂȂ�悤�e�N�X�`���𕪊�����
*/
void Sprite::SetSplitTexture(DirectX::XMINT2 divNum)
{
#ifdef DEBUG_SPRITE
	/*! �G���[���m */
	try
	{
		if (divNum.x < 0) { throw "x=" + std::to_string(divNum.x); }
		if (divNum.y < 0) { throw "y=" + std::to_string(divNum.y); }
	}
	catch (std::string xORy) 
	{
		std::string error = "�͈͊O�Q��\nSetSplitTexture�֐��̈�����0�ȉ��̒l��n�����Ƃ��o���܂���B\n" + xORy;
		ErrorLog(error);
	}
#endif // DEBUG_SPRITE

	m_DivNum = divNum;
}

/*!
	@brief	�`�惂�[�h���A�g���X�e�N�X�`���̂��̂̕`��C���f�b�N�X��ύX
	@detail	�C���f�b�N�X�͈̔�{0<=index.x<m_divNum.x,0<=index.y<m_divNum.y}
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

		std::string category;	/*!< �J�e�S�� */
		std::string cause;		/*!< ���� */
		std::string input;		/*!< ���͒l */
		std::string error;		/*!< ���O�ɕ\������G���[�i�[�ϐ� */

		switch (error_code)
		{
		case -1:
		{
			std::string detail;	/*!< �ڍ� */
			std::string range;	/*!< �͈� */

			category = "�͈͊O�Q��\n";
			cause = "�`��C���f�b�N�X�ɑ΂��Ė����ȕ��̒l�����͂���܂����B\n";
			detail = "�C���f�b�N�X�ɕ��̒l����͂��邱�Ƃ͏o���܂���B\n";
			range = "\n0<=x<" + std::to_string(m_DivNum.x) + " , 0<=y<" + std::to_string(m_DivNum.y) + "\n\n";
			input = "���͒l�o " + std::to_string(index.x) + " , " + std::to_string(index.y) + " �p";
			error = category + cause + detail + range + input;
		}
		break;
		case 0:
		{
			std::string division;	/*!< ������ */

			category = "�͈͊O�Q��\n";
			cause = "���������T�C�Y�ɑ΂��ĕ����T�C�Y�𒴂��閳���Ȓl�����͂���܂����B\n";
			division = "�������o " + std::to_string(m_DivNum.x) + " , " + std::to_string(m_DivNum.y) + " �p\n";
			input = "���͒l�o " + std::to_string(index.x) + " , " + std::to_string(index.y) + " �p";
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
	@brief	���
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
	@brief	�`��
	@detail	�X�v���C�g�̒��S�͒��S���W
*/
HRESULT Sprite::Render(Texture * texture, bool isReverse)
{
	/*! �e�N�X�`���f�[�^�̎擾 */
	auto& size = texture->GetSize();		/*!< �e�N�X�`���T�C�Y */
	auto pSampler = texture->GetSamplerState();
	auto pTex = texture->GetTexture();	/*!< �e�N�X�`���f�[�^ */

	auto large = size.x < size.y ? size.y : size.x;

	///*! �|���S���̍��W */
	DirectX::XMFLOAT2 leftTop;		
	DirectX::XMFLOAT2 rightBottom;	
	
	//leftTop.x = -0.5f*size.x / m_DivNum.x / large;
	//leftTop.y = 0.5f*size.y / m_DivNum.y / large;

	//rightBottom.x = 0.5f*size.x / m_DivNum.x / large;
	//rightBottom.y = -0.5f*size.y / m_DivNum.y / large;

	leftTop.x = rightBottom.y = -0.5f;
	leftTop.y = rightBottom.x = 0.5f;

	/*! UV(�e�N�X�`��)���W */
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
	/*				 ������					*/
	/****************************************/
	/*
		�e�N�X�`���͍���(0,0)
		���_���W�͒����(0,0)
		�����:���A����:��
		(UV�ƍ��W�n�̈Ⴂ)
	*/


	/*! ���_��` */
	SpriteVertex vertices[] =
	{
		/*! �E�㒸�_ */
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
		/*! �E�����_ */
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
		/*! ���㒸�_ */
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
		/*! �������_ */
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

	/*! �|���S��(�l�p�`�|���S��)�̃o�b�t�@���` */
	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				/*!< GPU����ǂݍ��݂Ə������݂����� */
	bd.ByteWidth = sizeof(vertices);			/*!< �o�b�t�@�̃T�C�Y */
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	/*!< ���_�o�b�t�@�Ƃ��ă����_�����O�p�C�v���C���Ƀo�C���h */

	/*! �T�u���\�[�X�̃f�[�^���` */
	D3D11_SUBRESOURCE_DATA subResourceData;
	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = vertices;			/*!< �������f�[�^�ւ̃|�C���^ */

	HRESULT hr;
	//hr = CreateVertex();
	//if (FAILED(hr)) { ErrorLog("�c�O�ł���"); }

	/*! ���_�o�b�t�@���� */
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

	/*! �g�|���W�[�Z�b�g */
	Direct3D11::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);

	auto shaderData = ShaderManager::GetInstance().GetShaderData(m_szShaderDataUsage);

	/*! ���_�C���v�b�g���C�A�E�g�Z�b�g */
	Direct3D11::GetInstance().GetDeviceContext()->IASetInputLayout(shaderData->m_pVertexLayout);

	/*! �V�F�[�_�[�̓o�^ */
	Direct3D11::GetInstance().GetDeviceContext()->VSSetShader(shaderData->m_pVertexShader, NULL, NULL);
	Direct3D11::GetInstance().GetDeviceContext()->PSSetShader(shaderData->m_pPixelShader, NULL, NULL);

	/*! �R���X�^���g�o�b�t�@�̓o�^ */
	Direct3D11::GetInstance().GetDeviceContext()->VSSetConstantBuffers(0, 1, &shaderData->m_pConstantBuffer);
	Direct3D11::GetInstance().GetDeviceContext()->PSSetConstantBuffers(0, 1, &shaderData->m_pConstantBuffer);


	/*! �e�N�X�`�� */
	Direct3D11::GetInstance().GetDeviceContext()->PSSetSamplers(0, 1, &pSampler);
	Direct3D11::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &pTex);

	/*! ���W�ϊ� */
	DirectX::XMMATRIX mWorld, mTran, mRot, mScale;
	mWorld = DirectX::XMMatrixIdentity();
	mTran = DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	mRot = DirectX::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	mScale = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

	/*! ���[���h�ϊ� */
	mWorld = mScale*mRot*mTran;

	/*! �}�b�s���O�p�ϐ���` */
	D3D11_MAPPED_SUBRESOURCE pData;
	SecureZeroMemory(&pData, sizeof(pData));
	SpriteShaderBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));

	/*! �o�b�t�@�ւ̃A�N�Z�X����(��������) */
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
		Direct3D11::GetInstance().GetDeviceContext()->Unmap(shaderData->m_pConstantBuffer, NULL);/*!< �A�N�Z�X������Ĕ����� */
		return E_FAIL;
	}

	/*! �R���X�^���g�o�b�t�@�Ƀf�[�^�𑗂� */
	auto camera = &Camera::GetInstance();
	DirectX::XMMATRIX m = mWorld*camera->GetViewMatrix()*camera->GetProjMatrix();
	m = DirectX::XMMatrixTranspose(m);	/*!< �]�u�s�� */
	cb.m_WVP = m;						/*!< ���[���h�s�� */



	cb.m_Color = m_Color;
	cb.m_Alpha = m_Alpha;

	/*! �������R�s�[ */
	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

	/*! �A�N�Z�X���I�� */
	Direct3D11::GetInstance().GetDeviceContext()->Unmap(shaderData->m_pConstantBuffer, NULL);

	/*! ���_�o�b�t�@�Z�b�g */
	uint32_t stride = sizeof(SpriteVertex);
	uint32_t offset = 0;
	Direct3D11::GetInstance().GetDeviceContext()->IASetVertexBuffers(
		0,
		1,
		&m_pVertexBuffer,
		&stride,
		&offset
	);

	/*! ���u�����h�̐ݒ� */
	//�����Ă����߂����c
	//uint32_t blendColor= 0xffffffff;
	//Direct3D11::GetInstance().GetDeviceContext()->OMSetBlendState(
	//	m_pBlendState,
	//	NULL,
	//	blendColor
	//);

	/*! ���ߐF */
	//uint32_t blendColor= 0xffffffff;
	//Direct3D11::GetInstance().GetDeviceContext()->OMSetBlendState(
	//	m_pBlendState,
	//	NULL,
	//	blendColor
	//);

	/*! �`�� */
	Direct3D11::GetInstance().GetDeviceContext()->Draw(
		4,		/*!< ���_��(�|���S���Ȃ̂Œ��_����4��) */
		NULL
	);

	/*! ���u�����h���f */
	//Direct3D11::GetInstance().GetDeviceContext()->OMSetBlendState(NULL, NULL, blendColor);

	return S_OK;
}

HRESULT Sprite::CreateVertex()
{
	DirectX::XMFLOAT2 leftTop, rightBottom;

	SpriteVertex vertices[] = {
		/*! �E�� */
		{
			/*! ���_ */
			DirectX::XMFLOAT3(
				rightBottom.x,
				leftTop.y,
				m_Pos.z
			),
			/*! UV���W */
			DirectX::XMFLOAT2(
				1,
				0
			),
		},
		/*! �E�� */
		{
			/*! ���_ */
			DirectX::XMFLOAT3(
				rightBottom.x,
				rightBottom.y,
				m_Pos.z
			),
			/*! UV���W */
			DirectX::XMFLOAT2(
				1,
				1
			),
		},
		/*! ���� */
		{
			/*! ���_ */
			DirectX::XMFLOAT3(
				leftTop.x,
				leftTop.y,
				m_Pos.z
			),
			/*! UV���W */
			DirectX::XMFLOAT2(
				0,
				0
			),
		},
		/*! ���� */
		{
			/*! ���_ */
			DirectX::XMFLOAT3(
				leftTop.x,
				rightBottom.y,
				m_Pos.z
			),
			/*! UV���W */
			DirectX::XMFLOAT2(
				0,
				1
			),

		}
	};

	/*! �|���S��(�l�p�`�|���S��)�̃o�b�t�@���` */
	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				/*!< GPU����ǂݍ��݂Ə������݂����� */
	bd.ByteWidth = sizeof(vertices);			/*!< �o�b�t�@�̃T�C�Y */
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	/*!< ���_�o�b�t�@�Ƃ��ă����_�����O�p�C�v���C���Ƀo�C���h */

												/*! �T�u���\�[�X�̃f�[�^���` */
	D3D11_SUBRESOURCE_DATA subResourceData;
	SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = vertices;			/*!< �������f�[�^�ւ̃|�C���^ */

	HRESULT hr;

	/*! ���_�o�b�t�@���� */
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

