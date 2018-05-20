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

	/*! ���_��` */
	//SpriteVertex vertices[] =
	//{
	//	/*! �������_ */
	//	{
	//		DirectX::XMFLOAT3(-0.5,-0.5,1),
	//		DirectX::XMFLOAT2(0,1)
	//	},

	//	/*! ���㒸�_ */
	//	{
	//		DirectX::XMFLOAT3(-0.5, 0.5,1),
	//		DirectX::XMFLOAT2(0,0)
	//	},

	//	/*! �E�����_ */
	//	{
	//		DirectX::XMFLOAT3(0.5,-0.5,1),
	//		DirectX::XMFLOAT2(1,1)
	//	},

	//	/*! �E�㒸�_ */
	//	{
	//		DirectX::XMFLOAT3(0.5, 0.5,1),
	//		DirectX::XMFLOAT2(1,0)
	//	},
	//};

	/*! �|���S��(�l�p�`�|���S��)�̃o�b�t�@���` */
	//D3D11_BUFFER_DESC bd;
	//SecureZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DEFAULT;				/*!< GPU����ǂݍ��݂Ə������݂����� */
	//bd.ByteWidth = sizeof(vertices);			/*!< �o�b�t�@�̃T�C�Y */
	//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	/*!< ���_�o�b�t�@�Ƃ��ă����_�����O�p�C�v���C���Ƀo�C���h */

	/*! �T�u���\�[�X�̃f�[�^���` */
	//D3D11_SUBRESOURCE_DATA subResourceData;
	//SecureZeroMemory(&subResourceData, sizeof(subResourceData));
	//subResourceData.pSysMem = vertices;			/*!< �������f�[�^�ւ̃|�C���^ */


	/*! ���_�o�b�t�@���� */
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

	/*! �o�[�e�b�N�X�o�b�t�@��ݒ� */
	//uint32_t stride = sizeof(SpriteVertex);
	//uint32_t offset = 0;
	//Direct3D11::GetDeviceContext()->IASetVertexBuffers(
	//	0,
	//	1,
	//	&m_pVertexBuffer,
	//	&stride,
	//	&offset
	//);

	///*! �e�N�X�`���T���v���[���` */
	//D3D11_SAMPLER_DESC sd;
	//SecureZeroMemory(&sd, sizeof(sd));
	//sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	///*! �e�N�X�`���T���v���[���쐬 */
	//hr = Direct3D11::GetDevice()->CreateSamplerState(
	//	&sd,
	//	&m_pSampleLinear
	//);
	//if (FAILED(hr)) { ErrorLog("da"); }

	///*! �}���`�o�C�g�E���C�h�����̗��Ή������邽�߂ɕϊ� */
	//std::string filePath = "Sprite.jpg";
	//auto s_path = tString(filePath);
	//const auto path = const_cast<LPTSTR>(s_path.c_str());

	///*! �e�N�X�`���쐬 */
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

	ID3DBlob* pCompiledShader = NULL;/*!< �R���p�C���p�u���u */

	/*! �o�[�e�b�N�X�V�F�[�_�[�̍쐬 */
	hr = ShaderManager::MakeShader("SimpleTexture.hlsl", "VS", "vs_5_0", (void**)&m_pVertexShader, &pCompiledShader);
	if (FAILED(hr)) {
		return E_FAIL;
	}

	/*! ���_�C���v�b�g���C�A�E�g��` */
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	uint32_t numElements = sizeof(layout) / sizeof(layout[0]);/*!< �|�C���^ */

	/*! ���_�C���v�b�g���C�A�E�g���쐬 */
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

	/*! �s�N�Z���V�F�[�_�[�̍쐬 */
	hr = ShaderManager::MakeShader("SimpleTexture.hlsl", "PS", "ps_5_0", (void**)&m_pPixelShader, &pCompiledShader);
	if (FAILED(hr)) {
		return E_FAIL;
	}
	
	if (pCompiledShader != nullptr) {
		pCompiledShader->Release();
	}

	/*! �R���X�^���g�o�b�t�@��` */
	D3D11_BUFFER_DESC cb;
	SecureZeroMemory(&cb, sizeof(cb));
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SpriteShaderBuffer);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	/*! �R���X�^���g�o�b�t�@�쐬 */
	hr = Direct3D11::GetDevice()->CreateBuffer(&cb, NULL, &m_pConstantBuffer);
	if (FAILED(hr)) {
		std::string error = "ConstantBuffer is not create!";
		ErrorLog(error);
		return E_FAIL;
	}

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
	@brief	�`��
	@detail	�X�v���C�g�̒��S�͒��S���W
*/
HRESULT Sprite::Render(Texture * texture, bool isReverse)
{
	/*! �e�N�X�`���f�[�^�̎擾 */
	auto size = texture->GetSize();		/*!< �e�N�X�`���T�C�Y */
	auto pSampler = texture->GetSamplerState();
	auto pTex = texture->GetTexture();	/*!< �e�N�X�`���f�[�^ */

	auto large = size.x < size.y ? size.y : size.x;

	/*! �|���S���̍��W */
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

	/*! UV(�e�N�X�`��)���W */
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
	/*				 ������					*/
	/****************************************/
	/*
		�e�N�X�`���͍���(0,0)
		���_���W�͒����(0,0)
		�����:���A����:��
	*/


	/*! ���_��` */
	SpriteVertex vertices[] =
	{
		/*! �E�㒸�_ */
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
		/*! �E�����_ */
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
		/*! ���㒸�_ */
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
		/*! �������_ */
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

	/*! �g�|���W�[�Z�b�g */
	Direct3D11::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);

	/*! ���_�C���v�b�g���C�A�E�g�Z�b�g */
	Direct3D11::GetDeviceContext()->IASetInputLayout(m_pVertexLayout);

	/*! �V�F�[�_�[�̓o�^ */
	Direct3D11::GetDeviceContext()->VSSetShader(m_pVertexShader, NULL, NULL);
	Direct3D11::GetDeviceContext()->PSSetShader(m_pPixelShader, NULL, NULL);

	/*! �R���X�^���g�o�b�t�@�̓o�^ */
	Direct3D11::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	Direct3D11::GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);


	/*! �e�N�X�`�� */
	Direct3D11::GetDeviceContext()->PSSetSamplers(0, 1, &pSampler);
	Direct3D11::GetDeviceContext()->PSSetShaderResources(0, 1, &pTex);

	/*! ���W�ϊ� */
	DirectX::XMMATRIX mWorld, mTran, mRot, mScale;
	mWorld = DirectX::XMMatrixIdentity();
	mTran = DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	mRot = DirectX::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	mScale = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

	mWorld = mScale*mRot*mTran;

	/*! �}�b�s���O�p�ϐ���` */
	D3D11_MAPPED_SUBRESOURCE pData;
	SecureZeroMemory(&pData, sizeof(pData));
	SpriteShaderBuffer cb;
	SecureZeroMemory(&cb, sizeof(cb));

	/*! �o�b�t�@�ւ̃A�N�Z�X����(��������) */
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
		Direct3D11::GetDeviceContext()->Unmap(m_pConstantBuffer, NULL);/*!< �A�N�Z�X������Ĕ����� */
		return E_FAIL;
	}

	/*! �R���X�^���g�o�b�t�@�Ƀf�[�^�𑗂� */
	DirectX::XMMATRIX m = mWorld*Camera::GetViewMatrix()*Camera::GetProjMatrix();
	m = DirectX::XMMatrixTranspose(m);/*!< �s���]�u�s��ɂ��� */
	cb.m_WVP = m;/*!< ���[���h�s�� */
	cb.m_Color = m_Color;
	cb.m_Alpha = m_Alpha;

	/*! �������R�s�[ */
	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

	/*! �A�N�Z�X���I�� */
	Direct3D11::GetDeviceContext()->Unmap(m_pConstantBuffer, NULL);

	/*! ���_�o�b�t�@�Z�b�g */
	uint32_t stride = sizeof(SpriteVertex);
	uint32_t offset = 0;
	Direct3D11::GetDeviceContext()->IASetVertexBuffers(
		0,
		1,
		&m_pVertexBuffer,
		&stride,
		&offset
	);

	/*! ���u�����h�̐ݒ� */
	//�����Ă����߂����c
	//uint32_t blendColor= 0xffffffff;
	//Direct3D11::GetDeviceContext()->OMSetBlendState(
	//	m_pBlendState,
	//	NULL,
	//	blendColor
	//);

	/*! ���ߐF */
	//uint32_t blendColor= 0xffffffff;
	//Direct3D11::GetDeviceContext()->OMSetBlendState(
	//	m_pBlendState,
	//	NULL,
	//	blendColor
	//);

	/*! �`�� */
	Direct3D11::GetDeviceContext()->Draw(
		4,		/*!< ���_��(�|���S���Ȃ̂Œ��_����4��) */
		NULL
	);

	/*! ���u�����h���f */
	//Direct3D11::GetDeviceContext()->OMSetBlendState(NULL, NULL, blendColor);

	return S_OK;
}

