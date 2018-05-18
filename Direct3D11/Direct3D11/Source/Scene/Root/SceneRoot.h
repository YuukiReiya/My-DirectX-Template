/*
	@file	SceneRoot.h
	@date	2018/02/22
	@author	”Ôê —G‹P
	@brief	ƒV[ƒ“‚Ì‘JˆÚƒNƒ‰ƒX
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

