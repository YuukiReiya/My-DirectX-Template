/*
	@file	Texture.h
	@date	2018/04/28
	@author	番場 宥輝
	@brief	テクスチャ
*/
#pragma once
#include <D3D11.h>
#include <string>
#include <DirectXMath.h>

class Texture
{
public:
	explicit Texture();
	~Texture();


	HRESULT Load(std::string filePath, DirectX::XMINT2 size);
	void Destroy();

	void SetSize(DirectX::XMINT2 size) { m_Size = size; }
	ID3D11SamplerState*GetSamplerState() { return m_pSamplerState; }
	ID3D11ShaderResourceView* GetTexture() { return m_pResorceTexture; }
	DirectX::XMINT2 GetSize() { return m_Size; }

	


private:


	DirectX::XMINT2 m_Size;
	//ID3D11Buffer* m_pVertexBuffer;
	ID3D11SamplerState* m_pSamplerState;
	ID3D11ShaderResourceView* m_pResorceTexture;

};