/*
	@file	StructShaderBase.h
	@date	2018/03/04
	@author	�ԏ� �G�P
	@brief	�V�F�[�_�[�֘A�̍\���̐錾
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
	@brief	��꒸�_�\���� 
*/
struct VARTEX_BASE
{
	DirectX::XMFLOAT3 m_Pos;
};

/*!
	@brief	�V�F�[�_�[���ɓn���R���X�^���g�o�b�t�@�̊��\����
*/
struct CONSTANT_BUFFER_BASE
{
	/*!
		@brief	�G�C���A�X�e���v���[�g��p�����ʖ���`
	*/
	template<typename alignType>
	using ALIGN16 = __declspec(align(16)) alignType;/*!< 16�o�C�g���E�Ɍ^��ݒ肷�邽�߂̕ʖ� */

	DirectX::XMMATRIX m_WVP;
};