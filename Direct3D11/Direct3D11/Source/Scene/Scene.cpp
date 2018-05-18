/*
	@file	Scene.cpp
	@date	2018/02/22
	@author	番場 宥輝
	@brief	各シーンの基底クラス
*/
#include "Scene.h"
#include "Root/SceneRoot.h"

/*!
	@brief	デストラクタ
*/
Scene::~Scene()
{
}

/*!
	@brief	更新
*/
ISceneBase * Scene::Update(ISceneBase * scene)
{
	SceneRoot* root = dynamic_cast<SceneRoot*>(scene);
	return Update(root);
}