/*
	@file	Wave.h
	@date	2018/02/11
	@author	î‘èÍ óGãP
	@brief	WaveÉTÉEÉìÉh
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

