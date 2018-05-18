/*
	@file	Camera.cpp
	@date	2018/04/22
	@author	�ԏ� �G�P
	@brief	�J����
	@detail	�V���O���g��
*/
#include "Camera.h"
#include "../Direct3D11/Direct3D11.h"

/*!
	@def	�萔�錾
*/
const double Camera::			c_Pi			= 3.14159265358979323846;					/*!< �~������ ��D3DX_PI�Ɠ��l */
const float Camera::			c_FieldOfView	= static_cast<float>(Camera::c_Pi) / 4.0f;	/*!< �f�t�H���g�̎���p(45�x) */
const float Camera::			c_NearClip		= 0.1f;										/*!< �f�t�H���g�̃N���b�s���O����:�� */
const float Camera::			c_FarClip		= 100.0f;									/*!< �f�t�H���g�̃N���b�s���O����:�� */
const DirectX::XMFLOAT3 Camera::c_UpVector		= { 0.0f,1.0f,0.0f };						/*!< �f�t�H���g�̏�����x�N�g�� */

/*!
	@brief	�R���X�g���N�^
*/
Camera::Camera()
{
	/*! �f�t�H���g�ݒ�X�R�[�v */
	{
		m_FieldOfView	= c_FieldOfView;
		m_NearClip		= c_NearClip;
		m_FarClip		= c_FarClip;
		m_EyePt			= DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		m_LookAtPt		= DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		m_UpVec			= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	}
}

/*!
	@brief	�f�X�g���N�^
*/
Camera::~Camera()
{
}

/*!
	@brief		������
	@detail		��������r���[�s��ƃv���W�F�N�V�����s��𐶐�
	@param[in]	���_�ʒu	
	@param[in]	�����_
	@param[in]	������x�N�g��
*/
void Camera::Initialize(DirectX::XMFLOAT3 eyePt, DirectX::XMFLOAT3 lookPt, DirectX::XMFLOAT3 upVec) 
{
	/*! FLOAT3��VECTOR�^�ɕϊ� */
	{
		GetInstance().m_EyePt		= DirectX::XMVectorSet(eyePt.x, eyePt.y, eyePt.z, 0.0f);
		GetInstance().m_LookAtPt	= DirectX::XMVectorSet(lookPt.x, lookPt.y, lookPt.z, 0.0f);
		GetInstance().m_UpVec		= DirectX::XMVectorSet(upVec.x, upVec.y, upVec.z, 0.0f);
	}

	/*! �r���[�s�� */
	GetInstance().m_ViewMat = DirectX::XMMatrixLookAtLH(
		GetInstance().m_EyePt,								/*!< ���_�ʒu */
		GetInstance().m_LookAtPt,							/*!< �����_ */
		GetInstance().m_UpVec								/*!< ��������� */
	);

	/*! �v���W�F�N�V�����s�� */
	GetInstance().m_ProjMat = DirectX::XMMatrixPerspectiveFovLH(
		GetInstance().m_FieldOfView,													/*!< ����p */
		static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT),			/*!< �A�X�y�N�g�� */
		GetInstance().m_NearClip,														/*!< �N���b�s���O����:�� */
		GetInstance().m_FarClip															/*!< �N���b�s���O����:�� */
	);

}