#include "../Direct3D11/Direct3D11.h"
#include "ShaderManager.h"
#include "../MemoryLeaks.h"
#include "../MyGame.h"


/*!
	@def	�萔�錾
*/
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
}

/*!
	@brief	�f�X�g���N�^
*/
ShaderManager::~ShaderManager()
{
}

void ShaderManager::Initialize()
{
}
/*!
	@brief	�V�F�[�_�[�̍쐬
	@return	�����L��
*/
HRESULT ShaderManager::MakeShader(std::string sFileName, LPSTR szFuncName, LPSTR szProfileName, void ** ppShader, ID3DBlob ** ppBlob)
{
	/*! UNICODE�A�}���`�o�C�g���Ή��p������ϊ� */
	std::string sFilePath = c_ResourceHierarchy + sFileName;
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
		hr = Direct3D11::GetDevice()->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + sFileName + "\" is not matched \"Vertex Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! �s�N�Z���V�F�[�_�[(Pixel Shader) */
	if (strcmp(szProfile, c_PixelProfile.c_str()) == 0) {
		hr = Direct3D11::GetDevice()->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + sFileName + "\" is not matched \"Pixel Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! �W�I���g���V�F�[�_�[(Geometry Shader) */
	if (strcmp(szProfile, c_GeometryProfile.c_str()) == 0) {
		hr = Direct3D11::GetDevice()->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + sFileName + "\" is not matched \"Geometry Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! �n���V�F�[�_�[(Hull Shader) */
	if (strcmp(szProfile, c_HullProfile.c_str()) == 0) {
		hr = Direct3D11::GetDevice()->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + sFileName + "\" is not matched \"Hull Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return true;
	}
	/*! �h���C���V�F�[�_�[(Domain Shader) */
	if (strcmp(szProfile, c_DomainProfile.c_str()) == 0) {
		hr = Direct3D11::GetDevice()->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + sFileName + "\" is not matched \"Domain Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! �R���s���[�g�V�F�[�_�[(Compute Shader) */
	if (strcmp(szProfile, c_ComputeProfile.c_str()) == 0) {
		hr = Direct3D11::GetDevice()->CreateComputeShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11ComputeShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + sFileName + "\" is not matched \"Compute Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	std::string error = "\"ProfileName\" is not ShaderModel!";
	ErrorLog(error);
	return E_FAIL;
}
