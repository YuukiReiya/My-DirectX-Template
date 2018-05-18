/*
	@file	Scene.h
	@date	2018/02/22
	@author	�ԏ� �G�P
	@brief	�e�V�[���̊��N���X
*/
#pragma once
#include "../Scene/Base/ISceneBase.h"

/*! �O���錾 */
class SceneRoot;

class Scene :
	public ISceneBase
{
public:
	virtual			~Scene();

	ISceneBase*		Update(ISceneBase* scene)final;
	virtual Scene*	Updaete(SceneRoot* root) = 0;
};