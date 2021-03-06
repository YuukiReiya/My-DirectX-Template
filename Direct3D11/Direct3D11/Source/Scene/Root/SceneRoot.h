/*
	@file	SceneRoot.h
	@date	2018/02/22
	@author	番場 宥輝
	@brief	シーンの遷移クラス
*/
#pragma once
#include "../Scene.h"


class SceneRoot final:
	public ISceneBase
{
public:
	SceneRoot();
	~SceneRoot();

	void		Init();
	ISceneBase*	Update(ISceneBase* scene);
	void		Render();
	void		Destroy();
private:
	Scene*		m_pScene;
};

