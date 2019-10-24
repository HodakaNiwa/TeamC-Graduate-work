//=============================================================================
//
// サウンド処理 [sound.h]
// Author : SATO ASUMI
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=============================================================================
// サウンドクラス
//=============================================================================
class CSound
{
public:
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,		// タイトル
		SOUND_LABEL_BGM001,			// ゲーム
		SOUND_LABEL_BGM002,			// リザルト
		SOUND_LABEL_BGM003,			// ゲームオーバー
		SOUND_LABEL_BGM004,			// ランキング
		SOUND_LABEL_BGM005,			// チュートリアル
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	CSound();
	~CSound();
	HRESULT InitSound(HWND hWnd);
	void UninitSound(void);
	static HRESULT PlaySound(SOUND_LABEL label);
	static void StopSound(SOUND_LABEL label);
	static void StopSound(void);
	static void SetVolume(SOUND_LABEL label, float fVolume);

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	static IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのポインタ
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイスへのポインタ
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイスへのポインタ
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータへのポインタ
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
};
#endif
