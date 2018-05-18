/*
	@file	Scene.h
	@date	2018/02/22
	@author	番場 宥輝
	@brief	各シーンの基底クラス
*/
#pragma once
#include "../Scene/Base/ISceneBase.h"

/*! 前方宣言 */
class SceneRoot;

class Scene :
	public ISceneBase
{
public:
	virtual			~Scene();

	ISceneBase*		Update(ISceneBase* scene)final;
	virtual Scene*	Updaete(SceneRoot* root) = 0;
};