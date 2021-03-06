/*
	@file	Wave.h
	@date	2018/02/11
	@author	番場 宥輝
	@brief	Waveサウンド
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

