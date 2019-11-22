//*****************************************************************************
//
//     UIの処理[UI.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "manager.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define UI_RANKICON_NUM (3)	// 順位アイコンの総数

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CScoreGame;
class CMiniMap;
class CCharacter;
class CSkilicon;
class CCharaicon;
class CIcon2D;
class CGame;

//*****************************************************************************
//    アイコン(ビルボード)クラスの定義
//*****************************************************************************
class CIconBillboard
{
public:    // 誰でもアクセス可能
	CIconBillboard();
	~CIconBillboard();

	static CIconBillboard *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, bool bLighting = false, float fRot = 0.0f, float fTexU = 0.0f, float fTexV = 0.0f, float fTexWidth = 1.0f, float fTexHeight = 1.0f);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void CalcNotScaleTransform(const LPDIRECT3DDEVICE9 pDevice);
	void SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice);
	void SetScale(float fWidth, float fHeight);
	virtual void SetVtxBuffValue(void);
	virtual void SetVtxBuffPos(void);
	void SetVtxBuffNor(void);
	void SetVtxBuffCol(void);
	void SetVtxBuffTex(void);


	//--------------------
	//	Set & Get
	//--------------------
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff) { m_pVtxBuff = pVtxBuff; }
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetPos(const D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetCol(const D3DXCOLOR col) { m_Col = col; }
	void SetRot(const float fRot) { m_fRot = fRot; }
	void SetWidth(const float fWidth) { m_fWidth = fWidth; }
	void SetHeight(const float fHeight) { m_fHeight = fHeight; }
	void SetAngle(const float fAngle) { m_fAngle = fAngle; }
	void SetLength(const float fLength) { m_fLength = fLength; }
	void SetTexU(const float fTexU) { m_fTexU = fTexU; }
	void SetTexV(const float fTexV) { m_fTexV = fTexV; }
	void SetTexWidth(const float fTexWidth) { m_fTexWidth = fTexWidth; }
	void SetTexHeight(const float fTexHeight) { m_fTexHeight = fTexHeight; }
	void SetMtxWorld(const D3DXMATRIX mtxWorld) { m_MtxWorld = mtxWorld; }
	void SetLighting(const bool bLighting) { m_bLighting = bLighting; }

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }
	D3DXCOLOR GetCol(void) { return m_Col; }
	float GetRot(void) { return m_fRot; }
	float GetWidth(void) { return m_fHeight; }
	float GetHeight(void) { return m_fHeight; }
	float GetAngle(void) { return m_fAngle; }
	float GetLength(void) { return m_fLength; }
	float GetTexU(void) { return m_fTexU; }
	float GetTexV(void) { return m_fTexV; }
	float GetTexWidth(void) { return m_fTexWidth; }
	float GetTexHeight(void) { return m_fTexHeight; }
	D3DXMATRIX GetMtxWorld(void) { return m_MtxWorld; }
	bool GetLighting(void) { return m_bLighting; }

protected: // このクラスと派生クラスだけがアクセス可能
	void ClearVariable(void);

private:   // このクラスだけがアクセス可能
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	    // テクスチャへのポインタ
	D3DXVECTOR3             m_Pos;          // ポリゴンの座標
	D3DXCOLOR               m_Col;          // ポリゴンの色
	float                   m_fRot;         // ポリゴンの向き
	float                   m_fWidth;       // ポリゴンの幅
	float                   m_fHeight;      // ポリゴンの高さ
	float                   m_fAngle;       // ポリゴンを出す角度
	float                   m_fLength;      // ポリゴンの長さ
	float                   m_fTexU;        // ポリゴンの左上テクスチャU座標
	float                   m_fTexV;        // ポリゴンの左上テクスチャV座標
	float                   m_fTexWidth;    // ポリゴンのテクスチャ座標の横幅
	float                   m_fTexHeight;   // ポリゴンのテクスチャ座標の縦幅
	D3DXMATRIX              m_MtxWorld;     // ワールドマトリックス
	bool                    m_bLighting;    // ライティングするかしないか
};

//*****************************************************************************
//    UIクラスの定義
//*****************************************************************************
class CUI
{
public:    // 誰でもアクセス可能
	CUI();
	~CUI();

	static CUI *Create(int nNumPlayer, int nAllCharacter);

	HRESULT Init(int nNumPlayer, int nAllCharacter);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateSkilicon(CCharacter *pChara, int nIdx, int nRecastTime);
	void DrawIcon(int nPlayerIdx);
	void BindTextureToIcon(void);
	void SortRankFromScore(void);

	//--------------------
	//	Set & Get
	//--------------------
	void SetNumPlayer(const int nNumPlayer) { m_nNumPlayer = nNumPlayer; }
	void SetNumAllCharacter(const int nNumAllCharacter) { m_nNumAllCharacter = nNumAllCharacter; }
	void SetScoreGame(CScoreGame *pScoreGame, const int nIdx) { m_pScoreGame[nIdx] = pScoreGame; }
	void SetMiniMap(CMiniMap *pMiniMap) { m_pMiniMap = pMiniMap; }
	void SetSkilicon(CSkilicon *pSkilicon, const int nIdx) { m_pSkilicon[nIdx] = pSkilicon; }
	void SetCharaicon(CCharaicon *pCharaicon, const int nPlayerIdx, const int nIdx) { m_pChraicon[nPlayerIdx][nIdx] = pCharaicon; }
	void SetPlayerIdxicon(CIconBillboard *pPlayerIdxicon, const int nIdx) { m_pPlayerIdxicon[nIdx] = pPlayerIdxicon; }
	void SetCrownicon(CIconBillboard *pCrownicon, const int nIdx) { m_pCrownicon[nIdx] = pCrownicon; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	int GetNumAllCharacter(void) { return m_nNumAllCharacter; }
	CScoreGame *GetScoreGame(const int nIdx) { return m_pScoreGame[nIdx]; }
	CMiniMap *GetMiniMap(void) { return m_pMiniMap; }
	CSkilicon *GetSkilicon(const int nIdx) { return m_pSkilicon[nIdx]; }
	CCharaicon *GetCharaicon(const int nPlayerIdx, const int nIdx) { return m_pChraicon[nPlayerIdx][nIdx]; }
	CIconBillboard *GetPlayerIdxicon(const int nIdx) { return m_pPlayerIdxicon[nIdx]; }
	CIconBillboard *GetCrownicon(const int nIdx) { return m_pCrownicon[nIdx]; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	//--------------------
	//	関数
	//--------------------
	void CreateScoreGame(void);
	void CreateMiniMap(void);
	void CreateCharaicon(void);
	void CreatePlayerIdxicon(void);
	void CreateCrownRank(void);
	void BindTextureToIcon_Chara(CGame *pGame);
	void BindTextureToIcon_PlayerIdx(CGame *pGame);
	void BindTextureToIcon_Crown(CGame *pGame);

	void ReleaseScoreGame(void);
	void ReleaseMiniMap(void);
	void ReleaseSkilicon(void);
	void ReleaseCharaicon(void);
	void ReleasePlayerIdxicon(void);
	void ReleaseCrownIcon(void);

	void UpdateSkilicon(void);
	void UpdateCharaicon(void);
	void UpdatePlayerIdxicon(CGame *pGame);
	void UpdateCrownicon(CGame *pGame);

	void DrawSkilicon(void);
	void DrawCharaicon(void);
	void DrawPlayerIdxicon(void);
	void DrawCrownIcon(void);

	//--------------------
	//	メンバ変数
	//--------------------
	int m_nNumPlayer;											// ゲームに参加しているプレイヤーの人数(保存用)
	int m_nNumAllCharacter;										// ゲームに参加している人数(保存用)
	CScoreGame *m_pScoreGame[MAX_PLAYERNUM * 2];				// ゲームスコアクラスへのポインタ
	CMiniMap *m_pMiniMap;										// ミニマップクラスへのポインタ
	CSkilicon *m_pSkilicon[MAX_PLAYERNUM];						// スキルアイコンクラスへのポインタ
	CCharaicon *m_pChraicon[MAX_PLAYERNUM][UI_RANKICON_NUM];	// キャラアイコンへのポインタ
	int m_nRank[MAX_PLAYERNUM * 2];								// 現在の順位(8人分)
	CIconBillboard *m_pPlayerIdxicon[MAX_PLAYERNUM * 2];		// プレイヤー番号アイコン
	CIconBillboard *m_pCrownicon[UI_RANKICON_NUM];				// 王冠アイコン
	LPDIRECT3DTEXTURE9 *m_pTexture;								// ゲームクラスが保有するテクスチャへのポインタ
};

#endif