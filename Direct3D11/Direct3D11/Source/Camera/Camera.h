/*
	@file	Camera.h
	@date	2018/04/22
	@author	�ԏ� �G�P
	@brief	�J����
	@detail	�V���O���g��
*/
#pragma once
#include <DirectXMath.h>

/****************************************/
/*			�J�����̕`���(v)			*/
/*		NearClip <= v < FarClip			*/
/*	Ex)									*/
/*	NearClip = 0.1f, FarClip = 100.0f	*/
/*	�`�悳���I�u�W�F�N�g�̓J�����̎�	*/
/*	�_+NearClip����J�����̎��_+FarClip */
/*	�͈̔͂Ɏ��܂��Ă�����̂�`�悷��	*/
/****************************************/
#ifdef _DEBUG
#define DEBUG_CAMERA
#endif // _DEBUG

class Camera
{
public:
	~Camera();
	static Camera& GetInstance() {
		static Camera instance;
		return instance;
	}

	void Initialize(
		DirectX::XMFLOAT3 eyePt,	
		DirectX::XMFLOAT3 lookPt	= {0.0f,0.0f,0.0f},
		DirectX::XMFLOAT3 upVec		= c_UpVector
	);

	/*! �萔 */
	static const double		c_Pi;				/*!< �~������ */
	static const float		c_FieldOfView;		/*!< ��p */
	static const float		c_NearClip;			/*!< �f�t�H���g�N���b�s���O����:�� */
	static const float		c_FarClip;			/*!< �f�t�H���g�N���b�s���O����:�� */
	static const DirectX::XMFLOAT3 c_UpVector;	/*!< �f�t�H���g�̏�����x�N�g�� */

	/*! �Q�b�^�[ */
	DirectX::XMMATRIX GetViewMatrix()const { return m_ViewMat; }
	DirectX::XMMATRIX GetProjMatrix()const { return m_ProjMat; }
	DirectX::XMFLOAT3 GetEyePt()const;
	DirectX::XMFLOAT3 GetLookAtPt()const;

	/*! �Z�b�^�[ */
	void SetFieldOfView(float fov)	{ m_FieldOfView = fov; }
	void SetNearClip(float nearClip){ m_NearClip	= nearClip; }
	void SetFarClip(float farClip)	{ m_FarClip		= farClip; }

private:
	Camera();

	/*! �ϐ� */
	float m_FieldOfView;
	float m_NearClip;
	float m_FarClip;
	DirectX::XMVECTOR m_EyePt;
	DirectX::XMVECTOR m_LookAtPt;
	DirectX::XMVECTOR m_UpVec;
	DirectX::XMMATRIX m_ViewMat;
	DirectX::XMMATRIX m_ProjMat;
};

