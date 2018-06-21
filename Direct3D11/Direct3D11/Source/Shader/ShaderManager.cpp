#include "../Direct3D11/Direct3D11.h"
#include "ShaderManager.h"
#include "../MemoryLeaks.h"
#include "../MyGame.h"
#include "../Sprite/Sprite.h"
#include "../Mesh/Mesh.h"
#include <string>

/*!
	@def	定数宣言
*/
const std::string ShaderManager::c_SpriteDefault = "SPRITE_DEFAULT";/*!< スプライトシェーダーのデフォルト設定 */
const std::string ShaderManager::c_MeshDefault	 = "MESH_DEFAULT";	/*!< メッシュシェーダーのデフォルト設定 */
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
	/*! ZeroMemoryはSTL内部で使う変数も初期化するのでインスタンスを再生成して回避する */
	m_pShaderDataMap = std::map<std::string, ShaderData*>();
}

/*!
	@brief	デストラクタ
*/
ShaderManager::~ShaderManager()
{
	Finalize();
}

HRESULT ShaderManager::Initialize()
{
	HRESULT hr;
	ID3DBlob* pCompiledShader = NULL;/*!< コンパイル用ブロブ */

	/*! スプライト用のデフォルト設定作成 */
	m_pAddDataRef = new ShaderData;
	{
		/*! バーテックスシェーダーの作成 */
		hr = MakeShader("DefaultSprite.hlsl", "VS", "vs_5_0", (void**)&m_pAddDataRef->m_pVertexShader, &pCompiledShader);
		if (FAILED(hr)) {
			ErrorLog("\"SpriteDefault\" vertex shader is not create!");
			return E_FAIL;
		}

		/*! 頂点インプットレイアウト定義 */
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		uint32_t numElements = sizeof(layout) / sizeof(*layout);/*!< ポインタ */

		/*! 頂点インプットレイアウトを作成 */
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

		/*! ピクセルシェーダーの作成 */
		hr = ShaderManager::MakeShader("DefaultSprite.hlsl", "PS", "ps_5_0", (void**)&m_pAddDataRef->m_pPixelShader, &pCompiledShader);
		if (FAILED(hr)) {
			ErrorLog("\"SpriteDefault\" pixel shader is not create!");
			return E_FAIL;
		}

		/*! コンスタントバッファ定義 */
		D3D11_BUFFER_DESC cb;
		SecureZeroMemory(&cb, sizeof(cb));
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(SpriteShaderBuffer);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.Usage = D3D11_USAGE_DYNAMIC;

		/*! コンスタントバッファ作成 */
		hr = Direct3D11::GetInstance().GetDevice()->CreateBuffer(&cb, NULL, &m_pAddDataRef->m_pConstantBuffer);
		if (FAILED(hr)) {
			std::string error = "\"SpriteDefault\ ConstantBuffer is not create!";
			ErrorLog(error);
			return E_FAIL;
		}

	}
	AddNewShaderData(c_SpriteDefault, m_pAddDataRef);
	
	/*! ブロブの解放 */
	pCompiledShader->Release();
	pCompiledShader = NULL;

	/*! メッシュ用のデフォルト設定 */
	m_pAddDataRef = new ShaderData;
	{
		/*! バーテックスシェーダーの作成 */
		hr = MakeShader("DefaultMesh.hlsl", "VS", "vs_5_0", (void**)&m_pAddDataRef->m_pVertexShader, &pCompiledShader);
		if (FAILED(hr)) {
			ErrorLog("\"MeshDefault\" vertex shader is not create!");
			return E_FAIL;
		}

		/*! 頂点インプットレイアウト定義 */
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		uint32_t numElements = sizeof(layout) / sizeof(*layout);/*!< ポインタ */
		
		/*! 頂点インプットレイアウトを作成 */
		hr = Direct3D11::GetInstance().GetDevice()->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pAddDataRef->m_pVertexLayout
		);

		/*! ピクセルシェーダーの作成 */
		hr = ShaderManager::MakeShader("DefaultMesh.hlsl", "PS", "ps_5_0", (void**)&m_pAddDataRef->m_pPixelShader, &pCompiledShader);
		if (FAILED(hr)) {
			ErrorLog("\"MeshDefault\" pixel shader is not create!");
			return E_FAIL;
		}

		/*! コンスタントバッファ定義 */
		D3D11_BUFFER_DESC cb;
		cb.ByteWidth = sizeof(MeshShaderBuffer);
		cb.Usage = D3D11_USAGE_DYNAMIC;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.StructureByteStride = 0;

		/*! コンスタントバッファ作成 */
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
	@brief	終了処理
*/
void ShaderManager::Finalize()
{
	for (auto it : m_pShaderDataMap) {
		delete it.second;
	}
	m_pShaderDataMap.clear();
}


/*!
	@brief	新たなシェーダーデータをクラスに登録する
	@detail	各メンバ変数の初期化は行わないので各自で初期化及び生成しておく
*/
HRESULT ShaderManager::AddNewShaderData(std::string szKeyName, ShaderData * pNewShaderData)
{
	/*! 登録可能か判定 */
	bool isFound = m_pShaderDataMap.find(szKeyName) == m_pShaderDataMap.end();

	/*! 登録しようとしたキー名は既に登録済みのため追加しない */
	if (!isFound) { return E_FAIL; }

	/*! キー名でリスト(map)に追加 */
	m_pShaderDataMap[szKeyName] = pNewShaderData;

	return S_OK;
}

/*!
	@brief	シェーダーの作成
	@return	成功有無
*/
HRESULT ShaderManager::MakeShader(std::string szFileName, LPSTR szFuncName, LPSTR szProfileName, void ** ppShader, ID3DBlob ** ppBlob)
{
	/*! UNICODE、マルチバイト両対応用文字列変換 */
	std::string sFilePath = c_ResourceHierarchy + szFileName;
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
		hr = Direct3D11::GetInstance().GetDevice()->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Vertex Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! ピクセルシェーダー(Pixel Shader) */
	if (strcmp(szProfile, c_PixelProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Pixel Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! ジオメトリシェーダー(Geometry Shader) */
	if (strcmp(szProfile, c_GeometryProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Geometry Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! ハルシェーダー(Hull Shader) */
	if (strcmp(szProfile, c_HullProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Hull Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return true;
	}
	/*! ドメインシェーダー(Domain Shader) */
	if (strcmp(szProfile, c_DomainProfile.c_str()) == 0) {
		hr = Direct3D11::GetInstance().GetDevice()->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader);
		if (FAILED(hr)) {
			std::string error = "\"" + szFileName + "\" is not matched \"Domain Shader\"!";
			ErrorLog(error);
			return E_FAIL;
		}
		else return S_OK;
	}
	/*! コンピュートシェーダー(Compute Shader) */
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
	@brief	キー(文字列)から登録済みのシェーダー情報を取得する
	@detail	探索文字列から情報を取得することに失敗した場合NULLを返す
*/
ShaderData * ShaderManager::GetShaderData(std::string szKeyName)
{
	return m_pShaderDataMap[szKeyName];
}
