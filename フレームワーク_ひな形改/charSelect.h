//=============================================================================
//
// キャラ選択の処理 [charselect.h]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#ifndef _SELECTPLAYER_H_
#define _SELECTPLAYER_H_

//=================================================
//		include / 他のデータ・コードの呼び出し・利用
//=================================================
#include "main.h"
#include "select.h"

//==============================================
//					マクロ定義
//==============================================
#define MAX_TYPE	(3)
#define MAX_CUNTRY	(8)
#define SELECTCHAR_TEX	(2)

//==============================================
//					前方宣言
//==============================================
class CMoveUI;
class CCharMultRender;

//==============================================
//			キャラクターの派生 → プレイヤー
//==============================================
class CSelectCharacter
{
public:
	typedef enum
	{
		STATE_SPEED = 0,
		STATE_PAWER,
		STATE_MINE,
		STATE_RANDUM,
		STATE_MAX,
	}STATE;

	CSelectCharacter();
	~CSelectCharacter();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nCuntry, int nNumPlayer, int nNumControler);
	void Uninit(void);
	void Update(void);
	static CSelectCharacter * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nCuntry, int nNumPlayer, int nNumControler);
	static void LoadTex(void);
	static void UnloadTex(void);
	void UpdateScaleUp(D3DXVECTOR3 AimSize, D3DXVECTOR2 Move);
	void UpdateScaleDown(D3DXVECTOR3 AimSize, D3DXVECTOR2 Move);
	void UninitUI(void);
	bool GetbEnter(void) { return m_bEnter; }
	int GetType(void) { return m_nType; }
	int GetCuntry(void) { return m_nCuntry; }
	void SetInputType(CSelect::INPUTTYPE InputType) { m_InputType = InputType; }	//入力デバイスのタイプを設定

private:
	void LoadMultRendering(void);
	void UpdateInput(void);
	void MoveRight(void);
	void MoveLeft(void);
	void PressEnter(void);

	static LPDIRECT3DTEXTURE9 m_pTexture[SELECTCHAR_TEX];
	LPDIRECT3DTEXTURE9 m_pStatusTex;
	CMoveUI * m_pMoveUI;
	CMoveUI * m_pTypeName;
	CMoveUI * m_pStatus;
	CMoveUI * m_pEnter;
	CCharMultRender * m_pCharMultRender[MAX_TYPE];
	int m_nCuntry;
	int m_nType;
	int m_nOldType;
	int m_nNumPlayer;
	int m_nNumControler;
	int m_nCountTime;
	bool m_bEnter;
	STATE m_state;
	CSelect::INPUTTYPE m_InputType;	//入力デバイスの種類
};
#endif