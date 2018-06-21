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
/*		3D���b�V���ň����\����			*/
/****************************************/

/*!
	@brief	���b�V���̒��_�\����
*/
struct MeshVertex
	:public VARTEX_BASE
{
	MeshVertex(DirectX::XMFLOAT3 pos) {
		m_Pos = pos;
	}
};

/*!
	@brief	���b�V���̃R���X�^���g�o�b�t�@�\����
*/
struct MeshShaderBuffer
	:CONSTANT_BUFFER_BASE
{
	MeshShaderBuffer() :CONSTANT_BUFFER_BASE() {}
};