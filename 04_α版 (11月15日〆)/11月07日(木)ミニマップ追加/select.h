//=============================================================================
//
// セレクト処理 [select.h]
// Author : 佐藤 安純
//
//=============================================================================
#ifndef _SELECT_H_
#define _SELECT_H_

#include "main.h"
#include "modebase.h"

//=================================
// マクロ定義
//=================================
#define MAX_COUNTRY	(8)
#define NUMPLAYER	(4)
#define SELECT_TEX	(2)

//=================================
// 前方宣言
//=================================
class CCamera;
class CFieldManager;
class CLoadTextObject;
class CLoadTextTerritory;
class CMoveUI;
class CPlayerIcon;

//=================================
// セレクトクラス
//=================================
class CSelect : public CModeBase
{
public:
	typedef enum
	{
		STATE_CUNTRY_SELECT = 0,	//国選択
		STATE_CHARACTER_SELECT,		//キャラ選択
		STATE_DICISION,				//決定
		STATE_CHANGE,				//変更状態
	}STATE;

	CSelect();				// コンストラクタ
	~CSelect();				// デストラクタ
	void Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理
	static CMoveUI * GetNatonalFlag(int nCuntry) { return m_pNationalFlag[nCuntry]; }	//国旗のポインタを取得
	static int GetEntryPlayer(void) { return m_nEntryPlayer; }
	LPDIRECT3DTEXTURE9 GetCuntryTex(int nTexNum) { return m_apTextureCountry[nTexNum]; }
	void SetState(STATE state) { m_state = state; }

private:
	void LoadTexture(void);
	void UnloadTexture(void);
	void InitCuntrySelect(void);
	void UpdateCuntrySelect(void);
	void InitCharacterSelect(void);
	void UpdateCharacterSelect(void);
	void UninitCharacterSelect(void);
	void UpdateDicision(void);
	void UpdateChange(void);

	static CMoveUI * m_pNationalFlag[MAX_COUNTRY];		//国の数
	static int m_nEntryPlayer;							//プレイヤーのエントリー数

	LPDIRECT3DTEXTURE9 m_apTextureCountry[MAX_COUNTRY];	//テクスチャ
	LPDIRECT3DTEXTURE9 m_apTexPlayer[NUMPLAYER];		//プレイヤー
	LPDIRECT3DTEXTURE9 m_apTexCuntoryName[MAX_COUNTRY];	//国の名前
	LPDIRECT3DTEXTURE9 m_pTexNoEntry;					//エントリーテクスチャ
	LPDIRECT3DTEXTURE9 m_pTex[SELECT_TEX];				//その他のテクスチャ
	CCamera * m_pCamera;								//カメラ
	CFieldManager * m_pFieldManager;					//フィールドマネージャー
	CLoadTextObject * m_pLoadTextObj;					//オブジェクト読み込み
	CLoadTextTerritory * m_pLoadTerritory;				// テリトリーの読み込み
	CPlayerIcon * m_pNumPlayer[NUMPLAYER];				//プレイヤーの選択画像
	CMoveUI * m_pCuntoryName[NUMPLAYER];				//国の名前表示
	CMoveUI * m_pNoEntry[NUMPLAYER];					//プレイヤーのエントリーしているかどうか
	CMoveUI * m_pLogo;									//ロゴUI
	CMoveUI * m_pFram[NUMPLAYER];						//フレーム
	STATE m_state;										//状態設定
};
#endif