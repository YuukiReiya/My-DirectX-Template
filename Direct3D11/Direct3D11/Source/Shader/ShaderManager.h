/*
	@file	ShaderManager.h
	@date	2018/03/04
	@author	番場 宥輝
	@brief	シェーダー関連
*/
#pragma once
//#include <string>
//#include <Windows.h>
//#include <D3Dcommon.h>
#include <map>

class ShaderManager
{
public:
	~ShaderManager();
	static ShaderManager& GetInstance() {
		static ShaderManager instance;
		return instance;
	}
	void Initialize();
	static HRESULT MakeShader(std::string sFileName,LPSTR szFuncName,LPSTR szProfileName,void** ppShader,ID3DBlob** ppBlob);
private:
	ShaderManager();

	std::map<std::string, ID3D11InputLayout*>	m_pVertexLayoutes;
	std::map<std::string, ID3D11VertexShader*>	m_pVertexShaderes;
	std::map<std::string, ID3D11PixelShader*>	m_pPixelShaderes;
	std::map<std::string, ID3D11Buffer*>		m_pConstantBufferes;
};

