/*
	@file	MemoryLeaks.h
	@date	2018/02/11
	@author	�ԏ� �G�P
	@brief	���������[�N����}�N����`�w�b�_�[
*/
#pragma once
#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC 
#define	new	new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif