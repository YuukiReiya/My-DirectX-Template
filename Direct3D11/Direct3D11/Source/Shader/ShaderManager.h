/*
	@file	ShaderManager.h
	@date	2018/03/04
	@author	�ԏ� �G�P
	@brief	�V�F�[�_�[�֘A
	@detail	vector,map�̒���:http://msty.hatenablog.jp/entry/2016/08/18/144934
*/
#pragma once
#include <map>
#include "../MemoryLeaks.h"
#include <D3D11.h>

/*! 
	@brief	�V�F�[�_�[���\������\����
*/
struct ShaderData
{
	/*! 
		@brief	�R���X�g���N�^ 
	*/
	ShaderData(){
		SecureZeroMemory(this, sizeof(this));
		m_pVertexLayout		= nullptr;
		m_pVertexShader		= nullptr;
		m_pPixelShader		= nullptr;
		m_pConstantBuffer	= nullptr;
	}
	/*!
		@brief	�f�X�g���N�^
	*/
	~ShaderData()
	{
		//if (m_pVertexLayout != nullptr) { m_pVertexLayout->Release(); }
		//m_pVertexLayout = nullptr;
		//if (m_pVertexShader != nullptr) { m_pVertexShader->Release(); }
		//m_pVertexShader = nullptr;
		//if (m_pPixelShader != nullptr) { m_pPixelShader->Release(); }
		//m_pPixelShader = nullptr;
		//if (m_pConstantBuffer != nullptr) { m_pConstantBuffer->Release(); }
		//m_pConstantBuffer = nullptr;

		if (m_pVertexLayout != NULL) { m_pVertexLayout->Release(); }
		m_pVertexLayout = NULL;
		if (m_pVertexShader != NULL) { m_pVertexShader->Release(); }
		m_pVertexShader = NULL;
		if (m_pPixelShader != NULL) { m_pPixelShader->Release(); }
		m_pPixelShader = NULL;
		if (m_pConstantBuffer != NULL) { m_pConstantBuffer->Release(); }
		m_pConstantBuffer = NULL;

	};
	ID3D11InputLayout*	m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader*	m_pPixelShader;
	ID3D11Buffer*		m_pConstantBuffer;
};

class ShaderManager
{
public:
	~ShaderManager();
	static ShaderManager& GetInstance() {
		static ShaderManager instance;
		return instance;
	}


	static const std::string c_SpriteDefault;
	static const std::string c_MeshDefault;

	HRESULT AddNewShaderData(std::string szKeyName,ShaderData* pNewShaderData);	/*!< �\���̂����X�g�ɒǉ��o�^ */
	HRESULT Initialize();
	void Finalize();
	static HRESULT MakeShader(std::string szFileName,LPSTR szFuncName,LPSTR szProfileName,void** ppShader,ID3DBlob** ppBlob);
	ShaderData* GetShaderData(std::string szKeyName);
private:
	ShaderManager();

	ShaderData* m_pAddDataRef;/*!< �f�[�^�ǉ��p�̎Q�ƃ����o */
	std::map<std::string, ShaderData*>m_pShaderDataMap;
};

