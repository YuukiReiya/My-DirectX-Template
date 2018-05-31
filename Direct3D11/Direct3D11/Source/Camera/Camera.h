/*
	@file	Camera.h
	@date	2018/04/22
	@author	番場 宥輝
	@brief	カメラ
	@detail	シングルトン
*/
#pragma once
#include <DirectXMath.h>

/****************************************/
/*			カメラの描画域(v)			*/
/*		NearClip <= v < FarClip			*/
/*	Ex)									*/
/*	NearClip = 0.1f, FarClip = 100.0f	*/
/*	描画されるオブジェクトはカメラの視	*/
/*	点+NearClipからカメラの視点+FarClip */
/*	の範囲に収まっているものを描画する	*/
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

	/*! 定数 */
	static const double c_Pi;
	static const float c_FieldOfView;
	static const float c_NearClip;
	static const float c_FarClip;
	static const DirectX::XMFLOAT3 c_UpVector;

	/*! ゲッター */
	DirectX::XMMATRIX GetViewMatrix()const { return GetInstance().m_ViewMat; }
	DirectX::XMMATRIX GetProjMatrix()const { return GetInstance().m_ProjMat; }
	DirectX::XMFLOAT3 GetEyePt()const;
	DirectX::XMFLOAT3 GetLookAtPt()const;

	/*! セッター */
	void SetFieldOfView(float fov)	{ m_FieldOfView = fov; }
	void SetNearClip(float nearClip) { m_NearClip	  = nearClip; }
	void SetFarClip(float farClip)	{ m_FarClip	  = farClip; }

private:
	Camera();

	/*! 変数 */
	float m_FieldOfView;
	float m_NearClip;
	float m_FarClip;
	DirectX::XMVECTOR m_EyePt;
	DirectX::XMVECTOR m_LookAtPt;
	DirectX::XMVECTOR m_UpVec;
	DirectX::XMMATRIX m_ViewMat;
	DirectX::XMMATRIX m_ProjMat;
};

