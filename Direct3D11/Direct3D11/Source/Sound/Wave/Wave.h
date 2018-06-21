/*
	@file	Wave.h
	@date	2018/02/11
	@author	�ԏ� �G�P
	@brief	Wave�T�E���h
*/
#pragma once
#include <string>
#include <XAudio2.h>


class Wave
{
public:
	explicit Wave();
	~Wave();

	bool Load(std::string filePath);
	void Release();
	void Play(bool isLoop = false);
	void Stop();
private:
	IXAudio2SourceVoice* m_pSourceVoice;
	BYTE* m_pWaveBuffer;
	DWORD m_dwWaveSize;
};

