/*
	@file	Camera.h
	@date	2018/04/22
	@author	�ԏ� �G�P
	@brief	�J����
	@detail	�V���O���g��
*/
#pragma once
#include <DirectXMath.h>

class Camera
{
public:
	~Camera();

	static void Initialize(
		DirectX::XMFLOAT3 eyePt,	
		DirectX::XMFLOAT3 lookPt	= {0.0f,0.0f,0.0f},
		DirectX::XMFLOAT3 upVec		= c_UpVector
	);

	/*! �萔 */
	static const double c_Pi;
	static const float c_FieldOfView;
	static const float c_NearClip;
	static const float c_FarClip;
	static const DirectX::XMFLOAT3 c_UpVector;

	/*! �Q�b�^�[ */
	static DirectX::XMMATRIX GetViewMatrix() { return GetInstance().m_ViewMat; }
	static DirectX::XMMATRIX GetProjMatrix() { return GetInstance().m_ProjMat; }


	/*! �Z�b�^�[ */
	static void SetFieldOfView(float fov)	{ GetInstance().m_FieldOfView = fov; }
	static void SetNearClip(float nearClip) { GetInstance().m_NearClip	  = nearClip; }
	static void SetFarClip(float farClip)	{ GetInstance().m_FarClip	  = farClip; }
private:
	Camera();
	static Camera& GetInstance() {
		static Camera instance;
		return instance;
	}

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

