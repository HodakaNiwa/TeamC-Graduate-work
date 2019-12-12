//*****************************************************************************
//
//     スキルアイコンの処理[skilicon.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SKILICON_H_
#define _SKILICON_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define SKILICON_TEXTURE_MAX (6)
#define SKILLICON_GROW_TEX	 (4)	//安

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CScene2D;
class CCharacter;
class CScene2D;

//*****************************************************************************
//    スキルアイコン用ポリゴンクラスの定義
//*****************************************************************************
class CIcon2D
{
public:     // 誰でもアクセス可能
	CIcon2D();
	~CIcon2D();

	static CIcon2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fTexU = 0.0f, float fTexV = 0.0f, float fTexWidth = 1.0f, float fTexHeight = 1.0f, float fRot = 0.0f);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetScale(float fWidth, float fHeight);
	virtual void SetVtxBuffValue(void);
	virtual void SetVtxBuffPos(void);
	void SetVtxBuffCol(void);
	void SetVtxBuffTex(void);

	//--------------------
	//	Set & Get
	//--------------------
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff) { m_pVtxBuff = pVtxBuff; }
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
	void SetUpdateFlag(bool bUpdate) { m_bUpdate = bUpdate; }	//安
	void SetEffectFlag(bool bEffect) { m_bEffect = bEffect; }	//安
	void SetRingFlag(bool bRing) { m_bRing = bRing; }			//安
	void SetLife(int nLife) { m_nLife = nLife; }				//安
	void SetOneTime(bool bOneTime) { m_bOneTime = bOneTime; }	//安

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }
	D3DXCOLOR GetCol(void) { return m_Col; }
	float GetRot(void) { return m_fRot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetAngle(void) { return m_fAngle; }
	float GetLength(void) { return m_fLength; }
	float GetTexU(void) { return m_fTexU; }
	float GetTexV(void) { return m_fTexV; }
	float GetTexWidth(void) { return m_fTexWidth; }
	float GetTexHeight(void) { return m_fTexHeight; }
	bool GetUpdateFlag(void) { return m_bUpdate; }	//安
	bool GetEffectFlag(void) { return m_bEffect; }	//安
	bool GetRingFlag(void) { return m_bRing; }	//安
	bool GetOneTime(void) { return m_bOneTime;}		//安

protected: // このクラスと派生クラスだけがアクセス可能
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

private:   // このクラスだけがアクセス可能
	void UpdateGrow(void);
	void UpdateRight(void);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	    // テクスチャへのポインタ
	D3DXVECTOR3				m_Pos;	        // ポリゴンの位置
	D3DXCOLOR               m_Col;          // ポリゴンの色
	float                   m_fRot;         // ポリゴンの向き
	float                   m_fAngle;       // ポリゴンを出す角度
	float                   m_fWidth;       // ポリゴンの幅
	float                   m_fHeight;      // ポリゴンの高さ
	float                   m_fLength;      // ポリゴンの長さ
	float                   m_fTexU;        // ポリゴンの左上テクスチャU座標
	float                   m_fTexV;        // ポリゴンの左上テクスチャV座標
	float                   m_fTexWidth;    // ポリゴンのテクスチャ座標の横幅
	float                   m_fTexHeight;   // ポリゴンのテクスチャ座標の縦幅

	//安
	bool					m_bUpdate;		// 更新するかどうか
	bool					m_bEffect;		// エフェクトを更新するかどうか
	bool					m_bOneTime;		// 一度だけやったかどうか
	bool					m_bRing;		// リングの更新処理
	float					m_fChangeCol;	// 色の変化量
	int						m_nLife;		// 表示時間
};

//*****************************************************************************
//    スキルアイコンクラスの定義
//*****************************************************************************
class CSkilicon
{
public:    // 誰でもアクセス可能
	CSkilicon();
	~CSkilicon();

	static CSkilicon *Create(CCharacter *pChara, int nIdx, int nRecastTime);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(CCharacter *pChara, int nIdx, int nRecastTime);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RevivalIconMask(void);
	void RevivalIconMask_teq(void);

	//--------------------
	//	Set & Get
	//--------------------
	void SetIconBG(CIcon2D *pIconBG) { m_pIconBG = pIconBG; }
	void SetIcon(CIcon2D *pIcon) { m_pIcon = pIcon; }
	void SetIconMask(CIcon2D *pIconMask) { m_pIconMask = pIconMask; }
	void SetParentChara(CCharacter *pChara) { m_pParentChara = pChara; }
	void SetIdx(const int nIdx) { m_nIdx = nIdx; }
	void SetRecastTime(const int nRecastTime) { m_nRecastTime = nRecastTime; }

	CIcon2D *GetIconBG(void) { return m_pIconBG; }
	CIcon2D *GetIcon(void) { return m_pIcon; }
	CIcon2D *GetIconMask(void) { return m_pIconMask; }
	CCharacter *GetCharacter(void) { return m_pParentChara; }
	int GetIdx(void) { return m_nIdx; }
	int GetRecastTime(void) { return m_nRecastTime; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	//--------------------
	//	関数
	//--------------------
	void CreateIconBG(void);
	void CreateIconMask(void);
	void CreateIcon(void);
	void CreateIconGrow(void);	//安
	void CreateIconRight(void);	//安
	void ReleaseIconBG(void);
	void ReleaseIconMask(void);
	void ReleaseIcon(void);
	void ReleaseIconGrow(void);	//安
	void ReleaseIconRight(void);	//安
	void RecastIcon(void);
	void DrawIconBG(void);
	void DrawIconMask(const LPDIRECT3DDEVICE9 pDevice);
	void DrawIcon(const LPDIRECT3DDEVICE9 pDevice);
	void DrawIconGrow(const LPDIRECT3DDEVICE9 pDevice);	//安
	void DrawIconRight(const LPDIRECT3DDEVICE9 pDevice);	//安
	void ClearStencil(const LPDIRECT3DDEVICE9 pDevice);

	//--------------------
	//	静的メンバ変数
	//--------------------
	static LPDIRECT3DTEXTURE9 m_apTexture[SKILICON_TEXTURE_MAX];
	
	//安
	static LPDIRECT3DTEXTURE9 m_apTectureGrow[SKILLICON_GROW_TEX];

	//--------------------
	//	メンバ変数
	//--------------------
	CIcon2D *m_pIconBG;				// スキルアイコン(背景)
	CIcon2D *m_pIconMask;			// スキルアイコンマスク用
	CIcon2D *m_pIcon;				// スキルアイコン
	CIcon2D * m_pGrowIcon;			// アイコンの光
	CIcon2D * m_pRightEffect;		// 光のエフェクト
	CIcon2D * m_pRingEffect;		// 円のエフェクト
	CCharacter *m_pParentChara;		// 状態を表示するキャラクターへのポインタ
	int m_nIdx;						// 自身の番号
	int m_nRecastTime;				// リキャストにかかる時間
};

#endif