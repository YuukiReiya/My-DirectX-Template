#include "../Direct3D11/Direct3D11.h"
#include "ShaderManager.h"
#include "../MemoryLeaks.h"
#include "../MyGame.h"
#include "../Sprite/Sprite.h"
#include "../Mesh/Mesh.h"
#include <string>

/*!
	@def	�萔�錾
*/
const std::string ShaderManager::c_SpriteDefault = "SPRITE_DEFAULT";/*!< �X�v���C�g�V�F�[�_�[�̃f�t�H���g�ݒ� */
const std::string ShaderManager::c_MeshDefault	 = "MESH_DEFAULT";	/*!< ���b�V���V�F�[�_�[�̃f�t�H���g�ݒ� */
const std::string c_ResourceHierarchy	= "Resource/Shader/";		/*!< �V�F�[�_�[�̃��\�[�X�������Ă���K�w�܂ł̃p�X */
const std::string c_VertexProfile		= "vs";						/*!< ���_�V�F�[�_�[ */
const std::string c_PixelProfile		= "ps";						/*!< �s�N�Z���V�F�[�_�[ */
const std::string c_GeometryProfile		= "gs";						/*!< �W�I���g���V�F�[�_�[ */
const std::string c_HullProfile			= "hs";						/*!< �n���V�F�[�_�[ */
const std::string c_DomainProfile		= "ds";						/*!< �h���C���V�F�[�_�[ */
const std::string c_ComputeProfile		= "cs";						/*!< �R���s���[�g�V�F�[�_�[ */
const int c_ShaderModelStringArraySize	= 3;						/*!< �V�F�[�_�[���f���̐ݒ蕶����(\0�܂�)�̔z��̃T�C�Y ����LProfile�̕�����T�C�Y+�I�[������ */


/*!
	@brief	�R���X�g���N�^
*/
ShaderManager::ShaderManager()
{
	SecureZeroMemory(this, sizeof(this));
	/*! ZeroMemory��STL�����Ŏg���ϐ�������������̂ŃC���X�^���X���Đ������ĉ������ */
	m_pShaderDataMap = std::map<std::string, ShaderData*>();
}

/*!
	@brief	�f�X�g���N�^
*/
ShaderManager::~ShaderManager()
{
	Finalize();
}

HRESULT ShaderManager::Initialize()
{
	HRESULT hr;
	ID3DBlob* pCompiledShader = NULL;/*!< �R���p�C���p�u���u */

	/*! �X�v���C�g�p�̃f�t�H���g�ݒ�쐬 */
	m_pAddDataRef = new ShaderData;
	{
		/*! �o�[�e�b�N�X�V�F�[�_�[�̍쐬 */
		hr = MakeShader("DefaultSprite.hlsl", "VS", "vs_5_0", (void**)&m_pAddDataRef->m_pVertexShader, &pCompiledShader);
		if (FAILED(hr)) {
			ErrorLog("\"SpriteDefault\" vertex shader is not create!");
			return E_FAIL;
		}

		/*! ���_�C���v�b�g���C�A�E�g��` */
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		uint32_t numElements = sizeof(layout) / sizeof(*layout);/*!< �|�C���^ */

		/*! ���_�C���v�b�g���C�A�E�g���쐬 */
		hr = Direct3D11::GetInstance().GetDevice()->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pAddDataRef->m_pVertexLayout
		);
		if (FAILED(hr)) {
			ErrorLog("\"SpriteDefault\" input layout is not create!");
			return E_FAIL;
		}

		/*! �s�N�Z���V�F�[�_�[�̍쐬 */
		hr = ShaderManager::MakeShader("DefaultSprite.hlsl", "PS", "ps_5_0", (void**)&m_pAddDataRef->m_pPixelShader, &pCompiledShader);
		if (FAILED(hr)) {
			ErrorLog("\"SpriteDefault\" pixel shader is not create!");
			return E_FAIL;
		}

		/*! �R���X�^���g�o�b�t�@��` */
		D3D11_BUFFER_DESC cb;
		SecureZeroMemory(&cb, sizeof(cb));
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(SpriteShaderBuffer);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.Usage = D3D11_USAGE_DYNAMIC;

		/*! �R���X�^���g�o�b�t�@�쐬 */
		hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(&cb, NULL, &m_pAddDataRef->m_pConstantBuffer);
		if (FAILED(hr)) {
			std::string error = "\"SpriteDefault\ ConstantBuffer is not create!";
			ErrorLog(error);
			return E_FAIL;
		}

	}
	AddNewShaderData(c_SpriteDefault, m_pAddDataRef);
	
	/*! �u���u�̉�� */
	pCompiledShader->Release();
	pCompiledShader = NULL;

	/*! ���b�V���p�̃f�t�H���g�ݒ� */
	m_pAddDataRef = new ShaderData;
	{
		/*! �o�[�e�b�N�X�V�F�[�_�[�̍쐬 */
		hr = MakeShader("DefaultMesh.hlsl", "VS", "vs_5_0", (void**)&m_pAddDataRef->m_pVertexShader, &pCompiledShader);
		if (FAILED(hr)) {
			ErrorLog("\"MeshDefault\" vertex shader is not create!");
			return E_FAIL;
		}

		/*! ���_�C���v�b�g���C�A�E�g��` */
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		uint32_t numElements = sizeof(layout) / sizeof(*layout);/*!< �|�C���^ */
		
		/*! ���_�C���v�b�g���C�A�E�g���쐬 */
		hr = Direct3D11::GetInstance().GetDevice()->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pAddDataRef->m_pVertexLayout
		);

		/*! �s�N�Z���V�F�[�_�[�̍쐬 */
		hr = ShaderManager::MakeShader("DefaultMesh.hlsl", "PS", "ps_5_0", (void**)&m_pAddDataRef->m_pPixelShader, &pCompiledShader);
		if (FAILED(hr)) {
			ErrorLog("\"MeshDefault\" pixel shader is not create!");
			return E_FAIL;
		}

		/*! �R���X�^���g�o�b�t�@��` */
		D3D11_BUFFER_DESC cb;
		cb.ByteWidth = sizeof(MeshShaderBuffer);
		cb.Usage = D3D11_USAGE_DYNAMIC;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.StructureByteStride = 0;

		/*! �R���X�^���g�o�b�t�@�쐬 */
		hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(&cb, NULL, &m_pAddDataRef->m_pConstantBuffer);
		if (FAILED(hr)) {
			std::string error = "\"MeshDefault\" ConstantBuffer is not create!";
			ErrorLog(error);
			return E_FAIL;
		}

	}
	AddNewShaderData(c_MeshDefault, m_pAddDataRef);

	return S_OK;
}

/*!
	@brief	�I������
*/
void ShaderManager::Finalize()
{
	for (auto it : m_pShaderDataMap) {
		delete it.second;
	}
	m_pShaderDataMap.clear();
}


/*!
	@brief	�V���ȃV�F�[�_�[�f�[�^���N���X�ɓo�^����
	@detail	�e�����o�ϐ��̏������͍s��Ȃ��̂Ŋe���ŏ������y�ѐ������Ă���
*/
HRESULT ShaderManager::AddNewShaderData(std::string szKeyName, ShaderData * pNewShaderData)
{
	/*! �o�^�\������ */
	bool isFound = m_pShaderDataMap.find(szKeyName) == m_pShaderDataMap.end();

	/*! �o�^���悤�Ƃ����L�[���͊��ɓo�^�ς݂̂��ߒǉ����Ȃ� */
	if (!isFound) { return E_FAIL; }

	/*! �L�[���Ń��X�g(map)�ɒǉ� */
	m_pShaderDataMap[szKeyName] = pNewShaderData;

	return S_OK;
}

/*!
	@brief	�V�F�[�_�[�̍쐬
	@return	�����L��
*/
HRESULT ShaderManager::MakeShader(std::string szFileName, LPSTR szFuncName, LPSTR szProfileName, void ** ppShader, ID3DBlob ** ppBlob)
{
	/*! UNICODE�A�}���`�o�C�g���Ή��p������ϊ� */
	std::string sFilePath = c_ResourceHierarchy + szFileName;
	auto tmp = tString(sFilePath);
	const auto path = const_cast<LPTSTR>(tmp.c_str());

	/*! �u���u�̃R���p�C�� */
	ID3DBlob* pErrors = NULL;
	HRESULT hr = D3DX11CompileFromFile(path, NULL, NULL, szFuncName, szProfileName, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, ppBlob, &pErrors, NULL);
	if (FAILED(hr)) {
		std::string bufferError = (char*)pErrors->GetBufferPointer();
		std::string error = "\"" + sFilePath + "\" is not compile from file!\n" + bufferError;
		ErrorLog(error);
		return E_FAIL;
	}
	if (pErrors != NULL) {
		pErrors->Release();
		pErrors = NULL;
	}
	char szProfile[c_ShaderModelStringArraySize] = { 0 };
	/*! �I�[�������܂܂Ȃ��o�b�t�@�̃�������ϐ��ɃR�s�[ */
	memcpy_s(szProfile, sizeof(szProfileName), szProfileName, (c_ShaderModelStringArraySize - 1));

	/*! ���_�V�F�[�_�[(Vertex Shader) */
	if (strcmp(szProfile, c_VertexProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Vertex Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! �s�N�Z���V�F�[�_�[(Pixel Shader) */
	if (strcmp(szProfile, c_PixelProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Pixel Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! �W�I���g���V�F�[�_�[(Geometry Shader) */
	if (strcmp(szProfile, c_GeometryProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Geometry Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! �n���V�F�[�_�[(Hull Shader) */
	if (strcmp(szProfile, c_HullProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Hull Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return true;
	}
	/*! �h���C���V�F�[�_�[(Domain Shader) */
	if (strcmp(szProfile, c_DomainProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Domain Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! �R���s���[�g�V�F�[�_�[(Compute Shader) */
	if (strcmp(szProfile, c_ComputeProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateComputeShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11ComputeShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Compute Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	std::string error = "\"ProfileName\" is not ShaderModel!";
	ErrorLog(error);
	return E_FAIL;
}

/*!
	@brief	�L�[(������)����o�^�ς݂̃V�F�[�_�[�����擾����
	@detail	�T�������񂩂�����擾���邱�ƂɎ��s�����ꍇNULL��Ԃ�
*/
ShaderData * ShaderManager::GetShaderData(std::string szKeyName)
{
	return m_pShaderDataMap[szKeyName];
}
