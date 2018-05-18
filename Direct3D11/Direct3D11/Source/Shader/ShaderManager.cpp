#include "../Direct3D11/Direct3D11.h"
#include "ShaderManager.h"
#include "../MemoryLeaks.h"
#include "../MyGame.h"


/*!
	@def	定数宣言
*/
const std::string c_ResourceHierarchy	= "Resource/Shader/";		/*!< シェーダーのリソースがおいてある階層までのパス */
const std::string c_VertexProfile		= "vs";						/*!< 頂点シェーダー */
const std::string c_PixelProfile		= "ps";						/*!< ピクセルシェーダー */
const std::string c_GeometryProfile		= "gs";						/*!< ジオメトリシェーダー */
const std::string c_HullProfile			= "hs";						/*!< ハルシェーダー */
const std::string c_DomainProfile		= "ds";						/*!< ドメインシェーダー */
const std::string c_ComputeProfile		= "cs";						/*!< コンピュートシェーダー */
const int c_ShaderModelStringArraySize	= 3;						/*!< シェーダーモデルの設定文字列(\0含む)の配列のサイズ ※上記Profileの文字列サイズ+終端文字分 */


/*!
	@brief	コンストラクタ
*/
ShaderManager::ShaderManager()
{
	SecureZeroMemory(this, sizeof(this));
}

/*!
	@brief	デストラクタ
*/
ShaderManager::~ShaderManager()
{
}

void ShaderManager::Initialize()
{
}
/*!
	@brief	シェーダーの作成
	@return	成功有無
*/
HRESULT ShaderManager::MakeShader(std::string sFileName, LPSTR szFuncName, LPSTR szProfileName, void ** ppShader, ID3DBlob ** ppBlob)
{
	/*! UNICODE、マルチバイト両対応用文字列変換 */
	std::string sFilePath = c_ResourceHierarchy + sFileName;
	auto tmp = tString(sFilePath);
	const auto path = const_cast<LPTSTR>(tmp.c_str());

	/*! ブロブのコンパイル */
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
	/*! 終端文字を含まないバッファのメモリを変数にコピー */
	memcpy_s(szProfile, sizeof(szProfileName), szProfileName, (c_ShaderModelStringArraySize - 1));

	/*! 頂点シェーダー(Vertex Shader) */
	if (strcmp(szProfile, c_VertexProfile.c_str()) == 0) {
		hr = Direct3D11::GetDevice()->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + sFileName + "\" is not matched \"Vertex Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! ピクセルシェーダー(Pixel Shader) */
	if (strcmp(szProfile, c_PixelProfile.c_str()) == 0) {
		hr = Direct3D11::GetDevice()->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + sFileName + "\" is not matched \"Pixel Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! ジオメトリシェーダー(Geometry Shader) */
	if (strcmp(szProfile, c_GeometryProfile.c_str()) == 0) {
		hr = Direct3D11::GetDevice()->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + sFileName + "\" is not matched \"Geometry Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! ハルシェーダー(Hull Shader) */
	if (strcmp(szProfile, c_HullProfile.c_str()) == 0) {
		hr = Direct3D11::GetDevice()->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + sFileName + "\" is not matched \"Hull Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return true;
	}
	/*! ドメインシェーダー(Domain Shader) */
	if (strcmp(szProfile, c_DomainProfile.c_str()) == 0) {
		hr = Direct3D11::GetDevice()->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + sFileName + "\" is not matched \"Domain Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! コンピュートシェーダー(Compute Shader) */
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
