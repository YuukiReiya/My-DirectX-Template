/*
	@file	Wave.h
	@date	2018/02/11
	@author	番場 宥輝
	@brief	Waveサウンド
*/
#include "Wave.h"
#include "../../MemoryLeaks.h"
#include "../../MyGame.h"
#include "../SoundManager/SoundManager.h"


/*!
	@brief	コンストラクタ
*/
Wave::Wave()
{
	SecureZeroMemory(this, sizeof(Wave));
}

/*!
	@brief	デストラクタ
*/
Wave::~Wave()
{
	Release();
}

/*!
	@brief	Waveファイル読み込み
*/
bool Wave::Load(std::string filePath)
{
	HMMIO hMmio = NULL;				/*!< WindowsマルチメディアAPIハンドル */
	DWORD waveSize = 0;				/*!< Waveデータサイズ */

	WAVEFORMATEX*	pFormat;		/*!< Waveフォーマット */
	MMCKINFO		chunkInfo;		/*!< チャンク情報 */
	MMCKINFO		riffChunkInfo;	/*!< 最上部チャンク */
	PCMWAVEFORMAT	pcmFormat;		/*!< PCMフォーマット */

	/*! Waveファイル内のヘッダー情報読み込み */
	auto s_path = tString(filePath);
	const auto path = const_cast<LPTSTR>(s_path.c_str());
	hMmio = mmioOpen(path, NULL, MMIO_ALLOCBUF | MMIO_READ);

	/*! Waveファイルの読み込み失敗 */
	if (hMmio == NULL) {
		std::string error = "\"" + filePath + "\" is not load in sound!";
		ErrorLog(error);
		return false;
	}

	/*! ファイルポインタをRIFFチャンクの先頭にセット */
	mmioDescend(hMmio, &riffChunkInfo, NULL, 0);

	/*! ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする */
	chunkInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hMmio, &chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK);

	/*! フォーマット読み込み */
	mmioRead(hMmio, (HPSTR)&pcmFormat, sizeof(pcmFormat));
	pFormat = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
	memcpy(pFormat, &pcmFormat, sizeof(pcmFormat));
	pFormat->cbSize = 0;
	mmioAscend(hMmio, &chunkInfo, 0);

	/*! 音データ読み込み */
	chunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hMmio, &chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK);
	waveSize = chunkInfo.cksize;
	m_pWaveBuffer = new BYTE[waveSize];
	DWORD offset = chunkInfo.dwDataOffset;
	mmioRead(hMmio, (char*)m_pWaveBuffer, waveSize);

	/*! ソースボイスにデータ詰め込み */
	XAUDIO2_SEND_DESCRIPTOR sendDescriptor;
	sendDescriptor.Flags = XAUDIO2_SEND_USEFILTER;
	sendDescriptor.pOutputVoice = SoundManager::GetMasterVoice();
	const XAUDIO2_VOICE_SENDS sendList = { 1,&sendDescriptor };

	/*! ソースボイス作成 */
	HRESULT hr = SoundManager::GetXAudio2()->CreateSourceVoice(&m_pSourceVoice, pFormat, 0, 2.0f, NULL, &sendList);


	SAFE_DELETE(pFormat);

	if (FAILED(hr)) {
		ErrorLog("SourceVoice is not create!");
		return false;
	}
	m_dwWaveSize = waveSize;

	return true;
}

/*!
	@brief	解放
*/
void Wave::Release()
{
	m_pSourceVoice->DestroyVoice();
	SAFE_DELETE(m_pWaveBuffer);
}

/*!
	@brief	再生
*/
void Wave::Play(bool isLoop)
{
	//サブミット
	XAUDIO2_BUFFER buffer = { 0 };
	SecureZeroMemory(&buffer, sizeof(buffer));
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = m_dwWaveSize;
	buffer.pAudioData = m_pWaveBuffer;
	buffer.PlayBegin = 0;	/*!< 再生されるバッファの最初のサンプル */
	buffer.PlayLength = 0;	/*!< サウンドのバッファ全て */
	if (isLoop) {

		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	}
	else {
		buffer.LoopBegin = XAUDIO2_NO_LOOP_REGION;
		buffer.LoopCount = 0;
	}
	HRESULT hr = m_pSourceVoice->SubmitSourceBuffer(&buffer);
	if (FAILED(hr)) {
		std::string error = "Wave is not played!";
		ErrorLog(error);
		return;
	}
	m_pSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
}

/*!
	@brief	停止
*/
void Wave::Stop()
{
	m_pSourceVoice->Stop(0, XAUDIO2_COMMIT_NOW);
}

