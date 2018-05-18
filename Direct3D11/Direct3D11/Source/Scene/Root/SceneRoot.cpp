/*
	@file	SceneRoot.cpp
	@date	2018/02/22
	@author	番場 宥輝
	@brief	シーンの遷移クラス
*/
#include "SceneRoot.h"


/*!
	@brief	コンストラクタ
*/
SceneRoot::SceneRoot()
{
}

/*!
	@brief	デストラクタ
*/
SceneRoot::~SceneRoot()
{
}

/*!
	@brief	初期化
*/
void SceneRoot::Init()
{
}

/*!
	@brief	更新
	@return	シーンのポインタ
*/
ISceneBase * SceneRoot::Update(ISceneBase * scene)
{
	return nullptr;
}

/*!
	@brief	描画
*/
void SceneRoot::Render()
{
}

/*!
	@brief	破棄
*/
void SceneRoot::Destroy()
{
}
