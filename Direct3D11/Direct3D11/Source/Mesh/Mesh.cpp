#include "Mesh.h"
#include "../Direct3D11/Direct3D11.h"
#include "../Camera/Camera.h"
#include "../MyGame.h"
#include "../MemoryLeaks.h"

Mesh::Mesh()
{
	m_szShaderDataUsage = ShaderManager::c_MeshDefault;
}


Mesh::~Mesh()
{
}


HRESULT Mesh::Initialize()
{

	return E_NOTIMPL;
}

HRESULT Mesh::Render()
{
	HRESULT hr;


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


	DirectX::XMMATRIX mWorld, mTran, mRot, mScale;
	mWorld = DirectX::XMMatrixIdentity();/*!< �P�ʍs�� */
	mTran = DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	mRot = DirectX::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	mScale = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

	mWorld = mScale*mRot*mTran;

	/*! �}�b�s���O�p�ϐ���` */
	D3D11_MAPPED_SUBRESOURCE pData;
	SecureZeroMemory(&pData, sizeof(pData));
	MeshShaderBuffer cb;
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
	m = DirectX::XMMatrixTranspose(m);/*!< �s���]�u�s��ɂ��� */
	cb.m_WVP = m;/*!< ���[���h�s�� */

	/*! �������R�s�[ */
	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

	/*! �A�N�Z�X���I�� */
	Direct3D11::GetInstance().GetDeviceContext()->Unmap(shaderData->m_pConstantBuffer, NULL);

	/*! �`�� */
	Direct3D11::GetInstance().GetDeviceContext()->Draw(
		3,		/*!< ���_��(�|���S���Ȃ̂Œ��_����4��) */
		NULL
	);

	return S_OK;
}
