/*
	@file	StructShaderBase.h
	@date	2018/03/04
	@author	番場 宥輝
	@brief	シェーダー関連の構造体宣言
*/
//#pragma once
//#pragma comment(lib,"d3dx10.lib")
//#pragma comment(lib,"d3dCompiler.lib")
///*
//*/
//#include <D3Dcompiler.h>
//
////#include <D3DX10.h>
//#include <D3D10_1.h>
//#include <D3DX10math.h>

#include <DirectXMath.h>


/*!
	@brief	基底頂点構造体 
*/
struct VARTEX_BASE
{
	DirectX::XMFLOAT3 m_Pos;
};

/*!
	@brief	シェーダー側に渡すコンスタントバッファの基底構造体
*/
struct CONSTANT_BUFFER_BASE
{
	/*!
		@brief	エイリアステンプレートを用いた別名定義
	*/
	template<typename alignType>
	using ALIGN16 = __declspec(align(16)) alignType;/*!< 16バイト境界に型を設定するための別名 */

	DirectX::XMMATRIX m_WVP;
};