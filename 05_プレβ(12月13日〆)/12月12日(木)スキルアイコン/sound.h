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
		SOUND_LABEL_BGM003,			// リザルト
		SOUND_LABEL_BGM004,			// ランキング
		SOUND_LABEL_BGM005,			// チュートリアル
		SOUND_LABEL_BGM006,			// セレクト
		SOUND_LABEL_SE007,			// 決定音
		SOUND_LABEL_SE008,			// キャンセル
		SOUND_LABEL_SE009,			// セレクト
		SOUND_LABEL_SE010,			// カウントダウン（開始時）
		SOUND_LABEL_SE011,			// ゲームスタート
		SOUND_LABEL_SE012,			// タイムアナウンス
		SOUND_LABEL_SE013,			// カウントダウン（終了時）
		SOUND_LABEL_SE014,			// ゲーム終了
		SOUND_LABEL_SE015,			// リザルト：順位発表
		SOUND_LABEL_SE016,			// ポーズ開始
		SOUND_LABEL_SE017,			// ポイント獲得
		SOUND_LABEL_SE018,			// 足音
		SOUND_LABEL_SE019,			// ダウン
		SOUND_LABEL_SE020,			// 速度型攻撃
		SOUND_LABEL_SE021,			// 速度型攻撃命中
		SOUND_LABEL_SE022,			// パワー型攻撃
		SOUND_LABEL_SE023,			// パワー型攻撃命中
		SOUND_LABEL_SE024,			// 地雷型攻撃
		SOUND_LABEL_SE025,			// 地雷型攻撃命中
		SOUND_LABEL_SE026,			// 領地確保
		SOUND_LABEL_SE027,			// 歓声1（おぉー！）
		SOUND_LABEL_SE028,			// 歓声2（がっかり）
		SOUND_LABEL_SE029,			// 領地確保妨害
		SOUND_LABEL_SE030,			// スピードアップ
		SOUND_LABEL_SE031,			// セレクト画面の完成		 
		SOUND_LABEL_SE032,			// レディートゥファイトの音　
		SOUND_LABEL_SE033,			// スキルアイコンがたまった時	安　
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
