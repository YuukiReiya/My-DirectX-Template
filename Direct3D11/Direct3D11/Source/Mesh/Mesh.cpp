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


	DirectX::XMMATRIX mWorld, mTran, mRot, mScale;
	mWorld = DirectX::XMMatrixIdentity();/*!< 単位行列 */
	mTran = DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	mRot = DirectX::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	mScale = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

	mWorld = mScale*mRot*mTran;

	/*! マッピング用変数定義 */
	D3D11_MAPPED_SUBRESOURCE pData;
	SecureZeroMemory(&pData, sizeof(pData));
	MeshShaderBuffer cb;
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
	m = DirectX::XMMatrixTranspose(m);/*!< 行列を転置行列にする */
	cb.m_WVP = m;/*!< ワールド行列 */

	/*! メモリコピー */
	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

	/*! アクセス許可終了 */
	Direct3D11::GetInstance().GetDeviceContext()->Unmap(shaderData->m_pConstantBuffer, NULL);

	/*! 描画 */
	Direct3D11::GetInstance().GetDeviceContext()->Draw(
		3,		/*!< 頂点数(板ポリゴンなので頂点数は4つ) */
		NULL
	);

	return S_OK;
}
