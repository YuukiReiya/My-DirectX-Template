#pragma once
#include "../Shader/StructShaderBase.h"
#include "../Shader/ShaderManager.h"
#include <D3D11.h>
#include <fstream>
#include <vector>

class Mesh
{
public:
	Mesh();
	~Mesh();

	HRESULT Initialize();
	void Finalize();
	HRESULT Render();

private:
	DirectX::XMFLOAT3 m_Pos;
	DirectX::XMFLOAT3 m_Rot;
	DirectX::XMFLOAT3 m_Scale;
	std::string m_szShaderDataUsage;
};

/****************************************/
/*		3Dメッシュで扱う構造体			*/
/****************************************/

/*!
	@brief	メッシュの頂点構造体
*/
struct MeshVertex
	:public VARTEX_BASE
{
	MeshVertex(DirectX::XMFLOAT3 pos) {
		m_Pos = pos;
	}
};

/*!
	@brief	メッシュのコンスタントバッファ構造体
*/
struct MeshShaderBuffer
	:CONSTANT_BUFFER_BASE
{
	MeshShaderBuffer() :CONSTANT_BUFFER_BASE() {}
};